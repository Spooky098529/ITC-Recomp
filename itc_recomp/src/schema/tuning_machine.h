// The views of docs/functions/tuning-machine.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/tuning-machine.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: TuningMachine
struct TuningMachineScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 36828;
  // 200 transitions, 48 bytes each (StateMachine's); the measure's +0 to +12 are ZeroRecord16's record (+30800, +30816), a measure fault
  uint32_t transitions_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // the machine's state: 1 open the movies, 2 wait for them, 9 wait for message 12, 4 start, 5 running, 8 unloading, 7 closing, 6 done
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // movies 2, 13 and 14 loaded
  uint32_t movies_loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_movies_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t movies_loaded_address() const { return address + 9608; }
  // nonzero: wait for message 12 after loading (1 from the mode's Init)
  uint32_t reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
  // nonzero: Unload deletes movies 2, 13 and 14
  uint32_t delete_movies() const { return guest::LoadU32(base, address + 9616); }
  void set_delete_movies(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t delete_movies_address() const { return address + 9616; }
  // the screen's state, 3 to 58 (3 TUNING SHOP MAIN, 4 the category menu, 9 CONF BUY, 10 the money short, 11 CONF EQUIP, ...)
  uint32_t screen_state() const { return guest::LoadU32(base, address + 9652); }
  void set_screen_state(uint32_t v) const { guest::StoreU32(base, address + 9652, v); }
  uint32_t screen_state_address() const { return address + 9652; }
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
  uint8_t bonnet_choice() const { return guest::LoadU8(base, address + 9663); }
  void set_bonnet_choice(uint8_t v) const { guest::StoreU8(base, address + 9663, v); }
  uint32_t bonnet_choice_address() const { return address + 9663; }
  // the option pick (color, hardness or door type)
  uint8_t option() const { return guest::LoadU8(base, address + 9664); }
  void set_option(uint8_t v) const { guest::StoreU8(base, address + 9664, v); }
  uint32_t option_address() const { return address + 9664; }
  // the dialogs 9 and 11's cursor
  uint8_t confirm_cursor() const { return guest::LoadU8(base, address + 9665); }
  void set_confirm_cursor(uint8_t v) const { guest::StoreU8(base, address + 9665, v); }
  uint32_t confirm_cursor_address() const { return address + 9665; }
  uint8_t unknown_9666() const { return guest::LoadU8(base, address + 9666); }
  void set_unknown_9666(uint8_t v) const { guest::StoreU8(base, address + 9666, v); }
  uint32_t unknown_9666_address() const { return address + 9666; }
  // dialog 12's cursor (probably)
  uint8_t keep_cursor() const { return guest::LoadU8(base, address + 9667); }
  void set_keep_cursor(uint8_t v) const { guest::StoreU8(base, address + 9667, v); }
  uint32_t keep_cursor_address() const { return address + 9667; }
  // dialog 13's cursor (probably)
  uint8_t discard_cursor() const { return guest::LoadU8(base, address + 9668); }
  void set_discard_cursor(uint8_t v) const { guest::StoreU8(base, address + 9668, v); }
  uint32_t discard_cursor_address() const { return address + 9668; }
  // dialog 14's cursor (probably)
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
  // 17 at creation
  uint32_t unknown_9684() const { return guest::LoadU32(base, address + 9684); }
  void set_unknown_9684(uint32_t v) const { guest::StoreU32(base, address + 9684, v); }
  uint32_t unknown_9684_address() const { return address + 9684; }
  // a mask of the items state 4 opens; -1 at creation
  uint32_t open_mask() const { return guest::LoadU32(base, address + 9688); }
  void set_open_mask(uint32_t v) const { guest::StoreU32(base, address + 9688, v); }
  uint32_t open_mask_address() const { return address + 9688; }
  // -1 at creation
  uint32_t unknown_9692() const { return guest::LoadU32(base, address + 9692); }
  void set_unknown_9692(uint32_t v) const { guest::StoreU32(base, address + 9692, v); }
  uint32_t unknown_9692_address() const { return address + 9692; }
  // -1 at creation
  uint32_t unknown_9696() const { return guest::LoadU32(base, address + 9696); }
  void set_unknown_9696(uint32_t v) const { guest::StoreU32(base, address + 9696, v); }
  uint32_t unknown_9696_address() const { return address + 9696; }
  // -1 at creation
  uint32_t unknown_9700() const { return guest::LoadU32(base, address + 9700); }
  void set_unknown_9700(uint32_t v) const { guest::StoreU32(base, address + 9700, v); }
  uint32_t unknown_9700_address() const { return address + 9700; }
  // -1 at creation
  uint32_t unknown_9704() const { return guest::LoadU32(base, address + 9704); }
  void set_unknown_9704(uint32_t v) const { guest::StoreU32(base, address + 9704, v); }
  uint32_t unknown_9704_address() const { return address + 9704; }
  // the refresh sub_821E3AD0 runs while 1
  uint32_t refresh() const { return guest::LoadU32(base, address + 9708); }
  void set_refresh(uint32_t v) const { guest::StoreU32(base, address + 9708, v); }
  uint32_t refresh_address() const { return address + 9708; }
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
  uint32_t menu_choice() const { return guest::LoadU32(base, address + 9744); }
  void set_menu_choice(uint32_t v) const { guest::StoreU32(base, address + 9744, v); }
  uint32_t menu_choice_address() const { return address + 9744; }
  // OnDialogNo copies the category menu's cursor here
  uint32_t unknown_9748() const { return guest::LoadU32(base, address + 9748); }
  void set_unknown_9748(uint32_t v) const { guest::StoreU32(base, address + 9748, v); }
  uint32_t unknown_9748_address() const { return address + 9748; }
  uint32_t unknown_9752() const { return guest::LoadU32(base, address + 9752); }
  void set_unknown_9752(uint32_t v) const { guest::StoreU32(base, address + 9752, v); }
  uint32_t unknown_9752_address() const { return address + 9752; }
  // 63 states (words), pushed before a sub-page, popped on B; no bound check
  uint32_t state_history_address() const { return address + 9756; }
  uint8_t history_depth() const { return guest::LoadU8(base, address + 10008); }
  void set_history_depth(uint8_t v) const { guest::StoreU8(base, address + 10008, v); }
  uint32_t history_depth_address() const { return address + 10008; }
  // a pending stand action (1 to 10), -1 none
  uint32_t stand_action() const { return guest::LoadU32(base, address + 10012); }
  void set_stand_action(uint32_t v) const { guest::StoreU32(base, address + 10012, v); }
  uint32_t stand_action_address() const { return address + 10012; }
  // the busy icon shown
  uint32_t busy_shown() const { return guest::LoadU32(base, address + 10016); }
  void set_busy_shown(uint32_t v) const { guest::StoreU32(base, address + 10016, v); }
  uint32_t busy_shown_address() const { return address + 10016; }
  // the busy icon's movie (2)
  uint32_t busy_movie() const { return guest::LoadU32(base, address + 10020); }
  void set_busy_movie(uint32_t v) const { guest::StoreU32(base, address + 10020, v); }
  uint32_t busy_movie_address() const { return address + 10020; }
  // a named animation (172)
  uint32_t busy_animation_a() const { return guest::LoadU32(base, address + 10024); }
  void set_busy_animation_a(uint32_t v) const { guest::StoreU32(base, address + 10024, v); }
  uint32_t busy_animation_a_address() const { return address + 10024; }
  // a named animation (171)
  uint32_t busy_animation_b() const { return guest::LoadU32(base, address + 10028); }
  void set_busy_animation_b(uint32_t v) const { guest::StoreU32(base, address + 10028, v); }
  uint32_t busy_animation_b_address() const { return address + 10028; }
  // a named animation (173)
  uint32_t busy_animation_c() const { return guest::LoadU32(base, address + 10032); }
  void set_busy_animation_c(uint32_t v) const { guest::StoreU32(base, address + 10032, v); }
  uint32_t busy_animation_c_address() const { return address + 10032; }
  // the command queue (28 bytes, VirtualAlloc'ed by Create)
  template <typename T = PageObject>
  T queue() const { return T(base, guest::LoadU32(base, address + 10036)); }
  void set_queue(const View& v) const { guest::StoreU32(base, address + 10036, v.address); }
  void set_queue(uint32_t v) const { guest::StoreU32(base, address + 10036, v); }
  uint32_t queue_address() const { return address + 10036; }
  template <typename T = FrontEndMenu>
  T category_menu() const { return T(base, guest::LoadU32(base, address + 10040)); }
  void set_category_menu(const View& v) const { guest::StoreU32(base, address + 10040, v.address); }
  void set_category_menu(uint32_t v) const { guest::StoreU32(base, address + 10040, v); }
  uint32_t category_menu_address() const { return address + 10040; }
  // the sticker editor's menu
  template <typename T = FrontEndMenu>
  T second_menu() const { return T(base, guest::LoadU32(base, address + 10044)); }
  void set_second_menu(const View& v) const { guest::StoreU32(base, address + 10044, v.address); }
  void set_second_menu(uint32_t v) const { guest::StoreU32(base, address + 10044, v); }
  uint32_t second_menu_address() const { return address + 10044; }
  // DecalImages (tuning-pages.toml), embedded
  uint32_t decal_images_address() const { return address + 10048; }
  // player 0's save data
  template <typename T = SaveData>
  T save_data() const { return T(base, guest::LoadU32(base, address + 18904)); }
  void set_save_data(const View& v) const { guest::StoreU32(base, address + 18904, v.address); }
  void set_save_data(uint32_t v) const { guest::StoreU32(base, address + 18904, v); }
  uint32_t save_data_address() const { return address + 18904; }
  // the car data's +0 (the car data embedded at +18912)
  float car_body_weight() const { return guest::LoadFloat(base, address + 18912); }
  void set_car_body_weight(float v) const { guest::StoreFloat(base, address + 18912, v); }
  uint32_t car_body_weight_address() const { return address + 18912; }
  // the car data's +400 (an axle's, probably)
  float car_front_weight() const { return guest::LoadFloat(base, address + 19312); }
  void set_car_front_weight(float v) const { guest::StoreFloat(base, address + 19312, v); }
  uint32_t car_front_weight_address() const { return address + 19312; }
  // the car data's +672 (an axle's, probably)
  float car_rear_weight() const { return guest::LoadFloat(base, address + 19584); }
  void set_car_rear_weight(float v) const { guest::StoreFloat(base, address + 19584, v); }
  uint32_t car_rear_weight_address() const { return address + 19584; }
  // the car data's +1288
  float car_displacement() const { return guest::LoadFloat(base, address + 20200); }
  void set_car_displacement(float v) const { guest::StoreFloat(base, address + 20200, v); }
  uint32_t car_displacement_address() const { return address + 20200; }
  uint32_t stat_displacement() const { return guest::LoadU32(base, address + 20768); }
  void set_stat_displacement(uint32_t v) const { guest::StoreU32(base, address + 20768, v); }
  uint32_t stat_displacement_address() const { return address + 20768; }
  uint32_t stat_weight() const { return guest::LoadU32(base, address + 20772); }
  void set_stat_weight(uint32_t v) const { guest::StoreU32(base, address + 20772, v); }
  uint32_t stat_weight_address() const { return address + 20772; }
  // the peak power
  uint32_t stat_power() const { return guest::LoadU32(base, address + 20776); }
  void set_stat_power(uint32_t v) const { guest::StoreU32(base, address + 20776, v); }
  uint32_t stat_power_address() const { return address + 20776; }
  // the torque shown
  float stat_torque() const { return guest::LoadFloat(base, address + 20780); }
  void set_stat_torque(float v) const { guest::StoreFloat(base, address + 20780, v); }
  uint32_t stat_torque_address() const { return address + 20780; }
  // the camera preset command last picked
  uint32_t camera_preset() const { return guest::LoadU32(base, address + 25072); }
  void set_camera_preset(uint32_t v) const { guest::StoreU32(base, address + 25072, v); }
  uint32_t camera_preset_address() const { return address + 25072; }
  // the part slot last previewed
  uint32_t previewed_slot() const { return guest::LoadU32(base, address + 25076); }
  void set_previewed_slot(uint32_t v) const { guest::StoreU32(base, address + 25076, v); }
  uint32_t previewed_slot_address() const { return address + 25076; }
  // 4 at Start
  uint32_t unknown_25080() const { return guest::LoadU32(base, address + 25080); }
  void set_unknown_25080(uint32_t v) const { guest::StoreU32(base, address + 25080, v); }
  uint32_t unknown_25080_address() const { return address + 25080; }
  // the list's scrolled row (category 4) or the level for long lists
  uint32_t list_row() const { return guest::LoadU32(base, address + 25100); }
  void set_list_row(uint32_t v) const { guest::StoreU32(base, address + 25100, v); }
  uint32_t list_row_address() const { return address + 25100; }
  uint32_t list_previous_row() const { return guest::LoadU32(base, address + 25104); }
  void set_list_previous_row(uint32_t v) const { guest::StoreU32(base, address + 25104, v); }
  uint32_t list_previous_row_address() const { return address + 25104; }
  uint32_t money() const { return guest::LoadU32(base, address + 25116); }
  void set_money(uint32_t v) const { guest::StoreU32(base, address + 25116, v); }
  uint32_t money_address() const { return address + 25116; }
  uint32_t last_price() const { return guest::LoadU32(base, address + 25120); }
  void set_last_price(uint32_t v) const { guest::StoreU32(base, address + 25120, v); }
  uint32_t last_price_address() const { return address + 25120; }
  // 0xFF at Start
  uint8_t unknown_25124() const { return guest::LoadU8(base, address + 25124); }
  void set_unknown_25124(uint8_t v) const { guest::StoreU8(base, address + 25124, v); }
  uint32_t unknown_25124_address() const { return address + 25124; }
  // 0 both, 1 front, 2 rear
  uint32_t wheel_part_cursor() const { return guest::LoadU32(base, address + 25196); }
  void set_wheel_part_cursor(uint32_t v) const { guest::StoreU32(base, address + 25196, v); }
  uint32_t wheel_part_cursor_address() const { return address + 25196; }
  uint32_t unknown_25200() const { return guest::LoadU32(base, address + 25200); }
  void set_unknown_25200(uint32_t v) const { guest::StoreU32(base, address + 25200, v); }
  uint32_t unknown_25200_address() const { return address + 25200; }
  // the part group or wheel brand (0 to 16)
  uint32_t wheel_brand() const { return guest::LoadU32(base, address + 25204); }
  void set_wheel_brand(uint32_t v) const { guest::StoreU32(base, address + 25204, v); }
  uint32_t wheel_brand_address() const { return address + 25204; }
  uint32_t unknown_25208() const { return guest::LoadU32(base, address + 25208); }
  void set_unknown_25208(uint32_t v) const { guest::StoreU32(base, address + 25208, v); }
  uint32_t unknown_25208_address() const { return address + 25208; }
  uint32_t brand_tab_row() const { return guest::LoadU32(base, address + 25212); }
  void set_brand_tab_row(uint32_t v) const { guest::StoreU32(base, address + 25212, v); }
  uint32_t brand_tab_row_address() const { return address + 25212; }
  uint32_t unknown_25216() const { return guest::LoadU32(base, address + 25216); }
  void set_unknown_25216(uint32_t v) const { guest::StoreU32(base, address + 25216, v); }
  uint32_t unknown_25216_address() const { return address + 25216; }
  // the list's cursor (the model in the brand)
  uint32_t wheel_model() const { return guest::LoadU32(base, address + 25220); }
  void set_wheel_model(uint32_t v) const { guest::StoreU32(base, address + 25220, v); }
  uint32_t wheel_model_address() const { return address + 25220; }
  // the list's last index
  uint32_t wheel_model_last() const { return guest::LoadU32(base, address + 25224); }
  void set_wheel_model_last(uint32_t v) const { guest::StoreU32(base, address + 25224, v); }
  uint32_t wheel_model_last_address() const { return address + 25224; }
  // state 17's cursor
  uint32_t wheel_size() const { return guest::LoadU32(base, address + 25228); }
  void set_wheel_size(uint32_t v) const { guest::StoreU32(base, address + 25228, v); }
  uint32_t wheel_size_address() const { return address + 25228; }
  // state 17's last index
  uint32_t wheel_size_last() const { return guest::LoadU32(base, address + 25232); }
  void set_wheel_size_last(uint32_t v) const { guest::StoreU32(base, address + 25232, v); }
  uint32_t wheel_size_last_address() const { return address + 25232; }
  uint32_t unknown_25236() const { return guest::LoadU32(base, address + 25236); }
  void set_unknown_25236(uint32_t v) const { guest::StoreU32(base, address + 25236, v); }
  uint32_t unknown_25236_address() const { return address + 25236; }
  // set by state 16's B
  uint32_t wheel_back_pressed() const { return guest::LoadU32(base, address + 25240); }
  void set_wheel_back_pressed(uint32_t v) const { guest::StoreU32(base, address + 25240, v); }
  uint32_t wheel_back_pressed_address() const { return address + 25240; }
  // 1 claim them, 2 paint them, 0 done
  uint32_t second_wheels_step() const { return guest::LoadU32(base, address + 25244); }
  void set_second_wheels_step(uint32_t v) const { guest::StoreU32(base, address + 25244, v); }
  uint32_t second_wheels_step_address() const { return address + 25244; }
  // hue, saturation, value, the choice, the buttons
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
  // a two-way choice
  uint32_t paint_choice() const { return guest::LoadU32(base, address + 30456); }
  void set_paint_choice(uint32_t v) const { guest::StoreU32(base, address + 30456, v); }
  uint32_t paint_choice_address() const { return address + 30456; }
  uint32_t paint_buttons() const { return guest::LoadU32(base, address + 30460); }
  void set_paint_buttons(uint32_t v) const { guest::StoreU32(base, address + 30460, v); }
  uint32_t paint_buttons_address() const { return address + 30460; }
  // one of the paint page's previous values
  uint32_t unknown_30464() const { return guest::LoadU32(base, address + 30464); }
  void set_unknown_30464(uint32_t v) const { guest::StoreU32(base, address + 30464, v); }
  uint32_t unknown_30464_address() const { return address + 30464; }
  // one of the paint page's previous values
  uint32_t unknown_30468() const { return guest::LoadU32(base, address + 30468); }
  void set_unknown_30468(uint32_t v) const { guest::StoreU32(base, address + 30468, v); }
  uint32_t unknown_30468_address() const { return address + 30468; }
  // one of the paint page's previous values
  uint32_t unknown_30472() const { return guest::LoadU32(base, address + 30472); }
  void set_unknown_30472(uint32_t v) const { guest::StoreU32(base, address + 30472, v); }
  uint32_t unknown_30472_address() const { return address + 30472; }
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
  uint32_t light_kit0_row() const { return guest::LoadU32(base, address + 30508); }
  void set_light_kit0_row(uint32_t v) const { guest::StoreU32(base, address + 30508, v); }
  uint32_t light_kit0_row_address() const { return address + 30508; }
  uint32_t light_kit0_value_0() const { return guest::LoadU32(base, address + 30512); }
  void set_light_kit0_value_0(uint32_t v) const { guest::StoreU32(base, address + 30512, v); }
  uint32_t light_kit0_value_0_address() const { return address + 30512; }
  uint32_t light_kit0_value_1() const { return guest::LoadU32(base, address + 30516); }
  void set_light_kit0_value_1(uint32_t v) const { guest::StoreU32(base, address + 30516, v); }
  uint32_t light_kit0_value_1_address() const { return address + 30516; }
  uint32_t light_kit0_value_2() const { return guest::LoadU32(base, address + 30520); }
  void set_light_kit0_value_2(uint32_t v) const { guest::StoreU32(base, address + 30520, v); }
  uint32_t light_kit0_value_2_address() const { return address + 30520; }
  uint32_t light_kit0_value_3() const { return guest::LoadU32(base, address + 30524); }
  void set_light_kit0_value_3(uint32_t v) const { guest::StoreU32(base, address + 30524, v); }
  uint32_t light_kit0_value_3_address() const { return address + 30524; }
  // read with light_kit0_row by the refresh
  uint32_t unknown_30528() const { return guest::LoadU32(base, address + 30528); }
  void set_unknown_30528(uint32_t v) const { guest::StoreU32(base, address + 30528, v); }
  uint32_t unknown_30528_address() const { return address + 30528; }
  uint32_t unknown_30532() const { return guest::LoadU32(base, address + 30532); }
  void set_unknown_30532(uint32_t v) const { guest::StoreU32(base, address + 30532, v); }
  uint32_t unknown_30532_address() const { return address + 30532; }
  uint32_t unknown_30536() const { return guest::LoadU32(base, address + 30536); }
  void set_unknown_30536(uint32_t v) const { guest::StoreU32(base, address + 30536, v); }
  uint32_t unknown_30536_address() const { return address + 30536; }
  uint32_t unknown_30540() const { return guest::LoadU32(base, address + 30540); }
  void set_unknown_30540(uint32_t v) const { guest::StoreU32(base, address + 30540, v); }
  uint32_t unknown_30540_address() const { return address + 30540; }
  uint32_t light_kit1_row() const { return guest::LoadU32(base, address + 30544); }
  void set_light_kit1_row(uint32_t v) const { guest::StoreU32(base, address + 30544, v); }
  uint32_t light_kit1_row_address() const { return address + 30544; }
  uint32_t light_kit1_value_0() const { return guest::LoadU32(base, address + 30548); }
  void set_light_kit1_value_0(uint32_t v) const { guest::StoreU32(base, address + 30548, v); }
  uint32_t light_kit1_value_0_address() const { return address + 30548; }
  uint32_t light_kit1_value_1() const { return guest::LoadU32(base, address + 30552); }
  void set_light_kit1_value_1(uint32_t v) const { guest::StoreU32(base, address + 30552, v); }
  uint32_t light_kit1_value_1_address() const { return address + 30552; }
  uint32_t light_kit1_value_2() const { return guest::LoadU32(base, address + 30556); }
  void set_light_kit1_value_2(uint32_t v) const { guest::StoreU32(base, address + 30556, v); }
  uint32_t light_kit1_value_2_address() const { return address + 30556; }
  // read with light_kit1_row by the refresh
  uint32_t unknown_30560() const { return guest::LoadU32(base, address + 30560); }
  void set_unknown_30560(uint32_t v) const { guest::StoreU32(base, address + 30560, v); }
  uint32_t unknown_30560_address() const { return address + 30560; }
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
  // the vinyl page (0 or 1), or the sticker editor's area (0 to 7)
  uint32_t editor_page() const { return guest::LoadU32(base, address + 30724); }
  void set_editor_page(uint32_t v) const { guest::StoreU32(base, address + 30724, v); }
  uint32_t editor_page_address() const { return address + 30724; }
  uint32_t editor_page_wrapped() const { return guest::LoadU32(base, address + 30728); }
  void set_editor_page_wrapped(uint32_t v) const { guest::StoreU32(base, address + 30728, v); }
  uint32_t editor_page_wrapped_address() const { return address + 30728; }
  // the vinyl layer, or the sticker slot
  uint32_t layer_cursor() const { return guest::LoadU32(base, address + 30732); }
  void set_layer_cursor(uint32_t v) const { guest::StoreU32(base, address + 30732, v); }
  uint32_t layer_cursor_address() const { return address + 30732; }
  // the layer page's remembered button
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
  // the edit page's remembered row
  uint32_t edit_row() const { return guest::LoadU32(base, address + 30752); }
  void set_edit_row(uint32_t v) const { guest::StoreU32(base, address + 30752, v); }
  uint32_t edit_row_address() const { return address + 30752; }
  // the order page's position (the slot moved)
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
  // the picker's group
  uint32_t image_group() const { return guest::LoadU32(base, address + 30768); }
  void set_image_group(uint32_t v) const { guest::StoreU32(base, address + 30768, v); }
  uint32_t image_group_address() const { return address + 30768; }
  // 0 before state 35
  uint32_t unknown_30772() const { return guest::LoadU32(base, address + 30772); }
  void set_unknown_30772(uint32_t v) const { guest::StoreU32(base, address + 30772, v); }
  uint32_t unknown_30772_address() const { return address + 30772; }
  // 0 before state 36
  uint32_t unknown_30776() const { return guest::LoadU32(base, address + 30776); }
  void set_unknown_30776(uint32_t v) const { guest::StoreU32(base, address + 30776, v); }
  uint32_t unknown_30776_address() const { return address + 30776; }
  // 0 before states 37 and 52
  uint32_t unknown_30780() const { return guest::LoadU32(base, address + 30780); }
  void set_unknown_30780(uint32_t v) const { guest::StoreU32(base, address + 30780, v); }
  uint32_t unknown_30780_address() const { return address + 30780; }
  uint32_t editor_last_page() const { return guest::LoadU32(base, address + 30784); }
  void set_editor_last_page(uint32_t v) const { guest::StoreU32(base, address + 30784, v); }
  uint32_t editor_last_page_address() const { return address + 30784; }
  uint32_t editor_last_layer() const { return guest::LoadU32(base, address + 30788); }
  void set_editor_last_layer(uint32_t v) const { guest::StoreU32(base, address + 30788, v); }
  uint32_t editor_last_layer_address() const { return address + 30788; }
  // state 27's last position
  uint32_t order_last_position() const { return guest::LoadU32(base, address + 30792); }
  void set_order_last_position(uint32_t v) const { guest::StoreU32(base, address + 30792, v); }
  uint32_t order_last_position_address() const { return address + 30792; }
  // 4, 193 or 198
  uint32_t image_last_index() const { return guest::LoadU32(base, address + 30796); }
  void set_image_last_index(uint32_t v) const { guest::StoreU32(base, address + 30796, v); }
  uint32_t image_last_index_address() const { return address + 30796; }
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
  // the edited decal's x
  uint32_t edit_x() const { return guest::LoadU32(base, address + 30832); }
  void set_edit_x(uint32_t v) const { guest::StoreU32(base, address + 30832, v); }
  uint32_t edit_x_address() const { return address + 30832; }
  uint32_t edit_y() const { return guest::LoadU32(base, address + 30836); }
  void set_edit_y(uint32_t v) const { guest::StoreU32(base, address + 30836, v); }
  uint32_t edit_y_address() const { return address + 30836; }
  float edit_angle() const { return guest::LoadFloat(base, address + 30840); }
  void set_edit_angle(float v) const { guest::StoreFloat(base, address + 30840, v); }
  uint32_t edit_angle_address() const { return address + 30840; }
  float edit_scale() const { return guest::LoadFloat(base, address + 30844); }
  void set_edit_scale(float v) const { guest::StoreFloat(base, address + 30844, v); }
  uint32_t edit_scale_address() const { return address + 30844; }
  uint32_t edit_flip() const { return guest::LoadU32(base, address + 30848); }
  void set_edit_flip(uint32_t v) const { guest::StoreU32(base, address + 30848, v); }
  uint32_t edit_flip_address() const { return address + 30848; }
  // the edited decal's last word
  uint32_t unknown_30852() const { return guest::LoadU32(base, address + 30852); }
  void set_unknown_30852(uint32_t v) const { guest::StoreU32(base, address + 30852, v); }
  uint32_t unknown_30852_address() const { return address + 30852; }
  // a copy of the edited decal (restored on B)
  uint32_t edit_backup_address() const { return address + 30856; }
  // six 24-byte decal records (x, y, angle, scale, flip, a word): 3 a page
  uint32_t vinyl_layers_address() const { return address + 30880; }
  uint32_t saved_vinyl_layers_address() const { return address + 31024; }
  // +12 x page + 4j, -1 an empty layer
  uint32_t vinyl_layer_ids_address() const { return address + 31168; }
  uint32_t saved_vinyl_layer_ids_address() const { return address + 31192; }
  // state 27's work list: ids (+0) and indices (+12)
  uint32_t order_list_address() const { return address + 31216; }
  // +120 x area + 24 x slot, decal records
  uint32_t saved_stickers_address() const { return address + 31240; }
  // +20 x area + 4 x slot
  uint32_t saved_sticker_ids_address() const { return address + 32200; }
  // the placed stickers: +120 x area + 24 x slot
  uint32_t stickers_address() const { return address + 32360; }
  // +20 x area + 4 x slot
  uint32_t sticker_ids_address() const { return address + 33320; }
  // state 42's order list (slot indices)
  uint32_t sticker_order_address() const { return address + 33480; }
  // the interior item's mode (0 goes back to state 53, 1 to 56)
  uint32_t interior_mode() const { return guest::LoadU32(base, address + 33540); }
  void set_interior_mode(uint32_t v) const { guest::StoreU32(base, address + 33540, v); }
  uint32_t interior_mode_address() const { return address + 33540; }
  // the interior accessory's cursors (+33564 the pages' mode, +33572 the pair's second index, +33576 the slot)
  uint32_t interior_pages_address() const { return address + 33544; }
  // set when a paint is kept, 0 when discarded
  uint32_t paint_kept() const { return guest::LoadU32(base, address + 36824); }
  void set_paint_kept(uint32_t v) const { guest::StoreU32(base, address + 36824, v); }
  uint32_t paint_kept_address() const { return address + 36824; }
};

