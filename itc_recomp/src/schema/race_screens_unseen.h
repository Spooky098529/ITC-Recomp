// The views of docs/functions/race-screens-unseen.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/race-screens-unseen.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: Kind6Menu
struct Kind6Menu : View {
  using View::View;
  static constexpr uint32_t kSize = 360;
  // the block's +0 (ShowKind6Results leaves its frame's leftover)
  uint32_t block_0() const { return guest::LoadU32(base, address + 0); }
  void set_block_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t block_0_address() const { return address + 0; }
  // the block's +4: the result's +20, a time packed as minutes << 16 | seconds << 10 | thousandths, capped at 59:59.999 by Open
  uint32_t time() const { return guest::LoadU32(base, address + 4); }
  void set_time(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t time_address() const { return address + 4; }
  // time's high halfword (the sign bit and the minutes)
  uint16_t time_high() const { return guest::LoadU16(base, address + 4); }
  void set_time_high(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t time_high_address() const { return address + 4; }
  // the block's +16 (not written by ShowKind6Results)
  uint32_t block_16() const { return guest::LoadU32(base, address + 16); }
  void set_block_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t block_16_address() const { return address + 16; }
  // the block's +20 (not written by ShowKind6Results)
  uint32_t block_20() const { return guest::LoadU32(base, address + 20); }
  void set_block_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t block_20_address() const { return address + 20; }
  // UI movie 17, the menu
  View menu_movie() const { return View(base, guest::LoadU32(base, address + 24)); }
  void set_menu_movie(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t menu_movie_address() const { return address + 24; }
  // UI movie 20, the question box
  View question_movie() const { return View(base, guest::LoadU32(base, address + 28)); }
  void set_question_movie(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t question_movie_address() const { return address + 28; }
  // UI movie 21, the message box
  View message_movie() const { return View(base, guest::LoadU32(base, address + 32)); }
  void set_message_movie(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t message_movie_address() const { return address + 32; }
  // UI movie 14, the waiting box
  View waiting_movie() const { return View(base, guest::LoadU32(base, address + 36)); }
  void set_waiting_movie(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t waiting_movie_address() const { return address + 36; }
  // the page Update runs; -1 while a transition runs
  uint32_t pages_a_page() const { return guest::LoadU32(base, address + 40); }
  void set_pages_a_page(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t pages_a_page_address() const { return address + 40; }
  uint32_t pages_a_next_page() const { return guest::LoadU32(base, address + 44); }
  void set_pages_a_next_page(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t pages_a_next_page_address() const { return address + 44; }
  uint32_t pages_a_previous_page() const { return guest::LoadU32(base, address + 48); }
  void set_pages_a_previous_page(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t pages_a_previous_page_address() const { return address + 48; }
  // the transition Draw runs; -1 none
  uint32_t pages_a_step() const { return guest::LoadU32(base, address + 52); }
  void set_pages_a_step(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t pages_a_step_address() const { return address + 52; }
  uint32_t pages_a_queued_step() const { return guest::LoadU32(base, address + 56); }
  void set_pages_a_queued_step(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t pages_a_queued_step_address() const { return address + 56; }
  uint32_t pages_a_unknown_20() const { return guest::LoadU32(base, address + 60); }
  void set_pages_a_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t pages_a_unknown_20_address() const { return address + 60; }
  // the transition's step word
  uint32_t pages_a_step_started() const { return guest::LoadU32(base, address + 64); }
  void set_pages_a_step_started(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t pages_a_step_started_address() const { return address + 64; }
  uint32_t main_menu_cursor() const { return guest::LoadU32(base, address + 68); }
  void set_main_menu_cursor(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t main_menu_cursor_address() const { return address + 68; }
  uint32_t main_menu_previous() const { return guest::LoadU32(base, address + 72); }
  void set_main_menu_previous(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t main_menu_previous_address() const { return address + 72; }
  uint32_t main_menu_moved() const { return guest::LoadU32(base, address + 76); }
  void set_main_menu_moved(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t main_menu_moved_address() const { return address + 76; }
  uint32_t main_menu_first_item() const { return guest::LoadU32(base, address + 80); }
  void set_main_menu_first_item(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t main_menu_first_item_address() const { return address + 80; }
  uint32_t main_menu_last_item() const { return guest::LoadU32(base, address + 84); }
  void set_main_menu_last_item(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t main_menu_last_item_address() const { return address + 84; }
  uint32_t main_menu_wraps() const { return guest::LoadU32(base, address + 88); }
  void set_main_menu_wraps(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t main_menu_wraps_address() const { return address + 88; }
  uint32_t main_menu_up_down_locked() const { return guest::LoadU32(base, address + 92); }
  void set_main_menu_up_down_locked(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t main_menu_up_down_locked_address() const { return address + 92; }
  uint32_t main_menu_left_right_steps() const { return guest::LoadU32(base, address + 96); }
  void set_main_menu_left_right_steps(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t main_menu_left_right_steps_address() const { return address + 96; }
  uint32_t main_menu_shoulders_step() const { return guest::LoadU32(base, address + 100); }
  void set_main_menu_shoulders_step(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t main_menu_shoulders_step_address() const { return address + 100; }
  int32_t main_menu_side() const { return int32_t(guest::LoadU32(base, address + 104)); }
  void set_main_menu_side(int32_t v) const { guest::StoreU32(base, address + 104, uint32_t(v)); }
  uint32_t main_menu_side_address() const { return address + 104; }
  uint32_t main_menu_wrapped() const { return guest::LoadU32(base, address + 108); }
  void set_main_menu_wrapped(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t main_menu_wrapped_address() const { return address + 108; }
  uint32_t main_menu_list_wrapped() const { return guest::LoadU32(base, address + 112); }
  void set_main_menu_list_wrapped(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t main_menu_list_wrapped_address() const { return address + 112; }
  uint32_t main_menu_visible_row() const { return guest::LoadU32(base, address + 116); }
  void set_main_menu_visible_row(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t main_menu_visible_row_address() const { return address + 116; }
  uint32_t main_menu_previous_row() const { return guest::LoadU32(base, address + 120); }
  void set_main_menu_previous_row(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t main_menu_previous_row_address() const { return address + 120; }
  uint32_t main_menu_first_row() const { return guest::LoadU32(base, address + 124); }
  void set_main_menu_first_row(uint32_t v) const { guest::StoreU32(base, address + 124, v); }
  uint32_t main_menu_first_row_address() const { return address + 124; }
  uint32_t main_menu_rows_shown() const { return guest::LoadU32(base, address + 128); }
  void set_main_menu_rows_shown(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t main_menu_rows_shown_address() const { return address + 128; }
  uint32_t main_menu_player() const { return guest::LoadU32(base, address + 132); }
  void set_main_menu_player(uint32_t v) const { guest::StoreU32(base, address + 132, v); }
  uint32_t main_menu_player_address() const { return address + 132; }
  uint32_t slot_list_count() const { return guest::LoadU32(base, address + 136); }
  void set_slot_list_count(uint32_t v) const { guest::StoreU32(base, address + 136, v); }
  uint32_t slot_list_count_address() const { return address + 136; }
  uint32_t slot_list_scroll() const { return guest::LoadU32(base, address + 140); }
  void set_slot_list_scroll(uint32_t v) const { guest::StoreU32(base, address + 140, v); }
  uint32_t slot_list_scroll_address() const { return address + 140; }
  uint32_t slot_list_scroll_before() const { return guest::LoadU32(base, address + 144); }
  void set_slot_list_scroll_before(uint32_t v) const { guest::StoreU32(base, address + 144, v); }
  uint32_t slot_list_scroll_before_address() const { return address + 144; }
  uint32_t slot_list_shown() const { return guest::LoadU32(base, address + 148); }
  void set_slot_list_shown(uint32_t v) const { guest::StoreU32(base, address + 148, v); }
  uint32_t slot_list_shown_address() const { return address + 148; }
  uint32_t slot_list_selection() const { return guest::LoadU32(base, address + 152); }
  void set_slot_list_selection(uint32_t v) const { guest::StoreU32(base, address + 152, v); }
  uint32_t slot_list_selection_address() const { return address + 152; }
  uint32_t slot_list_selection_before() const { return guest::LoadU32(base, address + 156); }
  void set_slot_list_selection_before(uint32_t v) const { guest::StoreU32(base, address + 156, v); }
  uint32_t slot_list_selection_before_address() const { return address + 156; }
  uint32_t slot_list_move() const { return guest::LoadU32(base, address + 160); }
  void set_slot_list_move(uint32_t v) const { guest::StoreU32(base, address + 160, v); }
  uint32_t slot_list_move_address() const { return address + 160; }
  uint32_t slot_list_side() const { return guest::LoadU32(base, address + 164); }
  void set_slot_list_side(uint32_t v) const { guest::StoreU32(base, address + 164, v); }
  uint32_t slot_list_side_address() const { return address + 164; }
  uint32_t slot_list_wrapped() const { return guest::LoadU32(base, address + 168); }
  void set_slot_list_wrapped(uint32_t v) const { guest::StoreU32(base, address + 168, v); }
  uint32_t slot_list_wrapped_address() const { return address + 168; }
  uint32_t slot_list_down() const { return guest::LoadU32(base, address + 172); }
  void set_slot_list_down(uint32_t v) const { guest::StoreU32(base, address + 172, v); }
  uint32_t slot_list_down_address() const { return address + 172; }
  uint32_t slot_list_up() const { return guest::LoadU32(base, address + 176); }
  void set_slot_list_up(uint32_t v) const { guest::StoreU32(base, address + 176, v); }
  uint32_t slot_list_up_address() const { return address + 176; }
  uint32_t slot_list_left() const { return guest::LoadU32(base, address + 180); }
  void set_slot_list_left(uint32_t v) const { guest::StoreU32(base, address + 180, v); }
  uint32_t slot_list_left_address() const { return address + 180; }
  uint32_t slot_list_right() const { return guest::LoadU32(base, address + 184); }
  void set_slot_list_right(uint32_t v) const { guest::StoreU32(base, address + 184, v); }
  uint32_t slot_list_right_address() const { return address + 184; }
  uint32_t slot_list_accept() const { return guest::LoadU32(base, address + 188); }
  void set_slot_list_accept(uint32_t v) const { guest::StoreU32(base, address + 188, v); }
  uint32_t slot_list_accept_address() const { return address + 188; }
  uint32_t slot_list_back() const { return guest::LoadU32(base, address + 192); }
  void set_slot_list_back(uint32_t v) const { guest::StoreU32(base, address + 192, v); }
  uint32_t slot_list_back_address() const { return address + 192; }
  uint32_t slot_list_player() const { return guest::LoadU32(base, address + 196); }
  void set_slot_list_player(uint32_t v) const { guest::StoreU32(base, address + 196, v); }
  uint32_t slot_list_player_address() const { return address + 196; }
  uint32_t slot_list_wraps() const { return guest::LoadU32(base, address + 200); }
  void set_slot_list_wraps(uint32_t v) const { guest::StoreU32(base, address + 200, v); }
  uint32_t slot_list_wraps_address() const { return address + 200; }
  uint32_t slot_list_block() const { return guest::LoadU32(base, address + 204); }
  void set_slot_list_block(uint32_t v) const { guest::StoreU32(base, address + 204, v); }
  uint32_t slot_list_block_address() const { return address + 204; }
  // the page Update runs; -1 while a transition runs
  uint32_t pages_b_page() const { return guest::LoadU32(base, address + 208); }
  void set_pages_b_page(uint32_t v) const { guest::StoreU32(base, address + 208, v); }
  uint32_t pages_b_page_address() const { return address + 208; }
  uint32_t pages_b_next_page() const { return guest::LoadU32(base, address + 212); }
  void set_pages_b_next_page(uint32_t v) const { guest::StoreU32(base, address + 212, v); }
  uint32_t pages_b_next_page_address() const { return address + 212; }
  uint32_t pages_b_previous_page() const { return guest::LoadU32(base, address + 216); }
  void set_pages_b_previous_page(uint32_t v) const { guest::StoreU32(base, address + 216, v); }
  uint32_t pages_b_previous_page_address() const { return address + 216; }
  // the transition Draw runs; -1 none
  uint32_t pages_b_step() const { return guest::LoadU32(base, address + 220); }
  void set_pages_b_step(uint32_t v) const { guest::StoreU32(base, address + 220, v); }
  uint32_t pages_b_step_address() const { return address + 220; }
  uint32_t pages_b_queued_step() const { return guest::LoadU32(base, address + 224); }
  void set_pages_b_queued_step(uint32_t v) const { guest::StoreU32(base, address + 224, v); }
  uint32_t pages_b_queued_step_address() const { return address + 224; }
  uint32_t pages_b_unknown_20() const { return guest::LoadU32(base, address + 228); }
  void set_pages_b_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 228, v); }
  uint32_t pages_b_unknown_20_address() const { return address + 228; }
  // the transition's step word
  uint32_t pages_b_step_started() const { return guest::LoadU32(base, address + 232); }
  void set_pages_b_step_started(uint32_t v) const { guest::StoreU32(base, address + 232, v); }
  uint32_t pages_b_step_started_address() const { return address + 232; }
  uint32_t question_menu_cursor() const { return guest::LoadU32(base, address + 236); }
  void set_question_menu_cursor(uint32_t v) const { guest::StoreU32(base, address + 236, v); }
  uint32_t question_menu_cursor_address() const { return address + 236; }
  uint32_t question_menu_previous() const { return guest::LoadU32(base, address + 240); }
  void set_question_menu_previous(uint32_t v) const { guest::StoreU32(base, address + 240, v); }
  uint32_t question_menu_previous_address() const { return address + 240; }
  uint32_t question_menu_moved() const { return guest::LoadU32(base, address + 244); }
  void set_question_menu_moved(uint32_t v) const { guest::StoreU32(base, address + 244, v); }
  uint32_t question_menu_moved_address() const { return address + 244; }
  uint32_t question_menu_first_item() const { return guest::LoadU32(base, address + 248); }
  void set_question_menu_first_item(uint32_t v) const { guest::StoreU32(base, address + 248, v); }
  uint32_t question_menu_first_item_address() const { return address + 248; }
  uint32_t question_menu_last_item() const { return guest::LoadU32(base, address + 252); }
  void set_question_menu_last_item(uint32_t v) const { guest::StoreU32(base, address + 252, v); }
  uint32_t question_menu_last_item_address() const { return address + 252; }
  uint32_t question_menu_wraps() const { return guest::LoadU32(base, address + 256); }
  void set_question_menu_wraps(uint32_t v) const { guest::StoreU32(base, address + 256, v); }
  uint32_t question_menu_wraps_address() const { return address + 256; }
  uint32_t question_menu_up_down_locked() const { return guest::LoadU32(base, address + 260); }
  void set_question_menu_up_down_locked(uint32_t v) const { guest::StoreU32(base, address + 260, v); }
  uint32_t question_menu_up_down_locked_address() const { return address + 260; }
  uint32_t question_menu_left_right_steps() const { return guest::LoadU32(base, address + 264); }
  void set_question_menu_left_right_steps(uint32_t v) const { guest::StoreU32(base, address + 264, v); }
  uint32_t question_menu_left_right_steps_address() const { return address + 264; }
  uint32_t question_menu_shoulders_step() const { return guest::LoadU32(base, address + 268); }
  void set_question_menu_shoulders_step(uint32_t v) const { guest::StoreU32(base, address + 268, v); }
  uint32_t question_menu_shoulders_step_address() const { return address + 268; }
  int32_t question_menu_side() const { return int32_t(guest::LoadU32(base, address + 272)); }
  void set_question_menu_side(int32_t v) const { guest::StoreU32(base, address + 272, uint32_t(v)); }
  uint32_t question_menu_side_address() const { return address + 272; }
  uint32_t question_menu_wrapped() const { return guest::LoadU32(base, address + 276); }
  void set_question_menu_wrapped(uint32_t v) const { guest::StoreU32(base, address + 276, v); }
  uint32_t question_menu_wrapped_address() const { return address + 276; }
  uint32_t question_menu_list_wrapped() const { return guest::LoadU32(base, address + 280); }
  void set_question_menu_list_wrapped(uint32_t v) const { guest::StoreU32(base, address + 280, v); }
  uint32_t question_menu_list_wrapped_address() const { return address + 280; }
  uint32_t question_menu_visible_row() const { return guest::LoadU32(base, address + 284); }
  void set_question_menu_visible_row(uint32_t v) const { guest::StoreU32(base, address + 284, v); }
  uint32_t question_menu_visible_row_address() const { return address + 284; }
  uint32_t question_menu_previous_row() const { return guest::LoadU32(base, address + 288); }
  void set_question_menu_previous_row(uint32_t v) const { guest::StoreU32(base, address + 288, v); }
  uint32_t question_menu_previous_row_address() const { return address + 288; }
  uint32_t question_menu_first_row() const { return guest::LoadU32(base, address + 292); }
  void set_question_menu_first_row(uint32_t v) const { guest::StoreU32(base, address + 292, v); }
  uint32_t question_menu_first_row_address() const { return address + 292; }
  uint32_t question_menu_rows_shown() const { return guest::LoadU32(base, address + 296); }
  void set_question_menu_rows_shown(uint32_t v) const { guest::StoreU32(base, address + 296, v); }
  uint32_t question_menu_rows_shown_address() const { return address + 296; }
  uint32_t question_menu_player() const { return guest::LoadU32(base, address + 300); }
  void set_question_menu_player(uint32_t v) const { guest::StoreU32(base, address + 300, v); }
  uint32_t question_menu_player_address() const { return address + 300; }
  // cleared by Open
  uint32_t unknown_304() const { return guest::LoadU32(base, address + 304); }
  void set_unknown_304(uint32_t v) const { guest::StoreU32(base, address + 304, v); }
  uint32_t unknown_304_address() const { return address + 304; }
  // the player's rank (the network manager's sub_822914A8)
  uint32_t rank() const { return guest::LoadU32(base, address + 308); }
  void set_rank(uint32_t v) const { guest::StoreU32(base, address + 308, v); }
  uint32_t rank_address() const { return address + 308; }
  // the leaderboard's rows read
  uint32_t rows_read() const { return guest::LoadU32(base, address + 312); }
  void set_rows_read(uint32_t v) const { guest::StoreU32(base, address + 312, v); }
  uint32_t rows_read_address() const { return address + 312; }
  // set: the time attack's rows or record shown
  uint32_t rows_shown() const { return guest::LoadU32(base, address + 316); }
  void set_rows_shown(uint32_t v) const { guest::StoreU32(base, address + 316, v); }
  uint32_t rows_shown_address() const { return address + 316; }
  // a leaderboard read started
  uint32_t read_started() const { return guest::LoadU32(base, address + 320); }
  void set_read_started(uint32_t v) const { guest::StoreU32(base, address + 320, v); }
  uint32_t read_started_address() const { return address + 320; }
  // the box's message, a string id: 44, 48 to 52, 57
  uint32_t box_message() const { return guest::LoadU32(base, address + 324); }
  void set_box_message(uint32_t v) const { guest::StoreU32(base, address + 324, v); }
  uint32_t box_message_address() const { return address + 324; }
  // which message (0 to 3)
  uint32_t message_which() const { return guest::LoadU32(base, address + 328); }
  void set_message_which(uint32_t v) const { guest::StoreU32(base, address + 328, v); }
  uint32_t message_which_address() const { return address + 328; }
  // which question (1 or 2)
  uint32_t question_which() const { return guest::LoadU32(base, address + 332); }
  void set_question_which(uint32_t v) const { guest::StoreU32(base, address + 332, v); }
  uint32_t question_which_address() const { return address + 332; }
  // the menu's movie run by Draw
  uint32_t menu_movie_running() const { return guest::LoadU32(base, address + 336); }
  void set_menu_movie_running(uint32_t v) const { guest::StoreU32(base, address + 336, v); }
  uint32_t menu_movie_running_address() const { return address + 336; }
  // the menu's input on (0 while a box opens or shows)
  uint32_t input_on() const { return guest::LoadU32(base, address + 340); }
  void set_input_on(uint32_t v) const { guest::StoreU32(base, address + 340, v); }
  uint32_t input_on_address() const { return address + 340; }
  // a request from sub_82211E90
  uint32_t upload_request() const { return guest::LoadU32(base, address + 344); }
  void set_upload_request(uint32_t v) const { guest::StoreU32(base, address + 344, v); }
  uint32_t upload_request_address() const { return address + 344; }
  // 0 the question, 1 the message, 2 the waiting box, 3 none
  uint32_t box_shown() const { return guest::LoadU32(base, address + 348); }
  void set_box_shown(uint32_t v) const { guest::StoreU32(base, address + 348, v); }
  uint32_t box_shown_address() const { return address + 348; }
  // a request from sub_82211F18 (to page 7 with message 44)
  uint32_t reset_request() const { return guest::LoadU32(base, address + 352); }
  void set_reset_request(uint32_t v) const { guest::StoreU32(base, address + 352, v); }
  uint32_t reset_request_address() const { return address + 352; }
  uint32_t ranking_skipped() const { return guest::LoadU32(base, address + 356); }
  void set_ranking_skipped(uint32_t v) const { guest::StoreU32(base, address + 356, v); }
  uint32_t ranking_skipped_address() const { return address + 356; }
  // measure fault: Kind6Menu::ShowReplay's r3 is a replay record (as AfterRaceMenu::ShowReplay's, race-screens.toml), joined with the menu
  uint16_t fault_replay_0() const { return guest::LoadU16(base, address + 0); }
  void set_fault_replay_0(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t fault_replay_0_address() const { return address + 0; }
  // measure fault: the replay record's +2 (ShowReplay)
  uint16_t fault_replay_2() const { return guest::LoadU16(base, address + 2); }
  void set_fault_replay_2(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t fault_replay_2_address() const { return address + 2; }
  // measure fault: the replay record's +6 (ShowReplay)
  uint16_t fault_replay_6() const { return guest::LoadU16(base, address + 6); }
  void set_fault_replay_6(uint16_t v) const { guest::StoreU16(base, address + 6, v); }
  uint32_t fault_replay_6_address() const { return address + 6; }
  // measure fault: the replay record's +8 (ShowReplay)
  uint16_t fault_replay_8() const { return guest::LoadU16(base, address + 8); }
  void set_fault_replay_8(uint16_t v) const { guest::StoreU16(base, address + 8, v); }
  uint32_t fault_replay_8_address() const { return address + 8; }
  // measure fault: the replay record's +10 (ShowReplay)
  uint16_t fault_replay_10() const { return guest::LoadU16(base, address + 10); }
  void set_fault_replay_10(uint16_t v) const { guest::StoreU16(base, address + 10, v); }
  uint32_t fault_replay_10_address() const { return address + 10; }
};

// allocated; measure: PauseMenu4
struct PauseMenu4 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // UI movie 5
  View movie() const { return View(base, guest::LoadU32(base, address + 0)); }
  void set_movie(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t movie_address() const { return address + 0; }
};

// allocated; measure: new<80>@PauseMenu4::Create
#pragma pack(push, 1)
struct PauseMenu4DataData {
  uint32_t music_volume = 0;  // +0
  uint32_t effects_volume = 0;  // +4
  uint32_t music_edit = 0;  // +8
  uint32_t effects_edit = 0;  // +12
  uint32_t music_default = 0;  // +16
  uint32_t effects_default = 0;  // +20
  uint8_t pad_24[28] = {};
  uint32_t item_count = 0;  // +52
  uint32_t answer = 0;  // +56
  uint32_t volume_toggle = 0;  // +60
  uint32_t main_selection = 0;  // +64
  uint32_t run_count = 0;  // +68
  uint32_t player = 0;  // +72
  uint32_t kind = 0;  // +76
};
#pragma pack(pop)
static_assert(sizeof(PauseMenu4DataData) == 80);
inline constexpr memory::LayoutField kPauseMenu4DataFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}, {68, 4}, {72, 4}, {76, 4}};
inline constexpr memory::Layout kPauseMenu4DataLayout = {kPauseMenu4DataFields, 13};

struct PauseMenu4Data : View {
  using View::View;
  static constexpr uint32_t kSize = 80;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  PauseMenu4DataData* data() const { return static_cast<PauseMenu4DataData*>(memory::HandleObject(address)); }
  static PauseMenu4Data New(uint8_t* base) {
    return PauseMenu4Data(base, memory::MakeHandle(new PauseMenu4DataData(), sizeof(PauseMenu4DataData), "PauseMenu4Data", &kPauseMenu4DataLayout));
  }
  void Delete() const { delete data(); memory::ReleaseHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kPauseMenu4DataLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kPauseMenu4DataLayout, offset, width, v); }
  // player 0's save data's byte +0x1F878, sign-extended
  uint32_t music_volume() const { return data()->music_volume; }
  void set_music_volume(uint32_t v) const { data()->music_volume = v; }
  uint32_t music_volume_bits() const { return data()->music_volume; }
  void set_music_volume_bits(uint32_t v) const { data()->music_volume = v; }
  // player 0's save data's byte +0x1F879, sign-extended
  uint32_t effects_volume() const { return data()->effects_volume; }
  void set_effects_volume(uint32_t v) const { data()->effects_volume = v; }
  uint32_t effects_volume_bits() const { return data()->effects_volume; }
  void set_effects_volume_bits(uint32_t v) const { data()->effects_volume = v; }
  // the volume page's edit, 0 to 15
  uint32_t music_edit() const { return data()->music_edit; }
  void set_music_edit(uint32_t v) const { data()->music_edit = v; }
  uint32_t music_edit_bits() const { return data()->music_edit; }
  void set_music_edit_bits(uint32_t v) const { data()->music_edit = v; }
  // the volume page's edit, 0 to 15
  uint32_t effects_edit() const { return data()->effects_edit; }
  void set_effects_edit(uint32_t v) const { data()->effects_edit = v; }
  uint32_t effects_edit_bits() const { return data()->effects_edit; }
  void set_effects_edit_bits(uint32_t v) const { data()->effects_edit = v; }
  // 15
  uint32_t music_default() const { return data()->music_default; }
  void set_music_default(uint32_t v) const { data()->music_default = v; }
  uint32_t music_default_bits() const { return data()->music_default; }
  void set_music_default_bits(uint32_t v) const { data()->music_default = v; }
  // 15
  uint32_t effects_default() const { return data()->effects_default; }
  void set_effects_default(uint32_t v) const { data()->effects_default = v; }
  uint32_t effects_default_bits() const { return data()->effects_default; }
  void set_effects_default_bits(uint32_t v) const { data()->effects_default = v; }
  uint32_t item_count() const { return data()->item_count; }
  void set_item_count(uint32_t v) const { data()->item_count = v; }
  uint32_t item_count_bits() const { return data()->item_count; }
  void set_item_count_bits(uint32_t v) const { data()->item_count = v; }
  // what Update answers on the answer page
  uint32_t answer() const { return data()->answer; }
  void set_answer(uint32_t v) const { data()->answer = v; }
  uint32_t answer_bits() const { return data()->answer; }
  void set_answer_bits(uint32_t v) const { data()->answer = v; }
  // the volume page's last item's choice, 0 or 1
  uint32_t volume_toggle() const { return data()->volume_toggle; }
  void set_volume_toggle(uint32_t v) const { data()->volume_toggle = v; }
  uint32_t volume_toggle_bits() const { return data()->volume_toggle; }
  void set_volume_toggle_bits(uint32_t v) const { data()->volume_toggle = v; }
  // the main list's selection kept
  uint32_t main_selection() const { return data()->main_selection; }
  void set_main_selection(uint32_t v) const { data()->main_selection = v; }
  uint32_t main_selection_bits() const { return data()->main_selection; }
  void set_main_selection_bits(uint32_t v) const { data()->main_selection = v; }
  // RunTen's counter, done at 10
  uint32_t run_count() const { return data()->run_count; }
  void set_run_count(uint32_t v) const { data()->run_count = v; }
  uint32_t run_count_bits() const { return data()->run_count; }
  void set_run_count_bits(uint32_t v) const { data()->run_count = v; }
  // the player who paused, 0 or 1
  uint32_t player() const { return data()->player; }
  void set_player(uint32_t v) const { data()->player = v; }
  uint32_t player_bits() const { return data()->player; }
  void set_player_bits(uint32_t v) const { data()->player = v; }
  // the items' kind: 0 or 1 (the replay bit)
  uint32_t kind() const { return data()->kind; }
  void set_kind(uint32_t v) const { data()->kind = v; }
  uint32_t kind_bits() const { return data()->kind; }
  void set_kind_bits(uint32_t v) const { data()->kind = v; }
};

// allocated; measure: RaceResults
struct RaceResults : View {
  using View::View;
  static constexpr uint32_t kSize = 52;
  // the block's +0: 1 from ShowResults; Intro and Show pick their animations by it
  uint32_t shown_flag() const { return guest::LoadU32(base, address + 0); }
  void set_shown_flag(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t shown_flag_address() const { return address + 0; }
  // the result's +20, a time packed as minutes << 16 | seconds << 10 | thousandths
  uint32_t time() const { return guest::LoadU32(base, address + 4); }
  void set_time(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t time_address() const { return address + 4; }
  // the result's +3892: item 7's string from a table (395 to 411) by it
  uint32_t place() const { return guest::LoadU32(base, address + 8); }
  void set_place(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t place_address() const { return address + 8; }
  // the race data's +60: item 8's string 1718 + it
  uint32_t race_word_60() const { return guest::LoadU32(base, address + 12); }
  void set_race_word_60(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t race_word_60_address() const { return address + 12; }
  // [0x826D82E8]'s +2196: item 9's string by it (0 to 10)
  uint32_t players_word_2196() const { return guest::LoadU32(base, address + 16); }
  void set_players_word_2196(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t players_word_2196_address() const { return address + 16; }
  // UI movie 7
  View movie() const { return View(base, guest::LoadU32(base, address + 20)); }
  void set_movie(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t movie_address() const { return address + 20; }
  // the page Update runs; -1 while a transition runs
  uint32_t pages_page() const { return guest::LoadU32(base, address + 24); }
  void set_pages_page(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t pages_page_address() const { return address + 24; }
  uint32_t pages_next_page() const { return guest::LoadU32(base, address + 28); }
  void set_pages_next_page(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t pages_next_page_address() const { return address + 28; }
  uint32_t pages_previous_page() const { return guest::LoadU32(base, address + 32); }
  void set_pages_previous_page(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t pages_previous_page_address() const { return address + 32; }
  // the transition Draw runs; -1 none
  uint32_t pages_step() const { return guest::LoadU32(base, address + 36); }
  void set_pages_step(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t pages_step_address() const { return address + 36; }
  uint32_t pages_queued_step() const { return guest::LoadU32(base, address + 40); }
  void set_pages_queued_step(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t pages_queued_step_address() const { return address + 40; }
  uint32_t pages_unknown_20() const { return guest::LoadU32(base, address + 44); }
  void set_pages_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t pages_unknown_20_address() const { return address + 44; }
  // the transition's step word
  uint32_t pages_step_started() const { return guest::LoadU32(base, address + 48); }
  void set_pages_step_started(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t pages_step_started_address() const { return address + 48; }
};

// allocated; measure: SplitResults
struct SplitResults : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  // 0 to 2
  uint32_t outcome() const { return guest::LoadU32(base, address + 0); }
  void set_outcome(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t outcome_address() const { return address + 0; }
  // 0 to 2
  uint32_t mode() const { return guest::LoadU32(base, address + 4); }
  void set_mode(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t mode_address() const { return address + 4; }
  // UI movie 8
  View movie() const { return View(base, guest::LoadU32(base, address + 8)); }
  void set_movie(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t movie_address() const { return address + 8; }
  // the page Update runs; -1 while a transition runs
  uint32_t pages_page() const { return guest::LoadU32(base, address + 12); }
  void set_pages_page(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t pages_page_address() const { return address + 12; }
  uint32_t pages_next_page() const { return guest::LoadU32(base, address + 16); }
  void set_pages_next_page(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t pages_next_page_address() const { return address + 16; }
  uint32_t pages_previous_page() const { return guest::LoadU32(base, address + 20); }
  void set_pages_previous_page(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t pages_previous_page_address() const { return address + 20; }
  // the transition Draw runs; -1 none
  uint32_t pages_step() const { return guest::LoadU32(base, address + 24); }
  void set_pages_step(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t pages_step_address() const { return address + 24; }
  uint32_t pages_queued_step() const { return guest::LoadU32(base, address + 28); }
  void set_pages_queued_step(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t pages_queued_step_address() const { return address + 28; }
  uint32_t pages_unknown_20() const { return guest::LoadU32(base, address + 32); }
  void set_pages_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t pages_unknown_20_address() const { return address + 32; }
  // the transition's step word
  uint32_t pages_step_started() const { return guest::LoadU32(base, address + 36); }
  void set_pages_step_started(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t pages_step_started_address() const { return address + 36; }
};

// image; measure: -
struct Kind6StatsSpec : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t view_id() const { return guest::LoadU32(base, address + 0); }
  void set_view_id(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t view_id_address() const { return address + 0; }
  uint32_t column_count() const { return guest::LoadU32(base, address + 4); }
  void set_column_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t column_count_address() const { return address + 4; }
  uint16_t column_0() const { return guest::LoadU16(base, address + 8); }
  void set_column_0(uint16_t v) const { guest::StoreU16(base, address + 8, v); }
  uint32_t column_0_address() const { return address + 8; }
  uint16_t column_1() const { return guest::LoadU16(base, address + 10); }
  void set_column_1(uint16_t v) const { guest::StoreU16(base, address + 10, v); }
  uint32_t column_1_address() const { return address + 10; }
  uint16_t column_2() const { return guest::LoadU16(base, address + 12); }
  void set_column_2(uint16_t v) const { guest::StoreU16(base, address + 12, v); }
  uint32_t column_2_address() const { return address + 12; }
};

// embedded; measure: new<360>@Kind6Menu::Create
struct Kind6MenuPart : View {
  using View::View;
  static constexpr uint32_t kSize = 344;
  uint32_t unknown_340() const { return guest::LoadU32(base, address + 340); }
  void set_unknown_340(uint32_t v) const { guest::StoreU32(base, address + 340, v); }
  uint32_t unknown_340_address() const { return address + 340; }
};

// The image words docs/functions/race-screens-unseen.md's code uses.
struct RaceScreensUnseenGlobals {
  // kind 4's pause menu (the holder's +1016)
  template <typename T = PauseMenu4>
  static T pause4_screen(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D84E0u)); }
  static void set_pause4_screen(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D84E0u, v.address); }
  static void set_pause4_screen(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84E0u, v); }
  static constexpr uint32_t pause4_screen_address() { return 0x826D84E0u; }
  template <typename T = PauseMenu4Data>
  static T pause4_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D84E4u)); }
  static void set_pause4_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D84E4u, v.address); }
  static void set_pause4_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84E4u, v); }
  static constexpr uint32_t pause4_data_address() { return 0x826D84E4u; }
  template <typename T = PageObject>
  static T pause4_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D84E8u)); }
  static void set_pause4_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D84E8u, v.address); }
  static void set_pause4_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84E8u, v); }
  static constexpr uint32_t pause4_pages_address() { return 0x826D84E8u; }
  // a MenuList (72 bytes, race-screens.toml)
  template <typename T = MenuList>
  static T pause4_list(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D84ECu)); }
  static void set_pause4_list(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D84ECu, v.address); }
  static void set_pause4_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84ECu, v); }
  static constexpr uint32_t pause4_list_address() { return 0x826D84ECu; }
  // the items by kind: six words a kind, -1 ends them
  static constexpr uint32_t pause4_items_address() { return 0x8200CD08u; }
  template <typename T = Kind6Menu>
  static T kind6_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82814E10u)); }
  static void set_kind6_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82814E10u, v.address); }
  static void set_kind6_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E10u, v); }
  static constexpr uint32_t kind6_menu_address() { return 0x82814E10u; }
  // Kind6Menu's answer, 1 to 3 (0 to 2 also open it with its movies' poses reset)
  static uint32_t kind6_answer(uint8_t* base) { return guest::LoadU32(base, 0x82814E14u); }
  static void set_kind6_answer(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E14u, v); }
  static constexpr uint32_t kind6_answer_address() { return 0x82814E14u; }
  template <typename T = RaceResults>
  static T race_results(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82814E18u)); }
  static void set_race_results(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82814E18u, v.address); }
  static void set_race_results(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E18u, v); }
  static constexpr uint32_t race_results_address() { return 0x82814E18u; }
  template <typename T = SplitResults>
  static T split_results(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82814E1Cu)); }
  static void set_split_results(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82814E1Cu, v.address); }
  static void set_split_results(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E1Cu, v); }
  static constexpr uint32_t split_results_address() { return 0x82814E1Cu; }
  // SplitResults' tick count
  static uint32_t split_ticks(uint8_t* base) { return guest::LoadU32(base, 0x82814E20u); }
  static void set_split_ticks(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E20u, v); }
  static constexpr uint32_t split_ticks_address() { return 0x82814E20u; }
  // set once the other console's result came
  static uint32_t split_exchanged(uint8_t* base) { return guest::LoadU32(base, 0x82814E24u); }
  static void set_split_exchanged(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E24u, v); }
  static constexpr uint32_t split_exchanged_address() { return 0x82814E24u; }
  // the network session (network-mode.toml's Session)
  template <typename T = Session>
  static T kind6_session(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82816E3Cu)); }
  static void set_kind6_session(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82816E3Cu, v.address); }
  static void set_kind6_session(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816E3Cu, v); }
  static constexpr uint32_t kind6_session_address() { return 0x82816E3Cu; }
  // the course, 0 to 7
  static uint32_t kind6_course(uint8_t* base) { return guest::LoadU32(base, 0x82816660u); }
  static void set_kind6_course(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816660u, v); }
  static constexpr uint32_t kind6_course_address() { return 0x82816660u; }
  static uint32_t kind6_course_flag(uint8_t* base) { return guest::LoadU32(base, 0x82816680u); }
  static void set_kind6_course_flag(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816680u, v); }
  static constexpr uint32_t kind6_course_flag_address() { return 0x82816680u; }
  // the replay list (save-interface.toml's SaveBuffers: +12 the 36-byte records, +20 their count)
  template <typename T = SaveBuffers>
  static T kind6_replays(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828161F0u)); }
  static void set_kind6_replays(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828161F0u, v.address); }
  static void set_kind6_replays(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161F0u, v); }
  static constexpr uint32_t kind6_replays_address() { return 0x828161F0u; }
  // the leaderboard read's results (an XUSER_STATS_READ_RESULTS)
  static View kind6_results(uint8_t* base) { return View(base, guest::LoadU32(base, 0x826C6AD4u)); }
  static void set_kind6_results(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C6AD4u, v); }
  static constexpr uint32_t kind6_results_address() { return 0x826C6AD4u; }
  // the leaderboard read's view id
  static uint32_t kind6_read_spec(uint8_t* base) { return guest::LoadU32(base, 0x826C72F4u); }
  static void set_kind6_read_spec(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C72F4u, v); }
  static constexpr uint32_t kind6_read_spec_address() { return 0x826C72F4u; }
  // a Kind6StatsSpec
  static constexpr uint32_t kind6_stats_spec_address() { return 0x826C6410u; }
  static uint32_t sign_in_watch_state(uint8_t* base) { return guest::LoadU32(base, 0x826BAB3Cu); }
  static void set_sign_in_watch_state(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BAB3Cu, v); }
  static constexpr uint32_t sign_in_watch_state_address() { return 0x826BAB3Cu; }
  static uint32_t sign_in_watch_reason(uint8_t* base) { return guest::LoadU32(base, 0x826BAB5Cu); }
  static void set_sign_in_watch_reason(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BAB5Cu, v); }
  static constexpr uint32_t sign_in_watch_reason_address() { return 0x826BAB5Cu; }
  // the network manager's peer list (+4 begin, +8 end, 384-byte entries)
  static constexpr uint32_t net_peers_address() { return 0x826C7308u; }
  // set: the peer list used
  static uint32_t net_peer_mode(uint8_t* base) { return guest::LoadU32(base, 0x826C7824u); }
  static void set_net_peer_mode(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7824u, v); }
  static constexpr uint32_t net_peer_mode_address() { return 0x826C7824u; }
  // the other console's answer, 255 none yet
  static uint8_t net_answer(uint8_t* base) { return guest::LoadU8(base, 0x826C6B0Cu); }
  static void set_net_answer(uint8_t* base, uint8_t v) { guest::StoreU8(base, 0x826C6B0Cu, v); }
  static constexpr uint32_t net_answer_address() { return 0x826C6B0Cu; }
  static uint32_t net_answer_word(uint8_t* base) { return guest::LoadU32(base, 0x826C6B04u); }
  static void set_net_answer_word(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C6B04u, v); }
  static constexpr uint32_t net_answer_word_address() { return 0x826C6B04u; }
  // the race data (race-states.toml): +60, +0x59620 the stages, +0x59AE8 the results records, +0xA3AF0 the kind
  template <typename T = RaceData>
  static T race_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5B2Cu)); }
  static void set_race_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5B2Cu, v.address); }
  static void set_race_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // the race's progress (race-states.toml): its +7244 is the id sent
  template <typename T = RaceProgress>
  static T net_players(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D82E8u)); }
  static void set_net_players(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D82E8u, v.address); }
  static void set_net_players(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D82E8u, v); }
  static constexpr uint32_t net_players_address() { return 0x826D82E8u; }
};

}  // namespace schema