// embedded; measure: TuningMachine::Close.r4, TuningMachine::OnDialogClosed.r4
#pragma pack(push, 1)
struct TuningMachineModeData {
  uint8_t pad_0[204] = {};
  uint8_t messages[600] = {};  // +204, guest order
  uint32_t message_count = 0;  // +804
  uint32_t message_start = 0;  // +808
};
#pragma pack(pop)
static_assert(sizeof(TuningMachineModeData) == 812);
inline constexpr memory::LayoutField kTuningMachineModeFields[] = {{804, 4}, {808, 4}};
inline constexpr memory::Layout kTuningMachineModeLayout = {kTuningMachineModeFields, 2};

struct TuningMachineMode : View {
  using View::View;
  static constexpr uint32_t kSize = 812;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  TuningMachineModeData* data() const { return static_cast<TuningMachineModeData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static TuningMachineMode New(uint8_t* base) {
    return TuningMachineMode(base, memory::MakeHandle(new TuningMachineModeData(), sizeof(TuningMachineModeData), "TuningMachineMode", &kTuningMachineModeLayout));
  }
  template <uint32_t Stride = kSize>
  static TuningMachineMode NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(TuningMachineModeData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<TuningMachineModeData*>(::operator new(sizeof(TuningMachineModeData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) TuningMachineModeData();
    return TuningMachineMode(base, memory::MakeHandle(p, sizeof(TuningMachineModeData) * count, "TuningMachineMode", &kTuningMachineModeLayout, sizeof(TuningMachineModeData)));
  }
  TuningMachineMode at(uint32_t i) const { return TuningMachineMode(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  TuningMachineModeData* direct() const { return static_cast<TuningMachineModeData*>(memory::HandleOwn(address, &kTuningMachineModeLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kTuningMachineModeLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTuningMachineModeLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 50 messages of 12 bytes (id, two words)
  uint32_t messages_address() const { return address + 204; }
  uint8_t* messages() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 204 : guest::Pointer(base, address + 204); }
  uint32_t message_count_address() const { return address + 804; }
  uint32_t message_count_bits() const { if (auto* d = direct()) return d->message_count; return uint32_t((memory::IsHandle(address + 804) ? uint32_t(memory::OwnerLoad(address + 804, 4)) : guest::LoadU32(base, address + 804))); }
  void set_message_count_bits(uint32_t v) const { if (auto* d = direct()) { d->message_count = v; return; } if (memory::IsHandle(address + 804)) memory::OwnerStore(address + 804, 4, v); else guest::StoreU32(base, address + 804, v); }
  uint32_t message_count() const { if (auto* d = direct()) return d->message_count; return uint32_t((memory::IsHandle(address + 804) ? uint32_t(memory::OwnerLoad(address + 804, 4)) : guest::LoadU32(base, address + 804))); }
  void set_message_count(uint32_t v) const { if (auto* d = direct()) { d->message_count = v; return; } if (memory::IsHandle(address + 804)) memory::OwnerStore(address + 804, 4, v); else guest::StoreU32(base, address + 804, v); }
  // the ring's first slot
  uint32_t message_start_address() const { return address + 808; }
  uint32_t message_start_bits() const { if (auto* d = direct()) return d->message_start; return uint32_t((memory::IsHandle(address + 808) ? uint32_t(memory::OwnerLoad(address + 808, 4)) : guest::LoadU32(base, address + 808))); }
  void set_message_start_bits(uint32_t v) const { if (auto* d = direct()) { d->message_start = v; return; } if (memory::IsHandle(address + 808)) memory::OwnerStore(address + 808, 4, v); else guest::StoreU32(base, address + 808, v); }
  uint32_t message_start() const { if (auto* d = direct()) return d->message_start; return uint32_t((memory::IsHandle(address + 808) ? uint32_t(memory::OwnerLoad(address + 808, 4)) : guest::LoadU32(base, address + 808))); }
  void set_message_start(uint32_t v) const { if (auto* d = direct()) { d->message_start = v; return; } if (memory::IsHandle(address + 808)) memory::OwnerStore(address + 808, 4, v); else guest::StoreU32(base, address + 808, v); }
};

// embedded; measure: TuningMachine@+30800
struct TuningMachineScreenAt30800 : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  float unknown_8() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  float unknown_12() const { return guest::LoadFloat(base, address + 12); }
  void set_unknown_12(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
};

// The image words docs/functions/tuning-machine.md's code uses.
struct TuningMachineGlobals {
  // 0.0 (as bits)
  static uint32_t float_zero(uint8_t* base) { return guest::LoadU32(base, 0x82000A34u); }
  static void set_float_zero(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82000A34u, v); }
  static constexpr uint32_t float_zero_address() { return 0x82000A34u; }
  // 1.0 (as bits)
  static uint32_t float_one(uint8_t* base) { return guest::LoadU32(base, 0x82000A30u); }
  static void set_float_one(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82000A30u, v); }
  static constexpr uint32_t float_one_address() { return 0x82000A30u; }
  // the axles' factor in the weight (2.0 probably)
  static float axle_factor(uint8_t* base) { return guest::LoadFloat(base, 0x820021E8u); }
  static void set_axle_factor(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021E8u, v); }
  static constexpr uint32_t axle_factor_address() { return 0x820021E8u; }
  // the torque shown's scale
  static float torque_scale(uint8_t* base) { return guest::LoadFloat(base, 0x8200ADA0u); }
  static void set_torque_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200ADA0u, v); }
  static constexpr uint32_t torque_scale_address() { return 0x8200ADA0u; }
  // ScreenFades::FadeIn's pass count
  static uint32_t fade_in_passes(uint8_t* base) { return guest::LoadU32(base, 0x82001BECu); }
  static void set_fade_in_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BECu, v); }
  static constexpr uint32_t fade_in_passes_address() { return 0x82001BECu; }
};

}  // namespace schema
