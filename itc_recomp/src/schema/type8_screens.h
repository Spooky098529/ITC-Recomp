// The views of docs/functions/type8-screens.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/type8-screens.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: Type8PanelScreen, PanelSetPair.+1908, PanelSetPair.+1912, PanelSetPair.+3880
struct Type8PanelScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 15252;
  static constexpr uint32_t kVtable = 0x82010AF0;
  // 0x82010AF0
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // its StateMachine (game-modes.toml); the measure meets its transitions' words at +88 and +316
  uint32_t machine_address() const { return address + 4; }
  // the machine's state (9 back, 10 leave)
  uint32_t state() const { return guest::LoadU32(base, address + 9608); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t state_address() const { return address + 9608; }
  // the car list (+11272)
  template <typename T = PanelSetPair>
  T car_list() const { return T(base, guest::LoadU32(base, address + 9644)); }
  void set_car_list(const View& v) const { guest::StoreU32(base, address + 9644, v.address); }
  void set_car_list(uint32_t v) const { guest::StoreU32(base, address + 9644, v); }
  uint32_t car_list_address() const { return address + 9644; }
  // the group holder (+15220)
  template <typename T = SingleGroup>
  T holder() const { return T(base, guest::LoadU32(base, address + 9648)); }
  void set_holder(const View& v) const { guest::StoreU32(base, address + 9648, v.address); }
  void set_holder(uint32_t v) const { guest::StoreU32(base, address + 9648, v); }
  uint32_t holder_address() const { return address + 9648; }
  // 1: PanelScreen::WaitForMovie goes to state 3, Show skips the car list's restore
  uint32_t opens_itself() const { return guest::LoadU32(base, address + 9652); }
  void set_opens_itself(uint32_t v) const { guest::StoreU32(base, address + 9652, v); }
  uint32_t opens_itself_address() const { return address + 9652; }
  // 0 the list, 1 the quit dialog, 2 leaving
  uint32_t draw_mode() const { return guest::LoadU32(base, address + 9656); }
  void set_draw_mode(uint32_t v) const { guest::StoreU32(base, address + 9656, v); }
  uint32_t draw_mode_address() const { return address + 9656; }
  // player 0's F panel (Type8TabPanel)
  uint32_t panel_f0_address() const { return address + 9660; }
  // player 0's E panel (Type8RowPanel)
  uint32_t panel_e0_address() const { return address + 9808; }
  // player 0's C panel (Type8CarPanel)
  uint32_t panel_c0_address() const { return address + 9972; }
  // player 0's D panel (Type8ColorPanel)
  uint32_t panel_d0_address() const { return address + 10296; }
  // player 0's confirm panel (Type8ConfirmPanel)
  uint32_t confirm0_address() const { return address + 10392; }
  // the destructor's store (0x82010B5C)
  uint32_t confirm0_vtable() const { return guest::LoadU32(base, address + 10392); }
  void set_confirm0_vtable(uint32_t v) const { guest::StoreU32(base, address + 10392, v); }
  uint32_t confirm0_vtable_address() const { return address + 10392; }
  // player 1's
  uint32_t panel_f1_address() const { return address + 10436; }
  uint32_t panel_e1_address() const { return address + 10584; }
  uint32_t panel_c1_address() const { return address + 10748; }
  uint32_t panel_d1_address() const { return address + 11072; }
  uint32_t confirm1_address() const { return address + 11168; }
  uint32_t confirm1_vtable() const { return guest::LoadU32(base, address + 11168); }
  void set_confirm1_vtable(uint32_t v) const { guest::StoreU32(base, address + 11168, v); }
  uint32_t confirm1_vtable_address() const { return address + 11168; }
  // the panel group (Type8PanelGroup, UI movie 4)
  uint32_t group_address() const { return address + 11212; }
  // 11: ShowPlayerNodes' node
  uint32_t player_node() const { return guest::LoadU32(base, address + 11256); }
  void set_player_node(uint32_t v) const { guest::StoreU32(base, address + 11256, v); }
  uint32_t player_node_address() const { return address + 11256; }
  // 4: the node's first child (six from it)
  uint32_t player_child() const { return guest::LoadU32(base, address + 11260); }
  void set_player_child(uint32_t v) const { guest::StoreU32(base, address + 11260, v); }
  uint32_t player_child_address() const { return address + 11260; }
  // 376: the group's opening animation
  uint32_t open_anim() const { return guest::LoadU32(base, address + 11264); }
  void set_open_anim(uint32_t v) const { guest::StoreU32(base, address + 11264, v); }
  uint32_t open_anim_address() const { return address + 11264; }
  // 354: looped after the opening
  uint32_t loop_anim() const { return guest::LoadU32(base, address + 11268); }
  void set_loop_anim(uint32_t v) const { guest::StoreU32(base, address + 11268, v); }
  uint32_t loop_anim_address() const { return address + 11268; }
  // the car list, a PanelSetPair (car-list.toml): bases at +11276 and +13248
  uint32_t pair_address() const { return address + 11272; }
  // base 0's page menu's player (PanelSetBase +120); 0 in the constructor
  uint32_t list0_page_menu_player() const { return guest::LoadU32(base, address + 11396); }
  void set_list0_page_menu_player(uint32_t v) const { guest::StoreU32(base, address + 11396, v); }
  uint32_t list0_page_menu_player_address() const { return address + 11396; }
  // base 0's PanelSetBase +232: 0 and 1 the unlocked cars, 2 the garage
  uint32_t list0_tab() const { return guest::LoadU32(base, address + 11508); }
  void set_list0_tab(uint32_t v) const { guest::StoreU32(base, address + 11508, v); }
  uint32_t list0_tab_address() const { return address + 11508; }
  // +236
  uint32_t list0_row() const { return guest::LoadU32(base, address + 11512); }
  void set_list0_row(uint32_t v) const { guest::StoreU32(base, address + 11512, v); }
  uint32_t list0_row_address() const { return address + 11512; }
  // +240
  uint32_t list0_scroll() const { return guest::LoadU32(base, address + 11516); }
  void set_list0_scroll(uint32_t v) const { guest::StoreU32(base, address + 11516, v); }
  uint32_t list0_scroll_address() const { return address + 11516; }
  // +244
  uint32_t list0_color() const { return guest::LoadU32(base, address + 11520); }
  void set_list0_color(uint32_t v) const { guest::StoreU32(base, address + 11520, v); }
  uint32_t list0_color_address() const { return address + 11520; }
  // +248
  uint32_t list0_color_scroll() const { return guest::LoadU32(base, address + 11524); }
  void set_list0_color_scroll(uint32_t v) const { guest::StoreU32(base, address + 11524, v); }
  uint32_t list0_color_scroll_address() const { return address + 11524; }
  // base 0's +1820, the CarListList shown
  uint32_t list0_current_list() const { return guest::LoadU32(base, address + 13096); }
  void set_list0_current_list(uint32_t v) const { guest::StoreU32(base, address + 13096, v); }
  uint32_t list0_current_list_address() const { return address + 13096; }
  // base 0's +1916 (3, InitWords)
  uint32_t list0_rows_shown() const { return guest::LoadU32(base, address + 13192); }
  void set_list0_rows_shown(uint32_t v) const { guest::StoreU32(base, address + 13192, v); }
  uint32_t list0_rows_shown_address() const { return address + 13192; }
  // base 0's +1920 (0 in the constructor)
  uint32_t list0_unknown_1920() const { return guest::LoadU32(base, address + 13196); }
  void set_list0_unknown_1920(uint32_t v) const { guest::StoreU32(base, address + 13196, v); }
  uint32_t list0_unknown_1920_address() const { return address + 13196; }
  // base 0's +1924 (4, InitWords)
  uint32_t list0_colors_shown() const { return guest::LoadU32(base, address + 13200); }
  void set_list0_colors_shown(uint32_t v) const { guest::StoreU32(base, address + 13200, v); }
  uint32_t list0_colors_shown_address() const { return address + 13200; }
  // base 0's +1944: set when player 0 backs out
  uint32_t list0_cancelled() const { return guest::LoadU32(base, address + 13220); }
  void set_list0_cancelled(uint32_t v) const { guest::StoreU32(base, address + 13220, v); }
  uint32_t list0_cancelled_address() const { return address + 13220; }
  // base 0's +1960 (360)
  uint32_t list0_busy_anim_a() const { return guest::LoadU32(base, address + 13236); }
  void set_list0_busy_anim_a(uint32_t v) const { guest::StoreU32(base, address + 13236, v); }
  uint32_t list0_busy_anim_a_address() const { return address + 13236; }
  // +1964 (359)
  uint32_t list0_busy_anim_b() const { return guest::LoadU32(base, address + 13240); }
  void set_list0_busy_anim_b(uint32_t v) const { guest::StoreU32(base, address + 13240, v); }
  uint32_t list0_busy_anim_b_address() const { return address + 13240; }
  // +1968 (361)
  uint32_t list0_busy_anim_done() const { return guest::LoadU32(base, address + 13244); }
  void set_list0_busy_anim_done(uint32_t v) const { guest::StoreU32(base, address + 13244, v); }
  uint32_t list0_busy_anim_done_address() const { return address + 13244; }
  // base 1's +120 (1 in the constructor)
  uint32_t list1_page_menu_player() const { return guest::LoadU32(base, address + 13368); }
  void set_list1_page_menu_player(uint32_t v) const { guest::StoreU32(base, address + 13368, v); }
  uint32_t list1_page_menu_player_address() const { return address + 13368; }
  // base 1's +1916 (3)
  uint32_t list1_rows_shown() const { return guest::LoadU32(base, address + 15164); }
  void set_list1_rows_shown(uint32_t v) const { guest::StoreU32(base, address + 15164, v); }
  uint32_t list1_rows_shown_address() const { return address + 15164; }
  // base 1's +1920
  uint32_t list1_unknown_1920() const { return guest::LoadU32(base, address + 15168); }
  void set_list1_unknown_1920(uint32_t v) const { guest::StoreU32(base, address + 15168, v); }
  uint32_t list1_unknown_1920_address() const { return address + 15168; }
  // base 1's +1924 (4)
  uint32_t list1_colors_shown() const { return guest::LoadU32(base, address + 15172); }
  void set_list1_colors_shown(uint32_t v) const { guest::StoreU32(base, address + 15172, v); }
  uint32_t list1_colors_shown_address() const { return address + 15172; }
  // base 1's +1944
  uint32_t list1_cancelled() const { return guest::LoadU32(base, address + 15192); }
  void set_list1_cancelled(uint32_t v) const { guest::StoreU32(base, address + 15192, v); }
  uint32_t list1_cancelled_address() const { return address + 15192; }
  // base 1's +1960 (363)
  uint32_t list1_busy_anim_a() const { return guest::LoadU32(base, address + 15208); }
  void set_list1_busy_anim_a(uint32_t v) const { guest::StoreU32(base, address + 15208, v); }
  uint32_t list1_busy_anim_a_address() const { return address + 15208; }
  // +1964 (362)
  uint32_t list1_busy_anim_b() const { return guest::LoadU32(base, address + 15212); }
  void set_list1_busy_anim_b(uint32_t v) const { guest::StoreU32(base, address + 15212, v); }
  uint32_t list1_busy_anim_b_address() const { return address + 15212; }
  // +1968 (364)
  uint32_t list1_busy_anim_done() const { return guest::LoadU32(base, address + 15216); }
  void set_list1_busy_anim_done(uint32_t v) const { guest::StoreU32(base, address + 15216, v); }
  uint32_t list1_busy_anim_done_address() const { return address + 15216; }
  // the SingleGroup holding the panel group
  uint32_t group_holder_address() const { return address + 15220; }
  // message 2 got: the opening may start
  uint32_t message2_got() const { return guest::LoadU32(base, address + 15228); }
  void set_message2_got(uint32_t v) const { guest::StoreU32(base, address + 15228, v); }
  uint32_t message2_got_address() const { return address + 15228; }
  // the save load asked (message 29); cleared by 31
  uint32_t load_asked() const { return guest::LoadU32(base, address + 15232); }
  void set_load_asked(uint32_t v) const { guest::StoreU32(base, address + 15232, v); }
  uint32_t load_asked_address() const { return address + 15232; }
  uint32_t opening_played() const { return guest::LoadU32(base, address + 15236); }
  void set_opening_played(uint32_t v) const { guest::StoreU32(base, address + 15236, v); }
  uint32_t opening_played_address() const { return address + 15236; }
  // the load asked this visit (0 after a race: no fade in, backing out leaves without the dialog)
  uint32_t loaded_this_visit() const { return guest::LoadU32(base, address + 15240); }
  void set_loaded_this_visit(uint32_t v) const { guest::StoreU32(base, address + 15240, v); }
  uint32_t loaded_this_visit_address() const { return address + 15240; }
  // -1 none, 0 stay, 1 or 2 leave
  uint32_t dialog_answer() const { return guest::LoadU32(base, address + 15244); }
  void set_dialog_answer(uint32_t v) const { guest::StoreU32(base, address + 15244, v); }
  uint32_t dialog_answer_address() const { return address + 15244; }
  // 0 in the constructor
  uint32_t unknown_15248() const { return guest::LoadU32(base, address + 15248); }
  void set_unknown_15248(uint32_t v) const { guest::StoreU32(base, address + 15248, v); }
  uint32_t unknown_15248_address() const { return address + 15248; }
};

// image; measure: Type8PanelScreen.+15220
struct Type8HolderVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t deleting_destructor() const { return guest::LoadU32(base, address + 0); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t deleting_destructor_address() const { return address + 0; }
  // slot 1 (GroupPair::Run)
  uint32_t run() const { return guest::LoadU32(base, address + 4); }
  void set_run(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t run_address() const { return address + 4; }
  // slot 2 (GroupPair::WaitForFade)
  uint32_t wait_for_fade() const { return guest::LoadU32(base, address + 8); }
  void set_wait_for_fade(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t wait_for_fade_address() const { return address + 8; }
};

// embedded; measure: PanelGroup8
struct Type8PanelGroup : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  static constexpr uint32_t kVtable = 0x82010B48;
  // 0x82010B48
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // its UI movie (4)
  uint32_t movie() const { return guest::LoadU32(base, address + 4); }
  void set_movie(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t movie_address() const { return address + 4; }
  // 10
  uint32_t text_node_set() const { return guest::LoadU32(base, address + 12); }
  void set_text_node_set(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t text_node_set_address() const { return address + 12; }
  // 0 in the constructor
  uint32_t caption_node() const { return guest::LoadU32(base, address + 32); }
  void set_caption_node(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t caption_node_address() const { return address + 32; }
};

// embedded; measure: Panel8F
struct Type8TabPanel : View {
  using View::View;
  static constexpr uint32_t kSize = 148;
  static constexpr uint32_t kVtable = 0x82001DD8;
  // 0x82001DD8 (ListNode's)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = Type8PanelGroup>
  T group() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_group(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_group(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t group_address() const { return address + 4; }
  // 6 or 7 by the player
  uint32_t node_set() const { return guest::LoadU32(base, address + 24); }
  void set_node_set(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t node_set_address() const { return address + 24; }
  uint32_t texts_node() const { return guest::LoadU32(base, address + 28); }
  void set_texts_node(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t texts_node_address() const { return address + 28; }
  uint32_t tabs_node() const { return guest::LoadU32(base, address + 32); }
  void set_tabs_node(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t tabs_node_address() const { return address + 32; }
  uint32_t buttons_node() const { return guest::LoadU32(base, address + 36); }
  void set_buttons_node(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t buttons_node_address() const { return address + 36; }
  uint32_t cursor() const { return guest::LoadU32(base, address + 40); }
  void set_cursor(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t cursor_address() const { return address + 40; }
  uint32_t tab0_text() const { return guest::LoadU32(base, address + 44); }
  void set_tab0_text(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t tab0_text_address() const { return address + 44; }
  uint32_t tab1_text() const { return guest::LoadU32(base, address + 48); }
  void set_tab1_text(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t tab1_text_address() const { return address + 48; }
  uint32_t tab2_text() const { return guest::LoadU32(base, address + 52); }
  void set_tab2_text(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t tab2_text_address() const { return address + 52; }
  uint32_t buttons() const { return guest::LoadU32(base, address + 84); }
  void set_buttons(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t buttons_address() const { return address + 84; }
  uint32_t tab_word_a() const { return guest::LoadU32(base, address + 88); }
  void set_tab_word_a(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t tab_word_a_address() const { return address + 88; }
  uint32_t tab_word_b() const { return guest::LoadU32(base, address + 92); }
  void set_tab_word_b(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t tab_word_b_address() const { return address + 92; }
  uint32_t tab_anims() const { return guest::LoadU32(base, address + 96); }
  void set_tab_anims(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t tab_anims_address() const { return address + 96; }
  uint32_t scroll_down() const { return guest::LoadU32(base, address + 100); }
  void set_scroll_down(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t scroll_down_address() const { return address + 100; }
  uint32_t scroll_up() const { return guest::LoadU32(base, address + 104); }
  void set_scroll_up(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t scroll_up_address() const { return address + 104; }
  uint32_t show_anim() const { return guest::LoadU32(base, address + 116); }
  void set_show_anim(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t show_anim_address() const { return address + 116; }
  uint32_t move_anim() const { return guest::LoadU32(base, address + 120); }
  void set_move_anim(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t move_anim_address() const { return address + 120; }
  uint32_t loop_a() const { return guest::LoadU32(base, address + 124); }
  void set_loop_a(uint32_t v) const { guest::StoreU32(base, address + 124, v); }
  uint32_t loop_a_address() const { return address + 124; }
  uint32_t loop_b() const { return guest::LoadU32(base, address + 128); }
  void set_loop_b(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t loop_b_address() const { return address + 128; }
  // 0 or 1
  uint32_t player() const { return guest::LoadU32(base, address + 144); }
  void set_player(uint32_t v) const { guest::StoreU32(base, address + 144, v); }
  uint32_t player_address() const { return address + 144; }
};

// embedded; measure: Panel8E
struct Type8RowPanel : View {
  using View::View;
  static constexpr uint32_t kSize = 164;
  template <typename T = Type8PanelGroup>
  T group() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_group(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_group(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t group_address() const { return address + 0; }
  uint32_t row_count() const { return guest::LoadU32(base, address + 4); }
  void set_row_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t row_count_address() const { return address + 4; }
  uint32_t node_set() const { return guest::LoadU32(base, address + 12); }
  void set_node_set(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t node_set_address() const { return address + 12; }
  uint32_t node() const { return guest::LoadU32(base, address + 16); }
  void set_node(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t node_address() const { return address + 16; }
  // the rows' child indices (RowPanel's row_nodes)
  uint32_t row_node0() const { return guest::LoadU32(base, address + 20); }
  void set_row_node0(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t row_node0_address() const { return address + 20; }
  uint32_t row_node1() const { return guest::LoadU32(base, address + 24); }
  void set_row_node1(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t row_node1_address() const { return address + 24; }
  uint32_t row_node2() const { return guest::LoadU32(base, address + 28); }
  void set_row_node2(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t row_node2_address() const { return address + 28; }
  uint32_t spare_row_node() const { return guest::LoadU32(base, address + 60); }
  void set_spare_row_node(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t spare_row_node_address() const { return address + 60; }
  // 15 or 186 by the player
  uint32_t unknown_64() const { return guest::LoadU32(base, address + 64); }
  void set_unknown_64(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  // 25 or 196
  uint32_t unknown_68() const { return guest::LoadU32(base, address + 68); }
  void set_unknown_68(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
  // 26 or 197
  uint32_t unknown_72() const { return guest::LoadU32(base, address + 72); }
  void set_unknown_72(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
  uint32_t show_anim() const { return guest::LoadU32(base, address + 76); }
  void set_show_anim(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t show_anim_address() const { return address + 76; }
  uint32_t hide_anim() const { return guest::LoadU32(base, address + 88); }
  void set_hide_anim(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t hide_anim_address() const { return address + 88; }
  uint32_t row_anims() const { return guest::LoadU32(base, address + 92); }
  void set_row_anims(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t row_anims_address() const { return address + 92; }
  uint32_t scroll_up() const { return guest::LoadU32(base, address + 96); }
  void set_scroll_up(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t scroll_up_address() const { return address + 96; }
  uint32_t scroll_down() const { return guest::LoadU32(base, address + 100); }
  void set_scroll_down(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t scroll_down_address() const { return address + 100; }
  uint32_t arrow_up() const { return guest::LoadU32(base, address + 104); }
  void set_arrow_up(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t arrow_up_address() const { return address + 104; }
  uint32_t arrow_down() const { return guest::LoadU32(base, address + 108); }
  void set_arrow_down(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t arrow_down_address() const { return address + 108; }
  uint32_t logo() const { return guest::LoadU32(base, address + 120); }
  void set_logo(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t logo_address() const { return address + 120; }
  uint32_t text_maker() const { return guest::LoadU32(base, address + 124); }
  void set_text_maker(uint32_t v) const { guest::StoreU32(base, address + 124, v); }
  uint32_t text_maker_address() const { return address + 124; }
  uint32_t text_name() const { return guest::LoadU32(base, address + 128); }
  void set_text_name(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t text_name_address() const { return address + 128; }
  uint32_t text_third() const { return guest::LoadU32(base, address + 132); }
  void set_text_third(uint32_t v) const { guest::StoreU32(base, address + 132, v); }
  uint32_t text_third_address() const { return address + 132; }
  uint32_t cursor() const { return guest::LoadU32(base, address + 144); }
  void set_cursor(uint32_t v) const { guest::StoreU32(base, address + 144, v); }
  uint32_t cursor_address() const { return address + 144; }
  uint32_t arrows() const { return guest::LoadU32(base, address + 148); }
  void set_arrows(uint32_t v) const { guest::StoreU32(base, address + 148, v); }
  uint32_t arrows_address() const { return address + 148; }
  uint32_t player() const { return guest::LoadU32(base, address + 160); }
  void set_player(uint32_t v) const { guest::StoreU32(base, address + 160, v); }
  uint32_t player_address() const { return address + 160; }
};

// embedded; measure: Panel8C
struct Type8CarPanel : View {
  using View::View;
  static constexpr uint32_t kSize = 324;
  static constexpr uint32_t kVtable = 0x82010B4C;
  // 0x82010B4C
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t shows_price() const { return guest::LoadU32(base, address + 4); }
  void set_shows_price(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t shows_price_address() const { return address + 4; }
  template <typename T = Type8PanelGroup>
  T group() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_group(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_group(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t group_address() const { return address + 12; }
  uint32_t node_set() const { return guest::LoadU32(base, address + 16); }
  void set_node_set(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t node_set_address() const { return address + 16; }
  uint32_t node() const { return guest::LoadU32(base, address + 20); }
  void set_node(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t node_address() const { return address + 20; }
  uint32_t stats_node() const { return guest::LoadU32(base, address + 24); }
  void set_stats_node(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t stats_node_address() const { return address + 24; }
  uint32_t buttons_node() const { return guest::LoadU32(base, address + 32); }
  void set_buttons_node(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t buttons_node_address() const { return address + 32; }
  uint32_t name_node() const { return guest::LoadU32(base, address + 36); }
  void set_name_node(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t name_node_address() const { return address + 36; }
  uint32_t show_anim_a() const { return guest::LoadU32(base, address + 40); }
  void set_show_anim_a(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t show_anim_a_address() const { return address + 40; }
  uint32_t show_anim_b() const { return guest::LoadU32(base, address + 44); }
  void set_show_anim_b(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t show_anim_b_address() const { return address + 44; }
  uint32_t hide_anim_a() const { return guest::LoadU32(base, address + 48); }
  void set_hide_anim_a(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t hide_anim_a_address() const { return address + 48; }
  uint32_t hide_anim_b() const { return guest::LoadU32(base, address + 52); }
  void set_hide_anim_b(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t hide_anim_b_address() const { return address + 52; }
  uint32_t show_again_anim() const { return guest::LoadU32(base, address + 56); }
  void set_show_again_anim(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t show_again_anim_address() const { return address + 56; }
  uint32_t arrow_a() const { return guest::LoadU32(base, address + 60); }
  void set_arrow_a(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t arrow_a_address() const { return address + 60; }
  uint32_t arrow_b() const { return guest::LoadU32(base, address + 64); }
  void set_arrow_b(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t arrow_b_address() const { return address + 64; }
  // CarPanel's action_anims
  uint32_t action_anim0() const { return guest::LoadU32(base, address + 68); }
  void set_action_anim0(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t action_anim0_address() const { return address + 68; }
  uint32_t action_anim1() const { return guest::LoadU32(base, address + 72); }
  void set_action_anim1(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t action_anim1_address() const { return address + 72; }
  uint32_t scroll_back() const { return guest::LoadU32(base, address + 108); }
  void set_scroll_back(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t scroll_back_address() const { return address + 108; }
  uint32_t scroll_on() const { return guest::LoadU32(base, address + 112); }
  void set_scroll_on(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t scroll_on_address() const { return address + 112; }
  uint32_t stats_in() const { return guest::LoadU32(base, address + 116); }
  void set_stats_in(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t stats_in_address() const { return address + 116; }
  uint32_t stats_out() const { return guest::LoadU32(base, address + 120); }
  void set_stats_out(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t stats_out_address() const { return address + 120; }
  uint32_t text_maker() const { return guest::LoadU32(base, address + 132); }
  void set_text_maker(uint32_t v) const { guest::StoreU32(base, address + 132, v); }
  uint32_t text_maker_address() const { return address + 132; }
  uint32_t text_name() const { return guest::LoadU32(base, address + 136); }
  void set_text_name(uint32_t v) const { guest::StoreU32(base, address + 136, v); }
  uint32_t text_name_address() const { return address + 136; }
  uint32_t text_third() const { return guest::LoadU32(base, address + 140); }
  void set_text_third(uint32_t v) const { guest::StoreU32(base, address + 140, v); }
  uint32_t text_third_address() const { return address + 140; }
  uint32_t texts() const { return guest::LoadU32(base, address + 144); }
  void set_texts(uint32_t v) const { guest::StoreU32(base, address + 144, v); }
  uint32_t texts_address() const { return address + 144; }
  uint32_t cursor() const { return guest::LoadU32(base, address + 152); }
  void set_cursor(uint32_t v) const { guest::StoreU32(base, address + 152, v); }
  uint32_t cursor_address() const { return address + 152; }
  // ShowArrows' child (negative: none)
  uint32_t arrows_child() const { return guest::LoadU32(base, address + 156); }
  void set_arrows_child(uint32_t v) const { guest::StoreU32(base, address + 156, v); }
  uint32_t arrows_child_address() const { return address + 156; }
  // 22 child indices of the stats node (CarPanel's)
  uint32_t stats_children_address() const { return address + 160; }
  uint32_t stats_child0() const { return guest::LoadU32(base, address + 160); }
  void set_stats_child0(uint32_t v) const { guest::StoreU32(base, address + 160, v); }
  uint32_t stats_child0_address() const { return address + 160; }
  uint32_t stats_child1() const { return guest::LoadU32(base, address + 164); }
  void set_stats_child1(uint32_t v) const { guest::StoreU32(base, address + 164, v); }
  uint32_t stats_child1_address() const { return address + 164; }
  uint32_t stats_child2() const { return guest::LoadU32(base, address + 168); }
  void set_stats_child2(uint32_t v) const { guest::StoreU32(base, address + 168, v); }
  uint32_t stats_child2_address() const { return address + 168; }
  uint32_t stats_child3() const { return guest::LoadU32(base, address + 172); }
  void set_stats_child3(uint32_t v) const { guest::StoreU32(base, address + 172, v); }
  uint32_t stats_child3_address() const { return address + 172; }
  uint32_t stats_child4() const { return guest::LoadU32(base, address + 176); }
  void set_stats_child4(uint32_t v) const { guest::StoreU32(base, address + 176, v); }
  uint32_t stats_child4_address() const { return address + 176; }
  uint32_t stats_child5() const { return guest::LoadU32(base, address + 180); }
  void set_stats_child5(uint32_t v) const { guest::StoreU32(base, address + 180, v); }
  uint32_t stats_child5_address() const { return address + 180; }
  uint32_t stats_child6() const { return guest::LoadU32(base, address + 184); }
  void set_stats_child6(uint32_t v) const { guest::StoreU32(base, address + 184, v); }
  uint32_t stats_child6_address() const { return address + 184; }
  uint32_t stats_child7() const { return guest::LoadU32(base, address + 188); }
  void set_stats_child7(uint32_t v) const { guest::StoreU32(base, address + 188, v); }
  uint32_t stats_child7_address() const { return address + 188; }
  uint32_t stats_child8() const { return guest::LoadU32(base, address + 192); }
  void set_stats_child8(uint32_t v) const { guest::StoreU32(base, address + 192, v); }
  uint32_t stats_child8_address() const { return address + 192; }
  uint32_t stats_child9() const { return guest::LoadU32(base, address + 196); }
  void set_stats_child9(uint32_t v) const { guest::StoreU32(base, address + 196, v); }
  uint32_t stats_child9_address() const { return address + 196; }
  uint32_t stats_child10() const { return guest::LoadU32(base, address + 200); }
  void set_stats_child10(uint32_t v) const { guest::StoreU32(base, address + 200, v); }
  uint32_t stats_child10_address() const { return address + 200; }
  uint32_t stats_child11() const { return guest::LoadU32(base, address + 204); }
  void set_stats_child11(uint32_t v) const { guest::StoreU32(base, address + 204, v); }
  uint32_t stats_child11_address() const { return address + 204; }
  uint32_t stats_child12() const { return guest::LoadU32(base, address + 208); }
  void set_stats_child12(uint32_t v) const { guest::StoreU32(base, address + 208, v); }
  uint32_t stats_child12_address() const { return address + 208; }
  uint32_t stats_child13() const { return guest::LoadU32(base, address + 212); }
  void set_stats_child13(uint32_t v) const { guest::StoreU32(base, address + 212, v); }
  uint32_t stats_child13_address() const { return address + 212; }
  uint32_t stats_child14() const { return guest::LoadU32(base, address + 216); }
  void set_stats_child14(uint32_t v) const { guest::StoreU32(base, address + 216, v); }
  uint32_t stats_child14_address() const { return address + 216; }
  uint32_t stats_child15() const { return guest::LoadU32(base, address + 220); }
  void set_stats_child15(uint32_t v) const { guest::StoreU32(base, address + 220, v); }
  uint32_t stats_child15_address() const { return address + 220; }
  uint32_t stats_child16() const { return guest::LoadU32(base, address + 224); }
  void set_stats_child16(uint32_t v) const { guest::StoreU32(base, address + 224, v); }
  uint32_t stats_child16_address() const { return address + 224; }
  uint32_t stats_child17() const { return guest::LoadU32(base, address + 228); }
  void set_stats_child17(uint32_t v) const { guest::StoreU32(base, address + 228, v); }
  uint32_t stats_child17_address() const { return address + 228; }
  uint32_t stats_child18() const { return guest::LoadU32(base, address + 232); }
  void set_stats_child18(uint32_t v) const { guest::StoreU32(base, address + 232, v); }
  uint32_t stats_child18_address() const { return address + 232; }
  uint32_t stats_child19() const { return guest::LoadU32(base, address + 236); }
  void set_stats_child19(uint32_t v) const { guest::StoreU32(base, address + 236, v); }
  uint32_t stats_child19_address() const { return address + 236; }
  uint32_t stats_child20() const { return guest::LoadU32(base, address + 240); }
  void set_stats_child20(uint32_t v) const { guest::StoreU32(base, address + 240, v); }
  uint32_t stats_child20_address() const { return address + 240; }
  uint32_t stats_child21() const { return guest::LoadU32(base, address + 244); }
  void set_stats_child21(uint32_t v) const { guest::StoreU32(base, address + 244, v); }
  uint32_t stats_child21_address() const { return address + 244; }
  uint32_t buttons() const { return guest::LoadU32(base, address + 272); }
  void set_buttons(uint32_t v) const { guest::StoreU32(base, address + 272, v); }
  uint32_t buttons_address() const { return address + 272; }
  uint32_t text_a() const { return guest::LoadU32(base, address + 276); }
  void set_text_a(uint32_t v) const { guest::StoreU32(base, address + 276, v); }
  uint32_t text_a_address() const { return address + 276; }
  uint32_t text_b() const { return guest::LoadU32(base, address + 280); }
  void set_text_b(uint32_t v) const { guest::StoreU32(base, address + 280, v); }
  uint32_t text_b_address() const { return address + 280; }
  uint32_t player() const { return guest::LoadU32(base, address + 320); }
  void set_player(uint32_t v) const { guest::StoreU32(base, address + 320, v); }
  uint32_t player_address() const { return address + 320; }
};

// embedded; measure: Panel8D
struct Type8ColorPanel : View {
  using View::View;
  static constexpr uint32_t kSize = 96;
  static constexpr uint32_t kVtable = 0x82001DD8;
  // 0x82001DD8 (ListNode's)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = Type8PanelGroup>
  T group() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_group(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_group(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t group_address() const { return address + 4; }
  uint32_t node_set() const { return guest::LoadU32(base, address + 8); }
  void set_node_set(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t node_set_address() const { return address + 8; }
  uint32_t node() const { return guest::LoadU32(base, address + 12); }
  void set_node(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t node_address() const { return address + 12; }
  uint32_t show_anim() const { return guest::LoadU32(base, address + 16); }
  void set_show_anim(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t show_anim_address() const { return address + 16; }
  uint32_t hide_anim() const { return guest::LoadU32(base, address + 20); }
  void set_hide_anim(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t hide_anim_address() const { return address + 20; }
  uint32_t item_anims() const { return guest::LoadU32(base, address + 24); }
  void set_item_anims(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t item_anims_address() const { return address + 24; }
  uint32_t scroll_back() const { return guest::LoadU32(base, address + 28); }
  void set_scroll_back(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t scroll_back_address() const { return address + 28; }
  uint32_t scroll_on() const { return guest::LoadU32(base, address + 32); }
  void set_scroll_on(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t scroll_on_address() const { return address + 32; }
  uint32_t swatches_on() const { return guest::LoadU32(base, address + 36); }
  void set_swatches_on(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t swatches_on_address() const { return address + 36; }
  uint32_t swatches_back() const { return guest::LoadU32(base, address + 40); }
  void set_swatches_back(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t swatches_back_address() const { return address + 40; }
  uint32_t arrow_a() const { return guest::LoadU32(base, address + 52); }
  void set_arrow_a(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t arrow_a_address() const { return address + 52; }
  uint32_t arrow_b() const { return guest::LoadU32(base, address + 56); }
  void set_arrow_b(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t arrow_b_address() const { return address + 56; }
  uint32_t arrows_node() const { return guest::LoadU32(base, address + 60); }
  void set_arrows_node(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t arrows_node_address() const { return address + 60; }
  uint32_t cursor() const { return guest::LoadU32(base, address + 64); }
  void set_cursor(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t cursor_address() const { return address + 64; }
  uint32_t swatches() const { return guest::LoadU32(base, address + 68); }
  void set_swatches(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t swatches_address() const { return address + 68; }
  uint32_t arrow_up_node() const { return guest::LoadU32(base, address + 72); }
  void set_arrow_up_node(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t arrow_up_node_address() const { return address + 72; }
  uint32_t arrow_down_node() const { return guest::LoadU32(base, address + 76); }
  void set_arrow_down_node(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t arrow_down_node_address() const { return address + 76; }
  uint32_t swatch_color() const { return guest::LoadU32(base, address + 80); }
  void set_swatch_color(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t swatch_color_address() const { return address + 80; }
  uint32_t player() const { return guest::LoadU32(base, address + 92); }
  void set_player(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t player_address() const { return address + 92; }
};

// embedded; measure: ConfirmPanel8
struct Type8ConfirmPanel : View {
  using View::View;
  static constexpr uint32_t kSize = 44;
  static constexpr uint32_t kVtable = 0x82010B5C;
  // 0x82010B5C
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = Type8PanelGroup>
  T group() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_group(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_group(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t group_address() const { return address + 4; }
  // 0 or 1 by the player
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t node_a() const { return guest::LoadU32(base, address + 16); }
  void set_node_a(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t node_a_address() const { return address + 16; }
  uint32_t node_b() const { return guest::LoadU32(base, address + 20); }
  void set_node_b(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t node_b_address() const { return address + 20; }
  uint32_t node_c() const { return guest::LoadU32(base, address + 24); }
  void set_node_c(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t node_c_address() const { return address + 24; }
  uint32_t show_anim() const { return guest::LoadU32(base, address + 28); }
  void set_show_anim(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t show_anim_address() const { return address + 28; }
  uint32_t loop_anim() const { return guest::LoadU32(base, address + 32); }
  void set_loop_anim(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t loop_anim_address() const { return address + 32; }
  uint32_t hide_anim() const { return guest::LoadU32(base, address + 36); }
  void set_hide_anim(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t hide_anim_address() const { return address + 36; }
  uint32_t player() const { return guest::LoadU32(base, address + 40); }
  void set_player(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t player_address() const { return address + 40; }
};

// embedded; measure: Type8ListScreen, Type8ListBase
struct Type8ListScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 11368;
  static constexpr uint32_t kVtable = 0x82010E18;
  // 0x82010E18 (0x82011638 while only the base is built)
  template <typename T = Type8ScreenVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the page block: the page (-1 while a step runs)
  uint32_t page() const { return guest::LoadU32(base, address + 9644); }
  void set_page(uint32_t v) const { guest::StoreU32(base, address + 9644, v); }
  uint32_t page_address() const { return address + 9644; }
  // the page after the step
  uint32_t next_page() const { return guest::LoadU32(base, address + 9648); }
  void set_next_page(uint32_t v) const { guest::StoreU32(base, address + 9648, v); }
  uint32_t next_page_address() const { return address + 9648; }
  uint32_t previous_page() const { return guest::LoadU32(base, address + 9652); }
  void set_previous_page(uint32_t v) const { guest::StoreU32(base, address + 9652, v); }
  uint32_t previous_page_address() const { return address + 9652; }
  // -1 none
  uint32_t step() const { return guest::LoadU32(base, address + 9656); }
  void set_step(uint32_t v) const { guest::StoreU32(base, address + 9656, v); }
  uint32_t step_address() const { return address + 9656; }
  uint32_t queued_step() const { return guest::LoadU32(base, address + 9660); }
  void set_queued_step(uint32_t v) const { guest::StoreU32(base, address + 9660, v); }
  uint32_t queued_step_address() const { return address + 9660; }
  // the step's first-pass word
  uint32_t step_started() const { return guest::LoadU32(base, address + 9668); }
  void set_step_started(uint32_t v) const { guest::StoreU32(base, address + 9668, v); }
  uint32_t step_started_address() const { return address + 9668; }
  // the node table (+10360)
  template <typename T = PairedNodeTable>
  T node_table() const { return T(base, guest::LoadU32(base, address + 9672)); }
  void set_node_table(const View& v) const { guest::StoreU32(base, address + 9672, v.address); }
  void set_node_table(uint32_t v) const { guest::StoreU32(base, address + 9672, v); }
  uint32_t node_table_address() const { return address + 9672; }
  // the list part (+10844)
  template <typename T = Type8ListPart>
  T part() const { return T(base, guest::LoadU32(base, address + 9676)); }
  void set_part(const View& v) const { guest::StoreU32(base, address + 9676, v.address); }
  void set_part(uint32_t v) const { guest::StoreU32(base, address + 9676, v); }
  uint32_t part_address() const { return address + 9676; }
  // its UI movie
  uint32_t movie_id() const { return guest::LoadU32(base, address + 9680); }
  void set_movie_id(uint32_t v) const { guest::StoreU32(base, address + 9680, v); }
  uint32_t movie_id_address() const { return address + 9680; }
  // 1 once the race is set (ListScreen::OnLeft goes to state 11)
  uint32_t race_set() const { return guest::LoadU32(base, address + 9688); }
  void set_race_set(uint32_t v) const { guest::StoreU32(base, address + 9688, v); }
  uint32_t race_set_address() const { return address + 9688; }
  // 1 while the list part is open
  uint32_t part_open() const { return guest::LoadU32(base, address + 9696); }
  void set_part_open(uint32_t v) const { guest::StoreU32(base, address + 9696, v); }
  uint32_t part_open_address() const { return address + 9696; }
  // 1
  uint32_t show_part_nodes() const { return guest::LoadU32(base, address + 9708); }
  void set_show_part_nodes(uint32_t v) const { guest::StoreU32(base, address + 9708, v); }
  uint32_t show_part_nodes_address() const { return address + 9708; }
  // 0 in the constructor
  uint32_t unknown_9712() const { return guest::LoadU32(base, address + 9712); }
  void set_unknown_9712(uint32_t v) const { guest::StoreU32(base, address + 9712, v); }
  uint32_t unknown_9712_address() const { return address + 9712; }
  // 10
  uint32_t unknown_9716() const { return guest::LoadU32(base, address + 9716); }
  void set_unknown_9716(uint32_t v) const { guest::StoreU32(base, address + 9716, v); }
  uint32_t unknown_9716_address() const { return address + 9716; }
  // 11
  uint32_t unknown_9720() const { return guest::LoadU32(base, address + 9720); }
  void set_unknown_9720(uint32_t v) const { guest::StoreU32(base, address + 9720, v); }
  uint32_t unknown_9720_address() const { return address + 9720; }
  // 15: ShowSelection's nodes
  uint32_t selection_node_set() const { return guest::LoadU32(base, address + 9724); }
  void set_selection_node_set(uint32_t v) const { guest::StoreU32(base, address + 9724, v); }
  uint32_t selection_node_set_address() const { return address + 9724; }
  // 6: part_node's
  uint32_t part_node_set() const { return guest::LoadU32(base, address + 9728); }
  void set_part_node_set(uint32_t v) const { guest::StoreU32(base, address + 9728, v); }
  uint32_t part_node_set_address() const { return address + 9728; }
  // 2 (SetIcons)
  uint32_t icons_node_set() const { return guest::LoadU32(base, address + 9732); }
  void set_icons_node_set(uint32_t v) const { guest::StoreU32(base, address + 9732, v); }
  uint32_t icons_node_set_address() const { return address + 9732; }
  // 20
  uint32_t color_cursor_node_set() const { return guest::LoadU32(base, address + 9736); }
  void set_color_cursor_node_set(uint32_t v) const { guest::StoreU32(base, address + 9736, v); }
  uint32_t color_cursor_node_set_address() const { return address + 9736; }
  // 1 (SetScrollText)
  uint32_t scroll_text_node_set() const { return guest::LoadU32(base, address + 9740); }
  void set_scroll_text_node_set(uint32_t v) const { guest::StoreU32(base, address + 9740, v); }
  uint32_t scroll_text_node_set_address() const { return address + 9740; }
  // 20: SetItemTexts' texts, the item cursor
  uint32_t item_node_set() const { return guest::LoadU32(base, address + 9744); }
  void set_item_node_set(uint32_t v) const { guest::StoreU32(base, address + 9744, v); }
  uint32_t item_node_set_address() const { return address + 9744; }
  // 0: shown or hidden by the list part's helpers
  uint32_t marker_node() const { return guest::LoadU32(base, address + 9748); }
  void set_marker_node(uint32_t v) const { guest::StoreU32(base, address + 9748, v); }
  uint32_t marker_node_address() const { return address + 9748; }
  // 8 (SetArrows)
  uint32_t arrow_a_node() const { return guest::LoadU32(base, address + 9752); }
  void set_arrow_a_node(uint32_t v) const { guest::StoreU32(base, address + 9752, v); }
  uint32_t arrow_a_node_address() const { return address + 9752; }
  // 9
  uint32_t arrow_b_node() const { return guest::LoadU32(base, address + 9756); }
  void set_arrow_b_node(uint32_t v) const { guest::StoreU32(base, address + 9756, v); }
  uint32_t arrow_b_node_address() const { return address + 9756; }
  // 3 (ShowSelection)
  uint32_t selection_node() const { return guest::LoadU32(base, address + 9768); }
  void set_selection_node(uint32_t v) const { guest::StoreU32(base, address + 9768, v); }
  uint32_t selection_node_address() const { return address + 9768; }
  // 0x82637020 (an array in .data ShowSelection reads)
  uint32_t selection_array() const { return guest::LoadU32(base, address + 9776); }
  void set_selection_array(uint32_t v) const { guest::StoreU32(base, address + 9776, v); }
  uint32_t selection_array_address() const { return address + 9776; }
  // 2
  uint32_t selection_entries() const { return guest::LoadU32(base, address + 9784); }
  void set_selection_entries(uint32_t v) const { guest::StoreU32(base, address + 9784, v); }
  uint32_t selection_entries_address() const { return address + 9784; }
  // 1
  uint32_t unknown_9788() const { return guest::LoadU32(base, address + 9788); }
  void set_unknown_9788(uint32_t v) const { guest::StoreU32(base, address + 9788, v); }
  uint32_t unknown_9788_address() const { return address + 9788; }
  // 0 (SetArrows: the node's +108 when on)
  uint32_t arrow_on_frame() const { return guest::LoadU32(base, address + 9800); }
  void set_arrow_on_frame(uint32_t v) const { guest::StoreU32(base, address + 9800, v); }
  uint32_t arrow_on_frame_address() const { return address + 9800; }
  // 1
  uint32_t arrow_off_frame() const { return guest::LoadU32(base, address + 9804); }
  void set_arrow_off_frame(uint32_t v) const { guest::StoreU32(base, address + 9804, v); }
  uint32_t arrow_off_frame_address() const { return address + 9804; }
  // 0
  uint32_t color_cursor_child() const { return guest::LoadU32(base, address + 9808); }
  void set_color_cursor_child(uint32_t v) const { guest::StoreU32(base, address + 9808, v); }
  uint32_t color_cursor_child_address() const { return address + 9808; }
  // 4
  uint32_t icons_child() const { return guest::LoadU32(base, address + 9812); }
  void set_icons_child(uint32_t v) const { guest::StoreU32(base, address + 9812, v); }
  uint32_t icons_child_address() const { return address + 9812; }
  // 0
  uint32_t scroll_text_child() const { return guest::LoadU32(base, address + 9816); }
  void set_scroll_text_child(uint32_t v) const { guest::StoreU32(base, address + 9816, v); }
  uint32_t scroll_text_child_address() const { return address + 9816; }
  // 7: hidden while the list is dimmed
  uint32_t prompt_node() const { return guest::LoadU32(base, address + 9820); }
  void set_prompt_node(uint32_t v) const { guest::StoreU32(base, address + 9820, v); }
  uint32_t prompt_node_address() const { return address + 9820; }
  // 1: under part_node_set, hidden while dimmed
  uint32_t part_node() const { return guest::LoadU32(base, address + 9824); }
  void set_part_node(uint32_t v) const { guest::StoreU32(base, address + 9824, v); }
  uint32_t part_node_address() const { return address + 9824; }
  // 0
  uint32_t item_cursor_child() const { return guest::LoadU32(base, address + 9828); }
  void set_item_cursor_child(uint32_t v) const { guest::StoreU32(base, address + 9828, v); }
  uint32_t item_cursor_child_address() const { return address + 9828; }
  // 143: played by Step0
  uint32_t opening_anim_c() const { return guest::LoadU32(base, address + 9832); }
  void set_opening_anim_c(uint32_t v) const { guest::StoreU32(base, address + 9832, v); }
  uint32_t opening_anim_c_address() const { return address + 9832; }
  // 144: Step2's
  uint32_t course_in_anim() const { return guest::LoadU32(base, address + 9848); }
  void set_course_in_anim(uint32_t v) const { guest::StoreU32(base, address + 9848, v); }
  uint32_t course_in_anim_address() const { return address + 9848; }
  // 145: Step3's
  uint32_t course_out_anim() const { return guest::LoadU32(base, address + 9852); }
  void set_course_out_anim(uint32_t v) const { guest::StoreU32(base, address + 9852, v); }
  uint32_t course_out_anim_address() const { return address + 9852; }
  // 146: replayed by Step0
  uint32_t opening_loop_a() const { return guest::LoadU32(base, address + 9856); }
  void set_opening_loop_a(uint32_t v) const { guest::StoreU32(base, address + 9856, v); }
  uint32_t opening_loop_a_address() const { return address + 9856; }
  // 0: replayed by Step0
  uint32_t opening_loop_b() const { return guest::LoadU32(base, address + 9860); }
  void set_opening_loop_b(uint32_t v) const { guest::StoreU32(base, address + 9860, v); }
  uint32_t opening_loop_b_address() const { return address + 9860; }
  // -1 here: played by Step0
  uint32_t opening_anim_a() const { return guest::LoadU32(base, address + 9864); }
  void set_opening_anim_a(uint32_t v) const { guest::StoreU32(base, address + 9864, v); }
  uint32_t opening_anim_a_address() const { return address + 9864; }
  // 60: replayed once Step0 is done
  uint32_t opened_loop_a() const { return guest::LoadU32(base, address + 9868); }
  void set_opened_loop_a(uint32_t v) const { guest::StoreU32(base, address + 9868, v); }
  uint32_t opened_loop_a_address() const { return address + 9868; }
  // 148: replayed once Step2 is done
  uint32_t course_loop() const { return guest::LoadU32(base, address + 9872); }
  void set_course_loop(uint32_t v) const { guest::StoreU32(base, address + 9872, v); }
  uint32_t course_loop_address() const { return address + 9872; }
  // 5
  uint32_t opened_loop_b() const { return guest::LoadU32(base, address + 9876); }
  void set_opened_loop_b(uint32_t v) const { guest::StoreU32(base, address + 9876, v); }
  uint32_t opened_loop_b_address() const { return address + 9876; }
  // -1
  uint32_t opened_loop_c() const { return guest::LoadU32(base, address + 9880); }
  void set_opened_loop_c(uint32_t v) const { guest::StoreU32(base, address + 9880, v); }
  uint32_t opened_loop_c_address() const { return address + 9880; }
  // 4: played and looped by Step2, finished by Step3
  uint32_t course_select_anim() const { return guest::LoadU32(base, address + 9884); }
  void set_course_select_anim(uint32_t v) const { guest::StoreU32(base, address + 9884, v); }
  uint32_t course_select_anim_address() const { return address + 9884; }
  // 4: played by Step0
  uint32_t opening_anim_b() const { return guest::LoadU32(base, address + 9888); }
  void set_opening_anim_b(uint32_t v) const { guest::StoreU32(base, address + 9888, v); }
  uint32_t opening_anim_b_address() const { return address + 9888; }
  // 6: the toggle at its selection 0
  uint32_t toggle_anim_on() const { return guest::LoadU32(base, address + 9892); }
  void set_toggle_anim_on(uint32_t v) const { guest::StoreU32(base, address + 9892, v); }
  uint32_t toggle_anim_on_address() const { return address + 9892; }
  // 7
  uint32_t toggle_anim_off() const { return guest::LoadU32(base, address + 9896); }
  void set_toggle_anim_off(uint32_t v) const { guest::StoreU32(base, address + 9896, v); }
  uint32_t toggle_anim_off_address() const { return address + 9896; }
  // 65: ShowSelection's
  uint32_t selection_anim() const { return guest::LoadU32(base, address + 9900); }
  void set_selection_anim(uint32_t v) const { guest::StoreU32(base, address + 9900, v); }
  uint32_t selection_anim_address() const { return address + 9900; }
  // 117
  uint32_t unknown_9904() const { return guest::LoadU32(base, address + 9904); }
  void set_unknown_9904(uint32_t v) const { guest::StoreU32(base, address + 9904, v); }
  uint32_t unknown_9904_address() const { return address + 9904; }
  // 118
  uint32_t unknown_9908() const { return guest::LoadU32(base, address + 9908); }
  void set_unknown_9908(uint32_t v) const { guest::StoreU32(base, address + 9908, v); }
  uint32_t unknown_9908_address() const { return address + 9908; }
  // 78: played once a step for an upward move
  uint32_t arrow_up_anim() const { return guest::LoadU32(base, address + 9912); }
  void set_arrow_up_anim(uint32_t v) const { guest::StoreU32(base, address + 9912, v); }
  uint32_t arrow_up_anim_address() const { return address + 9912; }
  // 79
  uint32_t arrow_down_anim() const { return guest::LoadU32(base, address + 9916); }
  void set_arrow_down_anim(uint32_t v) const { guest::StoreU32(base, address + 9916, v); }
  uint32_t arrow_down_anim_address() const { return address + 9916; }
  // 130
  uint32_t unknown_9928() const { return guest::LoadU32(base, address + 9928); }
  void set_unknown_9928(uint32_t v) const { guest::StoreU32(base, address + 9928, v); }
  uint32_t unknown_9928_address() const { return address + 9928; }
  // 129
  uint32_t unknown_9932() const { return guest::LoadU32(base, address + 9932); }
  void set_unknown_9932(uint32_t v) const { guest::StoreU32(base, address + 9932, v); }
  uint32_t unknown_9932_address() const { return address + 9932; }
  // the courses' named animations, 107 to 112 (one a course list item)
  uint32_t course_anims_address() const { return address + 9936; }
  // the kinds' named animations, 68 to 70 (one a kind list item)
  uint32_t kind_anims_address() const { return address + 9976; }
  uint32_t kind_anim0() const { return guest::LoadU32(base, address + 9976); }
  void set_kind_anim0(uint32_t v) const { guest::StoreU32(base, address + 9976, v); }
  uint32_t kind_anim0_address() const { return address + 9976; }
  uint32_t kind_anim1() const { return guest::LoadU32(base, address + 9980); }
  void set_kind_anim1(uint32_t v) const { guest::StoreU32(base, address + 9980, v); }
  uint32_t kind_anim1_address() const { return address + 9980; }
  uint32_t kind_anim2() const { return guest::LoadU32(base, address + 9984); }
  void set_kind_anim2(uint32_t v) const { guest::StoreU32(base, address + 9984, v); }
  uint32_t kind_anim2_address() const { return address + 9984; }
  // the race kind's MenuList (3 items; the race data's +32)
  uint32_t kind_list_address() const { return address + 10144; }
  // the kind list's +4
  uint32_t kind_list_scroll() const { return guest::LoadU32(base, address + 10148); }
  void set_kind_list_scroll(uint32_t v) const { guest::StoreU32(base, address + 10148, v); }
  uint32_t kind_list_scroll_address() const { return address + 10148; }
  // its +8
  uint32_t kind_list_scroll_before() const { return guest::LoadU32(base, address + 10152); }
  void set_kind_list_scroll_before(uint32_t v) const { guest::StoreU32(base, address + 10152, v); }
  uint32_t kind_list_scroll_before_address() const { return address + 10152; }
  // its +20
  uint32_t kind_list_selection_before() const { return guest::LoadU32(base, address + 10164); }
  void set_kind_list_selection_before(uint32_t v) const { guest::StoreU32(base, address + 10164, v); }
  uint32_t kind_list_selection_before_address() const { return address + 10164; }
  // its +24
  uint32_t kind_list_move() const { return guest::LoadU32(base, address + 10168); }
  void set_kind_list_move(uint32_t v) const { guest::StoreU32(base, address + 10168, v); }
  uint32_t kind_list_move_address() const { return address + 10168; }
  // the course's MenuList (8 items, 6 shown)
  uint32_t course_list_address() const { return address + 10216; }
  // its +4: the first course shown
  uint32_t course_list_scroll() const { return guest::LoadU32(base, address + 10220); }
  void set_course_list_scroll(uint32_t v) const { guest::StoreU32(base, address + 10220, v); }
  uint32_t course_list_scroll_address() const { return address + 10220; }
  // its +8
  uint32_t course_list_scroll_before() const { return guest::LoadU32(base, address + 10224); }
  void set_course_list_scroll_before(uint32_t v) const { guest::StoreU32(base, address + 10224, v); }
  uint32_t course_list_scroll_before_address() const { return address + 10224; }
  // its +24
  uint32_t course_list_move() const { return guest::LoadU32(base, address + 10240); }
  void set_course_list_move(uint32_t v) const { guest::StoreU32(base, address + 10240, v); }
  uint32_t course_list_move_address() const { return address + 10240; }
  // the traffic toggle's MenuList (2 items; the first: traffic 3)
  uint32_t toggle_address() const { return address + 10288; }
  // its +24
  uint32_t toggle_move() const { return guest::LoadU32(base, address + 10312); }
  void set_toggle_move(uint32_t v) const { guest::StoreU32(base, address + 10312, v); }
  uint32_t toggle_move_address() const { return address + 10312; }
  // the PairedNodeTable (ListNodeTable); the measure meets its vtable's slot 1 (Type8NodeTableVtable)
  uint32_t node_table_object_address() const { return address + 10360; }
  // the kinds' scroll texts, 545 to 547
  uint32_t kind_texts_address() const { return address + 10820; }
  // 548
  uint32_t unknown_10832() const { return guest::LoadU32(base, address + 10832); }
  void set_unknown_10832(uint32_t v) const { guest::StoreU32(base, address + 10832, v); }
  uint32_t unknown_10832_address() const { return address + 10832; }
  // 549: the course list's scroll text
  uint32_t course_text() const { return guest::LoadU32(base, address + 10836); }
  void set_course_text(uint32_t v) const { guest::StoreU32(base, address + 10836, v); }
  uint32_t course_text_address() const { return address + 10836; }
  // -1 in the base, 10 in the screen: the list part's scroll text
  uint32_t part_text() const { return guest::LoadU32(base, address + 10840); }
  void set_part_text(uint32_t v) const { guest::StoreU32(base, address + 10840, v); }
  uint32_t part_text_address() const { return address + 10840; }
  // the Type8ListPart
  uint32_t list_part_address() const { return address + 10844; }
};

// embedded; measure: Type8ColorListScreen
struct Type8ColorListScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 11828;
  static constexpr uint32_t kVtable = 0x82010CF0;
  // 0x82010CF0
  template <typename T = Type8ScreenVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the machine's state (10: leave, SetUpRace)
  uint32_t state() const { return guest::LoadU32(base, address + 9608); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t state_address() const { return address + 9608; }
  // the color node table (+10432)
  template <typename T = ColorNodeTable>
  T node_table() const { return T(base, guest::LoadU32(base, address + 9672)); }
  void set_node_table(const View& v) const { guest::StoreU32(base, address + 9672, v.address); }
  void set_node_table(uint32_t v) const { guest::StoreU32(base, address + 9672, v); }
  uint32_t node_table_address() const { return address + 9672; }
  // the list part (+11040)
  template <typename T = Type8ColorListPart>
  T part() const { return T(base, guest::LoadU32(base, address + 9676)); }
  void set_part(const View& v) const { guest::StoreU32(base, address + 9676, v.address); }
  void set_part(uint32_t v) const { guest::StoreU32(base, address + 9676, v); }
  uint32_t part_address() const { return address + 9676; }
  uint32_t movie_id() const { return guest::LoadU32(base, address + 9680); }
  void set_movie_id(uint32_t v) const { guest::StoreU32(base, address + 9680, v); }
  uint32_t movie_id_address() const { return address + 9680; }
  // 0 in the constructor
  uint32_t unknown_9684() const { return guest::LoadU32(base, address + 9684); }
  void set_unknown_9684(uint32_t v) const { guest::StoreU32(base, address + 9684, v); }
  uint32_t unknown_9684_address() const { return address + 9684; }
  // 1
  uint32_t show_part_nodes() const { return guest::LoadU32(base, address + 9708); }
  void set_show_part_nodes(uint32_t v) const { guest::StoreU32(base, address + 9708, v); }
  uint32_t show_part_nodes_address() const { return address + 9708; }
  // 0
  uint32_t unknown_9712() const { return guest::LoadU32(base, address + 9712); }
  void set_unknown_9712(uint32_t v) const { guest::StoreU32(base, address + 9712, v); }
  uint32_t unknown_9712_address() const { return address + 9712; }
  // 10
  uint32_t unknown_9716() const { return guest::LoadU32(base, address + 9716); }
  void set_unknown_9716(uint32_t v) const { guest::StoreU32(base, address + 9716, v); }
  uint32_t unknown_9716_address() const { return address + 9716; }
  // 11
  uint32_t unknown_9720() const { return guest::LoadU32(base, address + 9720); }
  void set_unknown_9720(uint32_t v) const { guest::StoreU32(base, address + 9720, v); }
  uint32_t unknown_9720_address() const { return address + 9720; }
  // 16
  uint32_t selection_node_set() const { return guest::LoadU32(base, address + 9724); }
  void set_selection_node_set(uint32_t v) const { guest::StoreU32(base, address + 9724, v); }
  uint32_t selection_node_set_address() const { return address + 9724; }
  // 6
  uint32_t part_node_set() const { return guest::LoadU32(base, address + 9728); }
  void set_part_node_set(uint32_t v) const { guest::StoreU32(base, address + 9728, v); }
  uint32_t part_node_set_address() const { return address + 9728; }
  // 2
  uint32_t icons_node_set() const { return guest::LoadU32(base, address + 9732); }
  void set_icons_node_set(uint32_t v) const { guest::StoreU32(base, address + 9732, v); }
  uint32_t icons_node_set_address() const { return address + 9732; }
  // 9
  uint32_t color_cursor_node_set() const { return guest::LoadU32(base, address + 9736); }
  void set_color_cursor_node_set(uint32_t v) const { guest::StoreU32(base, address + 9736, v); }
  uint32_t color_cursor_node_set_address() const { return address + 9736; }
  // 1
  uint32_t scroll_text_node_set() const { return guest::LoadU32(base, address + 9740); }
  void set_scroll_text_node_set(uint32_t v) const { guest::StoreU32(base, address + 9740, v); }
  uint32_t scroll_text_node_set_address() const { return address + 9740; }
  // 21: SetItemText's texts
  uint32_t item_node_set() const { return guest::LoadU32(base, address + 9744); }
  void set_item_node_set(uint32_t v) const { guest::StoreU32(base, address + 9744, v); }
  uint32_t item_node_set_address() const { return address + 9744; }
  // 0
  uint32_t marker_node() const { return guest::LoadU32(base, address + 9748); }
  void set_marker_node(uint32_t v) const { guest::StoreU32(base, address + 9748, v); }
  uint32_t marker_node_address() const { return address + 9748; }
  // 8
  uint32_t arrow_a_node() const { return guest::LoadU32(base, address + 9752); }
  void set_arrow_a_node(uint32_t v) const { guest::StoreU32(base, address + 9752, v); }
  uint32_t arrow_a_node_address() const { return address + 9752; }
  // 9
  uint32_t arrow_b_node() const { return guest::LoadU32(base, address + 9756); }
  void set_arrow_b_node(uint32_t v) const { guest::StoreU32(base, address + 9756, v); }
  uint32_t arrow_b_node_address() const { return address + 9756; }
  // 3
  uint32_t selection_node() const { return guest::LoadU32(base, address + 9768); }
  void set_selection_node(uint32_t v) const { guest::StoreU32(base, address + 9768, v); }
  uint32_t selection_node_address() const { return address + 9768; }
  // 0x8263700C
  uint32_t selection_array() const { return guest::LoadU32(base, address + 9776); }
  void set_selection_array(uint32_t v) const { guest::StoreU32(base, address + 9776, v); }
  uint32_t selection_array_address() const { return address + 9776; }
  // 2
  uint32_t selection_entries() const { return guest::LoadU32(base, address + 9784); }
  void set_selection_entries(uint32_t v) const { guest::StoreU32(base, address + 9784, v); }
  uint32_t selection_entries_address() const { return address + 9784; }
  // 1
  uint32_t unknown_9788() const { return guest::LoadU32(base, address + 9788); }
  void set_unknown_9788(uint32_t v) const { guest::StoreU32(base, address + 9788, v); }
  uint32_t unknown_9788_address() const { return address + 9788; }
  // 0
  uint32_t arrow_on_frame() const { return guest::LoadU32(base, address + 9800); }
  void set_arrow_on_frame(uint32_t v) const { guest::StoreU32(base, address + 9800, v); }
  uint32_t arrow_on_frame_address() const { return address + 9800; }
  // 1
  uint32_t arrow_off_frame() const { return guest::LoadU32(base, address + 9804); }
  void set_arrow_off_frame(uint32_t v) const { guest::StoreU32(base, address + 9804, v); }
  uint32_t arrow_off_frame_address() const { return address + 9804; }
  // 0
  uint32_t color_cursor_child() const { return guest::LoadU32(base, address + 9808); }
  void set_color_cursor_child(uint32_t v) const { guest::StoreU32(base, address + 9808, v); }
  uint32_t color_cursor_child_address() const { return address + 9808; }
  // 4
  uint32_t icons_child() const { return guest::LoadU32(base, address + 9812); }
  void set_icons_child(uint32_t v) const { guest::StoreU32(base, address + 9812, v); }
  uint32_t icons_child_address() const { return address + 9812; }
  // 0
  uint32_t scroll_text_child() const { return guest::LoadU32(base, address + 9816); }
  void set_scroll_text_child(uint32_t v) const { guest::StoreU32(base, address + 9816, v); }
  uint32_t scroll_text_child_address() const { return address + 9816; }
  // 7
  uint32_t prompt_node() const { return guest::LoadU32(base, address + 9820); }
  void set_prompt_node(uint32_t v) const { guest::StoreU32(base, address + 9820, v); }
  uint32_t prompt_node_address() const { return address + 9820; }
  // 1
  uint32_t part_node() const { return guest::LoadU32(base, address + 9824); }
  void set_part_node(uint32_t v) const { guest::StoreU32(base, address + 9824, v); }
  uint32_t part_node_address() const { return address + 9824; }
  // 0
  uint32_t item_cursor_child() const { return guest::LoadU32(base, address + 9828); }
  void set_item_cursor_child(uint32_t v) const { guest::StoreU32(base, address + 9828, v); }
  uint32_t item_cursor_child_address() const { return address + 9828; }
  // 165, a named animation
  uint32_t unknown_9832() const { return guest::LoadU32(base, address + 9832); }
  void set_unknown_9832(uint32_t v) const { guest::StoreU32(base, address + 9832, v); }
  uint32_t unknown_9832_address() const { return address + 9832; }
  // 166
  uint32_t unknown_9840() const { return guest::LoadU32(base, address + 9840); }
  void set_unknown_9840(uint32_t v) const { guest::StoreU32(base, address + 9840, v); }
  uint32_t unknown_9840_address() const { return address + 9840; }
  // 168
  uint32_t unknown_9844() const { return guest::LoadU32(base, address + 9844); }
  void set_unknown_9844(uint32_t v) const { guest::StoreU32(base, address + 9844, v); }
  uint32_t unknown_9844_address() const { return address + 9844; }
  // 167
  uint32_t unknown_9848() const { return guest::LoadU32(base, address + 9848); }
  void set_unknown_9848(uint32_t v) const { guest::StoreU32(base, address + 9848, v); }
  uint32_t unknown_9848_address() const { return address + 9848; }
  // 169
  uint32_t unknown_9852() const { return guest::LoadU32(base, address + 9852); }
  void set_unknown_9852(uint32_t v) const { guest::StoreU32(base, address + 9852, v); }
  uint32_t unknown_9852_address() const { return address + 9852; }
  // 170
  uint32_t unknown_9856() const { return guest::LoadU32(base, address + 9856); }
  void set_unknown_9856(uint32_t v) const { guest::StoreU32(base, address + 9856, v); }
  uint32_t unknown_9856_address() const { return address + 9856; }
  // 0
  uint32_t unknown_9860() const { return guest::LoadU32(base, address + 9860); }
  void set_unknown_9860(uint32_t v) const { guest::StoreU32(base, address + 9860, v); }
  uint32_t unknown_9860_address() const { return address + 9860; }
  // -1
  uint32_t unknown_9864() const { return guest::LoadU32(base, address + 9864); }
  void set_unknown_9864(uint32_t v) const { guest::StoreU32(base, address + 9864, v); }
  uint32_t unknown_9864_address() const { return address + 9864; }
  // 47
  uint32_t unknown_9868() const { return guest::LoadU32(base, address + 9868); }
  void set_unknown_9868(uint32_t v) const { guest::StoreU32(base, address + 9868, v); }
  uint32_t unknown_9868_address() const { return address + 9868; }
  // 171
  uint32_t unknown_9872() const { return guest::LoadU32(base, address + 9872); }
  void set_unknown_9872(uint32_t v) const { guest::StoreU32(base, address + 9872, v); }
  uint32_t unknown_9872_address() const { return address + 9872; }
  // 5
  uint32_t unknown_9876() const { return guest::LoadU32(base, address + 9876); }
  void set_unknown_9876(uint32_t v) const { guest::StoreU32(base, address + 9876, v); }
  uint32_t unknown_9876_address() const { return address + 9876; }
  // -1
  uint32_t unknown_9888() const { return guest::LoadU32(base, address + 9888); }
  void set_unknown_9888(uint32_t v) const { guest::StoreU32(base, address + 9888, v); }
  uint32_t unknown_9888_address() const { return address + 9888; }
  // 6
  uint32_t toggle_anim_on() const { return guest::LoadU32(base, address + 9892); }
  void set_toggle_anim_on(uint32_t v) const { guest::StoreU32(base, address + 9892, v); }
  uint32_t toggle_anim_on_address() const { return address + 9892; }
  // 7
  uint32_t toggle_anim_off() const { return guest::LoadU32(base, address + 9896); }
  void set_toggle_anim_off(uint32_t v) const { guest::StoreU32(base, address + 9896, v); }
  uint32_t toggle_anim_off_address() const { return address + 9896; }
  // 87
  uint32_t selection_anim() const { return guest::LoadU32(base, address + 9900); }
  void set_selection_anim(uint32_t v) const { guest::StoreU32(base, address + 9900, v); }
  uint32_t selection_anim_address() const { return address + 9900; }
  // 139
  uint32_t unknown_9904() const { return guest::LoadU32(base, address + 9904); }
  void set_unknown_9904(uint32_t v) const { guest::StoreU32(base, address + 9904, v); }
  uint32_t unknown_9904_address() const { return address + 9904; }
  // 140
  uint32_t unknown_9908() const { return guest::LoadU32(base, address + 9908); }
  void set_unknown_9908(uint32_t v) const { guest::StoreU32(base, address + 9908, v); }
  uint32_t unknown_9908_address() const { return address + 9908; }
  // 100
  uint32_t arrow_up_anim() const { return guest::LoadU32(base, address + 9912); }
  void set_arrow_up_anim(uint32_t v) const { guest::StoreU32(base, address + 9912, v); }
  uint32_t arrow_up_anim_address() const { return address + 9912; }
  // 101
  uint32_t arrow_down_anim() const { return guest::LoadU32(base, address + 9916); }
  void set_arrow_down_anim(uint32_t v) const { guest::StoreU32(base, address + 9916, v); }
  uint32_t arrow_down_anim_address() const { return address + 9916; }
  // 20
  uint32_t unknown_9920() const { return guest::LoadU32(base, address + 9920); }
  void set_unknown_9920(uint32_t v) const { guest::StoreU32(base, address + 9920, v); }
  uint32_t unknown_9920_address() const { return address + 9920; }
  // 21
  uint32_t unknown_9924() const { return guest::LoadU32(base, address + 9924); }
  void set_unknown_9924(uint32_t v) const { guest::StoreU32(base, address + 9924, v); }
  uint32_t unknown_9924_address() const { return address + 9924; }
  // 152
  uint32_t unknown_9928() const { return guest::LoadU32(base, address + 9928); }
  void set_unknown_9928(uint32_t v) const { guest::StoreU32(base, address + 9928, v); }
  uint32_t unknown_9928_address() const { return address + 9928; }
  // 151
  uint32_t unknown_9932() const { return guest::LoadU32(base, address + 9932); }
  void set_unknown_9932(uint32_t v) const { guest::StoreU32(base, address + 9932, v); }
  uint32_t unknown_9932_address() const { return address + 9932; }
  // the item list's named animations (129 to 131)
  uint32_t item_anims_address() const { return address + 9936; }
  // 90 to 92
  uint32_t second_anims_address() const { return address + 9976; }
  // 10 to 12
  uint32_t third_anims_address() const { return address + 10016; }
  // 141
  uint32_t unknown_10056() const { return guest::LoadU32(base, address + 10056); }
  void set_unknown_10056(uint32_t v) const { guest::StoreU32(base, address + 10056, v); }
  uint32_t unknown_10056_address() const { return address + 10056; }
  // 142
  uint32_t unknown_10060() const { return guest::LoadU32(base, address + 10060); }
  void set_unknown_10060(uint32_t v) const { guest::StoreU32(base, address + 10060, v); }
  uint32_t unknown_10060_address() const { return address + 10060; }
  // 143
  uint32_t unknown_10064() const { return guest::LoadU32(base, address + 10064); }
  void set_unknown_10064(uint32_t v) const { guest::StoreU32(base, address + 10064, v); }
  uint32_t unknown_10064_address() const { return address + 10064; }
  // 155
  uint32_t unknown_10096() const { return guest::LoadU32(base, address + 10096); }
  void set_unknown_10096(uint32_t v) const { guest::StoreU32(base, address + 10096, v); }
  uint32_t unknown_10096_address() const { return address + 10096; }
  // 156
  uint32_t unknown_10100() const { return guest::LoadU32(base, address + 10100); }
  void set_unknown_10100(uint32_t v) const { guest::StoreU32(base, address + 10100, v); }
  uint32_t unknown_10100_address() const { return address + 10100; }
  // 157
  uint32_t unknown_10104() const { return guest::LoadU32(base, address + 10104); }
  void set_unknown_10104(uint32_t v) const { guest::StoreU32(base, address + 10104, v); }
  uint32_t unknown_10104_address() const { return address + 10104; }
  // the item list's MenuList (9 items, 3 shown)
  uint32_t item_list_address() const { return address + 10144; }
  // its +4
  uint32_t item_list_scroll() const { return guest::LoadU32(base, address + 10148); }
  void set_item_list_scroll(uint32_t v) const { guest::StoreU32(base, address + 10148, v); }
  uint32_t item_list_scroll_address() const { return address + 10148; }
  // the course list's MenuList (2 items)
  uint32_t course_list_address() const { return address + 10216; }
  // the traffic toggle's MenuList
  uint32_t toggle_address() const { return address + 10288; }
  // the race kind's MenuList (3 items)
  uint32_t kind_list_address() const { return address + 10360; }
  // the ColorNodeTable (ListColorNodeTable)
  uint32_t color_node_table_address() const { return address + 10432; }
  // the table's +8 (12)
  uint32_t table_node_set_a() const { return guest::LoadU32(base, address + 10440); }
  void set_table_node_set_a(uint32_t v) const { guest::StoreU32(base, address + 10440, v); }
  uint32_t table_node_set_a_address() const { return address + 10440; }
  // its +12 (11)
  uint32_t table_node_set_b() const { return guest::LoadU32(base, address + 10444); }
  void set_table_node_set_b(uint32_t v) const { guest::StoreU32(base, address + 10444, v); }
  uint32_t table_node_set_b_address() const { return address + 10444; }
  // its +16: 48 + the item
  uint32_t table_item_anims_address() const { return address + 10448; }
  // its +156: 1 + the item
  uint32_t table_item_nodes_address() const { return address + 10588; }
  // its +312: the item records' color masks
  uint32_t table_item_masks_address() const { return address + 10744; }
  // its +420 (5)
  uint32_t table_node_base_a() const { return guest::LoadU32(base, address + 10852); }
  void set_table_node_base_a(uint32_t v) const { guest::StoreU32(base, address + 10852, v); }
  uint32_t table_node_base_a_address() const { return address + 10852; }
  // its +424 (20)
  uint32_t table_node_base_b() const { return guest::LoadU32(base, address + 10856); }
  void set_table_node_base_b(uint32_t v) const { guest::StoreU32(base, address + 10856, v); }
  uint32_t table_node_base_b_address() const { return address + 10856; }
  // its +480: 79, 76, 77, 78, 80, 81 at +484 on
  uint32_t table_grid_a_address() const { return address + 10912; }
  uint32_t table_grid_a0() const { return guest::LoadU32(base, address + 10912); }
  void set_table_grid_a0(uint32_t v) const { guest::StoreU32(base, address + 10912, v); }
  uint32_t table_grid_a0_address() const { return address + 10912; }
  uint32_t table_grid_a1() const { return guest::LoadU32(base, address + 10916); }
  void set_table_grid_a1(uint32_t v) const { guest::StoreU32(base, address + 10916, v); }
  uint32_t table_grid_a1_address() const { return address + 10916; }
  uint32_t table_grid_a2() const { return guest::LoadU32(base, address + 10920); }
  void set_table_grid_a2(uint32_t v) const { guest::StoreU32(base, address + 10920, v); }
  uint32_t table_grid_a2_address() const { return address + 10920; }
  uint32_t table_grid_a3() const { return guest::LoadU32(base, address + 10924); }
  void set_table_grid_a3(uint32_t v) const { guest::StoreU32(base, address + 10924, v); }
  uint32_t table_grid_a3_address() const { return address + 10924; }
  uint32_t table_grid_a4() const { return guest::LoadU32(base, address + 10928); }
  void set_table_grid_a4(uint32_t v) const { guest::StoreU32(base, address + 10928, v); }
  uint32_t table_grid_a4_address() const { return address + 10928; }
  uint32_t table_grid_a5() const { return guest::LoadU32(base, address + 10932); }
  void set_table_grid_a5(uint32_t v) const { guest::StoreU32(base, address + 10932, v); }
  uint32_t table_grid_a5_address() const { return address + 10932; }
  uint32_t table_grid_a6() const { return guest::LoadU32(base, address + 10936); }
  void set_table_grid_a6(uint32_t v) const { guest::StoreU32(base, address + 10936, v); }
  uint32_t table_grid_a6_address() const { return address + 10936; }
  uint32_t table_grid_a7() const { return guest::LoadU32(base, address + 10940); }
  void set_table_grid_a7(uint32_t v) const { guest::StoreU32(base, address + 10940, v); }
  uint32_t table_grid_a7_address() const { return address + 10940; }
  uint32_t table_grid_a8() const { return guest::LoadU32(base, address + 10944); }
  void set_table_grid_a8(uint32_t v) const { guest::StoreU32(base, address + 10944, v); }
  uint32_t table_grid_a8_address() const { return address + 10944; }
  // its +516: 67, 64, 65, 66, 68, 69 at +520 on
  uint32_t table_grid_b_address() const { return address + 10948; }
  uint32_t table_grid_b0() const { return guest::LoadU32(base, address + 10948); }
  void set_table_grid_b0(uint32_t v) const { guest::StoreU32(base, address + 10948, v); }
  uint32_t table_grid_b0_address() const { return address + 10948; }
  uint32_t table_grid_b1() const { return guest::LoadU32(base, address + 10952); }
  void set_table_grid_b1(uint32_t v) const { guest::StoreU32(base, address + 10952, v); }
  uint32_t table_grid_b1_address() const { return address + 10952; }
  uint32_t table_grid_b2() const { return guest::LoadU32(base, address + 10956); }
  void set_table_grid_b2(uint32_t v) const { guest::StoreU32(base, address + 10956, v); }
  uint32_t table_grid_b2_address() const { return address + 10956; }
  uint32_t table_grid_b3() const { return guest::LoadU32(base, address + 10960); }
  void set_table_grid_b3(uint32_t v) const { guest::StoreU32(base, address + 10960, v); }
  uint32_t table_grid_b3_address() const { return address + 10960; }
  uint32_t table_grid_b4() const { return guest::LoadU32(base, address + 10964); }
  void set_table_grid_b4(uint32_t v) const { guest::StoreU32(base, address + 10964, v); }
  uint32_t table_grid_b4_address() const { return address + 10964; }
  uint32_t table_grid_b5() const { return guest::LoadU32(base, address + 10968); }
  void set_table_grid_b5(uint32_t v) const { guest::StoreU32(base, address + 10968, v); }
  uint32_t table_grid_b5_address() const { return address + 10968; }
  uint32_t table_grid_b6() const { return guest::LoadU32(base, address + 10972); }
  void set_table_grid_b6(uint32_t v) const { guest::StoreU32(base, address + 10972, v); }
  uint32_t table_grid_b6_address() const { return address + 10972; }
  uint32_t table_grid_b7() const { return guest::LoadU32(base, address + 10976); }
  void set_table_grid_b7(uint32_t v) const { guest::StoreU32(base, address + 10976, v); }
  uint32_t table_grid_b7_address() const { return address + 10976; }
  uint32_t table_grid_b8() const { return guest::LoadU32(base, address + 10980); }
  void set_table_grid_b8(uint32_t v) const { guest::StoreU32(base, address + 10980, v); }
  uint32_t table_grid_b8_address() const { return address + 10980; }
  // 13: the list part's scroll text
  uint32_t part_text() const { return guest::LoadU32(base, address + 11036); }
  void set_part_text(uint32_t v) const { guest::StoreU32(base, address + 11036, v); }
  uint32_t part_text_address() const { return address + 11036; }
  // the Type8ColorListPart
  uint32_t list_part_address() const { return address + 11040; }
};

// image; measure: Type8ListBase.+0, Type8ListScreen.+0, Type8ColorListScreen.+0
struct Type8ScreenVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 116;
  // slot 2
  uint32_t set_item_texts() const { return guest::LoadU32(base, address + 8); }
  void set_set_item_texts(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t set_item_texts_address() const { return address + 8; }
  // slot 26: the list screen's ShowMarks, the color list's SetDetails
  uint32_t slot_26() const { return guest::LoadU32(base, address + 104); }
  void set_slot_26(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t slot_26_address() const { return address + 104; }
  // slot 27: the list screen's SetLabels
  uint32_t slot_27() const { return guest::LoadU32(base, address + 108); }
  void set_slot_27(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t slot_27_address() const { return address + 108; }
  // slot 28: the color list's SetLabels
  uint32_t slot_28() const { return guest::LoadU32(base, address + 112); }
  void set_slot_28(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t slot_28_address() const { return address + 112; }
};

// image; measure: Type8ListBase.+10360
struct Type8NodeTableVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t deleting_destructor() const { return guest::LoadU32(base, address + 0); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t deleting_destructor_address() const { return address + 0; }
  // slot 1, called by Step2 before PairedNodeTable::Refresh
  uint32_t slot_1() const { return guest::LoadU32(base, address + 4); }
  void set_slot_1(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t slot_1_address() const { return address + 4; }
};

// embedded; measure: Type8ListScreen::ItemInfo.r5
struct Type8CourseInfo : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // the node table's +444
  uint32_t third_pair() const { return guest::LoadU32(base, address + 0); }
  void set_third_pair(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t third_pair_address() const { return address + 0; }
  // its +428
  uint32_t first_pair() const { return guest::LoadU32(base, address + 4); }
  void set_first_pair(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t first_pair_address() const { return address + 4; }
  // its +436
  uint32_t second_pair() const { return guest::LoadU32(base, address + 8); }
  void set_second_pair(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t second_pair_address() const { return address + 8; }
  // its +452 (bit 0: NodeListScreen's +9700 and +9704)
  uint32_t fourth_pair() const { return guest::LoadU32(base, address + 12); }
  void set_fourth_pair(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t fourth_pair_address() const { return address + 12; }
};

// embedded; measure: Type8ListPart, Type8ListBase.+9676
struct Type8ListPart : View {
  using View::View;
  static constexpr uint32_t kSize = 524;
  static constexpr uint32_t kVtable = 0x820114AC;
  // 0x820114AC
  template <typename T = Type8ListPartVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the prompt's yes
  uint32_t yes() const { return guest::LoadU32(base, address + 4); }
  void set_yes(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t yes_address() const { return address + 4; }
  // the prompt's back
  uint32_t back() const { return guest::LoadU32(base, address + 8); }
  void set_back(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t back_address() const { return address + 8; }
  // its UI movie's id (20)
  uint32_t movie_id() const { return guest::LoadU32(base, address + 12); }
  void set_movie_id(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t movie_id_address() const { return address + 12; }
  // +20
  template <typename T = Type8ListPartInner>
  T inner_part() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_inner_part(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_inner_part(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t inner_part_address() const { return address + 16; }
  // the Type8ListPartInner
  uint32_t inner_address() const { return address + 20; }
};

// embedded; measure: Type8ColorListPart
struct Type8ColorListPart : View {
  using View::View;
  static constexpr uint32_t kSize = 788;
  static constexpr uint32_t kVtable = 0x820113F4;
  // 0x820113F4
  template <typename T = Type8ListPartVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t yes() const { return guest::LoadU32(base, address + 4); }
  void set_yes(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t yes_address() const { return address + 4; }
  uint32_t back() const { return guest::LoadU32(base, address + 8); }
  void set_back(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t back_address() const { return address + 8; }
  // 20
  uint32_t movie_id() const { return guest::LoadU32(base, address + 12); }
  void set_movie_id(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t movie_id_address() const { return address + 12; }
  // +20
  template <typename T = Type8ColorPartInner>
  T inner_part() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_inner_part(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_inner_part(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t inner_part_address() const { return address + 16; }
  // the Type8ColorPartInner
  uint32_t inner_address() const { return address + 20; }
};

// image; measure: Type8ListBase.+9676.+0, Type8ColorListScreen.+11040
struct Type8ListPartVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 52;
  // slot 1 (ListPartBase::Open)
  uint32_t open() const { return guest::LoadU32(base, address + 4); }
  void set_open(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t open_address() const { return address + 4; }
  // slot 2 (ListPartBase::Pick)
  uint32_t pick() const { return guest::LoadU32(base, address + 8); }
  void set_pick(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t pick_address() const { return address + 8; }
  // slot 11 (Type8ColorListPart::IsFirstChoice)
  uint32_t is_first_choice() const { return guest::LoadU32(base, address + 44); }
  void set_is_first_choice(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t is_first_choice_address() const { return address + 44; }
  // slot 12 (Type8ColorListPart::ThirdChoice)
  uint32_t third_choice() const { return guest::LoadU32(base, address + 48); }
  void set_third_choice(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t third_choice_address() const { return address + 48; }
};

// embedded; measure: Type8PartInner
struct Type8PartInner : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // -1 while a step runs
  uint32_t page() const { return guest::LoadU32(base, address + 4); }
  void set_page(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t page_address() const { return address + 4; }
  uint32_t next_page() const { return guest::LoadU32(base, address + 8); }
  void set_next_page(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t next_page_address() const { return address + 8; }
  uint32_t previous_page() const { return guest::LoadU32(base, address + 12); }
  void set_previous_page(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t previous_page_address() const { return address + 12; }
  // -1 none
  uint32_t step() const { return guest::LoadU32(base, address + 16); }
  void set_step(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t step_address() const { return address + 16; }
  uint32_t queued_step() const { return guest::LoadU32(base, address + 20); }
  void set_queued_step(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t queued_step_address() const { return address + 20; }
  // -1 at Open, 0 when a queued step starts
  uint32_t unknown_24() const { return guest::LoadU32(base, address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  // the step's first-call word
  uint32_t step_started() const { return guest::LoadU32(base, address + 28); }
  void set_step_started(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t step_started_address() const { return address + 28; }
  uint32_t yes() const { return guest::LoadU32(base, address + 32); }
  void set_yes(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t yes_address() const { return address + 32; }
  uint32_t back() const { return guest::LoadU32(base, address + 36); }
  void set_back(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t back_address() const { return address + 36; }
};

// embedded; measure: Type8ListPartInner
struct Type8ListPartInner : View {
  using View::View;
  static constexpr uint32_t kSize = 504;
  static constexpr uint32_t kVtable = 0x82011490;
  // 0x82011490
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t page() const { return guest::LoadU32(base, address + 4); }
  void set_page(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t page_address() const { return address + 4; }
  uint32_t next_page() const { return guest::LoadU32(base, address + 8); }
  void set_next_page(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t next_page_address() const { return address + 8; }
  uint32_t previous_page() const { return guest::LoadU32(base, address + 12); }
  void set_previous_page(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t previous_page_address() const { return address + 12; }
  uint32_t step() const { return guest::LoadU32(base, address + 16); }
  void set_step(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t step_address() const { return address + 16; }
  uint32_t queued_step() const { return guest::LoadU32(base, address + 20); }
  void set_queued_step(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t queued_step_address() const { return address + 20; }
  uint32_t unknown_24() const { return guest::LoadU32(base, address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  uint32_t step_started() const { return guest::LoadU32(base, address + 28); }
  void set_step_started(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t step_started_address() const { return address + 28; }
  uint32_t yes() const { return guest::LoadU32(base, address + 32); }
  void set_yes(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t yes_address() const { return address + 32; }
  uint32_t back() const { return guest::LoadU32(base, address + 36); }
  void set_back(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t back_address() const { return address + 36; }
  // player 0's Type8PlayerBlock
  uint32_t block0_address() const { return address + 40; }
  uint32_t block0_ready() const { return guest::LoadU32(base, address + 44); }
  void set_block0_ready(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t block0_ready_address() const { return address + 44; }
  uint32_t block0_backed_out() const { return guest::LoadU32(base, address + 48); }
  void set_block0_backed_out(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t block0_backed_out_address() const { return address + 48; }
  // player 1's
  uint32_t block1_address() const { return address + 272; }
  uint32_t block1_ready() const { return guest::LoadU32(base, address + 276); }
  void set_block1_ready(uint32_t v) const { guest::StoreU32(base, address + 276, v); }
  uint32_t block1_ready_address() const { return address + 276; }
};

// embedded; measure: Type8ColorPartInner
struct Type8ColorPartInner : View {
  using View::View;
  static constexpr uint32_t kSize = 768;
  static constexpr uint32_t kVtable = 0x820113D8;
  // 0x820113D8
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t page() const { return guest::LoadU32(base, address + 4); }
  void set_page(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t page_address() const { return address + 4; }
  uint32_t next_page() const { return guest::LoadU32(base, address + 8); }
  void set_next_page(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t next_page_address() const { return address + 8; }
  uint32_t previous_page() const { return guest::LoadU32(base, address + 12); }
  void set_previous_page(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t previous_page_address() const { return address + 12; }
  uint32_t step() const { return guest::LoadU32(base, address + 16); }
  void set_step(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t step_address() const { return address + 16; }
  uint32_t queued_step() const { return guest::LoadU32(base, address + 20); }
  void set_queued_step(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t queued_step_address() const { return address + 20; }
  uint32_t unknown_24() const { return guest::LoadU32(base, address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  uint32_t step_started() const { return guest::LoadU32(base, address + 28); }
  void set_step_started(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t step_started_address() const { return address + 28; }
  uint32_t yes() const { return guest::LoadU32(base, address + 32); }
  void set_yes(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t yes_address() const { return address + 32; }
  uint32_t back() const { return guest::LoadU32(base, address + 36); }
  void set_back(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t back_address() const { return address + 36; }
  // player 0's Type8ColorBlock
  uint32_t block0_address() const { return address + 40; }
  uint32_t block0_ready() const { return guest::LoadU32(base, address + 44); }
  void set_block0_ready(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t block0_ready_address() const { return address + 44; }
  uint32_t block0_backed_out() const { return guest::LoadU32(base, address + 48); }
  void set_block0_backed_out(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t block0_backed_out_address() const { return address + 48; }
  // player 1's
  uint32_t block1_address() const { return address + 404; }
  uint32_t block1_ready() const { return guest::LoadU32(base, address + 408); }
  void set_block1_ready(uint32_t v) const { guest::StoreU32(base, address + 408, v); }
  uint32_t block1_ready_address() const { return address + 408; }
};

// embedded; measure: Type8PlayerBlock, Type8PlayerBlock.+228
struct Type8PlayerBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 232;
  // 0 or 1
  uint32_t player() const { return guest::LoadU32(base, address + 0); }
  void set_player(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t player_address() const { return address + 0; }
  // 0 picking, 1 ready (confirmed)
  uint32_t ready() const { return guest::LoadU32(base, address + 4); }
  void set_ready(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t ready_address() const { return address + 4; }
  uint32_t backed_out() const { return guest::LoadU32(base, address + 8); }
  void set_backed_out(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t backed_out_address() const { return address + 8; }
  // the item MenuList (item 1: ready)
  uint32_t menu_address() const { return address + 12; }
  // the menu's +20
  uint32_t menu_selection_before() const { return guest::LoadU32(base, address + 32); }
  void set_menu_selection_before(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t menu_selection_before_address() const { return address + 32; }
  // the menu's +24
  uint32_t menu_move() const { return guest::LoadU32(base, address + 36); }
  void set_menu_move(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t menu_move_address() const { return address + 36; }
  // the menu's +60: the player's pad
  uint32_t menu_player() const { return guest::LoadU32(base, address + 72); }
  void set_menu_player(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t menu_player_address() const { return address + 72; }
  // the menu's +68
  uint32_t menu_block() const { return guest::LoadU32(base, address + 80); }
  void set_menu_block(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t menu_block_address() const { return address + 80; }
  // the setting MenuList (2 items)
  uint32_t setting_address() const { return address + 84; }
  // its +20
  uint32_t setting_selection_before() const { return guest::LoadU32(base, address + 104); }
  void set_setting_selection_before(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t setting_selection_before_address() const { return address + 104; }
  // its +24
  uint32_t setting_move() const { return guest::LoadU32(base, address + 108); }
  void set_setting_move(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t setting_move_address() const { return address + 108; }
  // its +60
  uint32_t setting_player() const { return guest::LoadU32(base, address + 144); }
  void set_setting_player(uint32_t v) const { guest::StoreU32(base, address + 144, v); }
  uint32_t setting_player_address() const { return address + 144; }
  // its +68
  uint32_t setting_block() const { return guest::LoadU32(base, address + 152); }
  void set_setting_block(uint32_t v) const { guest::StoreU32(base, address + 152, v); }
  uint32_t setting_block_address() const { return address + 152; }
  uint32_t node_set() const { return guest::LoadU32(base, address + 156); }
  void set_node_set(uint32_t v) const { guest::StoreU32(base, address + 156, v); }
  uint32_t node_set_address() const { return address + 156; }
  // 5
  uint32_t node() const { return guest::LoadU32(base, address + 160); }
  void set_node(uint32_t v) const { guest::StoreU32(base, address + 160, v); }
  uint32_t node_address() const { return address + 160; }
  // 7
  uint32_t unknown_164() const { return guest::LoadU32(base, address + 164); }
  void set_unknown_164(uint32_t v) const { guest::StoreU32(base, address + 164, v); }
  uint32_t unknown_164_address() const { return address + 164; }
  // 6: SetUp's SetText
  uint32_t text_node() const { return guest::LoadU32(base, address + 168); }
  void set_text_node(uint32_t v) const { guest::StoreU32(base, address + 168, v); }
  uint32_t text_node_address() const { return address + 168; }
  // 10
  uint32_t text_child_a() const { return guest::LoadU32(base, address + 172); }
  void set_text_child_a(uint32_t v) const { guest::StoreU32(base, address + 172, v); }
  uint32_t text_child_a_address() const { return address + 172; }
  // 11
  uint32_t text_child_b() const { return guest::LoadU32(base, address + 176); }
  void set_text_child_b(uint32_t v) const { guest::StoreU32(base, address + 176, v); }
  uint32_t text_child_b_address() const { return address + 176; }
  // the menu's items' named animations
  uint32_t item_anims_address() const { return address + 180; }
  uint32_t item_anim0() const { return guest::LoadU32(base, address + 180); }
  void set_item_anim0(uint32_t v) const { guest::StoreU32(base, address + 180, v); }
  uint32_t item_anim0_address() const { return address + 180; }
  uint32_t item_anim1() const { return guest::LoadU32(base, address + 184); }
  void set_item_anim1(uint32_t v) const { guest::StoreU32(base, address + 184, v); }
  uint32_t item_anim1_address() const { return address + 184; }
  uint32_t unknown_188() const { return guest::LoadU32(base, address + 188); }
  void set_unknown_188(uint32_t v) const { guest::StoreU32(base, address + 188, v); }
  uint32_t unknown_188_address() const { return address + 188; }
  // played by PlayReady
  uint32_t ready_anim() const { return guest::LoadU32(base, address + 192); }
  void set_ready_anim(uint32_t v) const { guest::StoreU32(base, address + 192, v); }
  uint32_t ready_anim_address() const { return address + 192; }
  uint32_t opening_anim_a() const { return guest::LoadU32(base, address + 196); }
  void set_opening_anim_a(uint32_t v) const { guest::StoreU32(base, address + 196, v); }
  uint32_t opening_anim_a_address() const { return address + 196; }
  uint32_t opening_anim_b() const { return guest::LoadU32(base, address + 200); }
  void set_opening_anim_b(uint32_t v) const { guest::StoreU32(base, address + 200, v); }
  uint32_t opening_anim_b_address() const { return address + 200; }
  // the setting's items' named animations
  uint32_t setting_anims_address() const { return address + 204; }
  // also played by PlayOpening
  uint32_t setting_anim0() const { return guest::LoadU32(base, address + 204); }
  void set_setting_anim0(uint32_t v) const { guest::StoreU32(base, address + 204, v); }
  uint32_t setting_anim0_address() const { return address + 204; }
  uint32_t setting_anim1() const { return guest::LoadU32(base, address + 208); }
  void set_setting_anim1(uint32_t v) const { guest::StoreU32(base, address + 208, v); }
  uint32_t setting_anim1_address() const { return address + 208; }
  // restarted at item 1
  uint32_t setting_item_anim() const { return guest::LoadU32(base, address + 212); }
  void set_setting_item_anim(uint32_t v) const { guest::StoreU32(base, address + 212, v); }
  uint32_t setting_item_anim_address() const { return address + 212; }
  uint32_t selection_anim_a() const { return guest::LoadU32(base, address + 216); }
  void set_selection_anim_a(uint32_t v) const { guest::StoreU32(base, address + 216, v); }
  uint32_t selection_anim_a_address() const { return address + 216; }
  uint32_t selection_anim_b() const { return guest::LoadU32(base, address + 220); }
  void set_selection_anim_b(uint32_t v) const { guest::StoreU32(base, address + 220, v); }
  uint32_t selection_anim_b_address() const { return address + 220; }
  // 8: on (+108 = 1) at setting 0
  uint32_t cursor_child_a() const { return guest::LoadU32(base, address + 224); }
  void set_cursor_child_a(uint32_t v) const { guest::StoreU32(base, address + 224, v); }
  uint32_t cursor_child_a_address() const { return address + 224; }
  // 9: on at setting 1
  uint32_t cursor_child_b() const { return guest::LoadU32(base, address + 228); }
  void set_cursor_child_b(uint32_t v) const { guest::StoreU32(base, address + 228, v); }
  uint32_t cursor_child_b_address() const { return address + 228; }
};

// embedded; measure: Type8ColorBlock, Type8ColorBlock.+360
struct Type8ColorBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 364;
  uint32_t player() const { return guest::LoadU32(base, address + 0); }
  void set_player(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t player_address() const { return address + 0; }
  uint32_t ready() const { return guest::LoadU32(base, address + 4); }
  void set_ready(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t ready_address() const { return address + 4; }
  uint32_t backed_out() const { return guest::LoadU32(base, address + 8); }
  void set_backed_out(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t backed_out_address() const { return address + 8; }
  // the item MenuList (item 2: ready)
  uint32_t menu_address() const { return address + 12; }
  // the menu's +20
  uint32_t menu_selection_before() const { return guest::LoadU32(base, address + 32); }
  void set_menu_selection_before(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t menu_selection_before_address() const { return address + 32; }
  uint32_t menu_move() const { return guest::LoadU32(base, address + 36); }
  void set_menu_move(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t menu_move_address() const { return address + 36; }
  uint32_t menu_player() const { return guest::LoadU32(base, address + 72); }
  void set_menu_player(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t menu_player_address() const { return address + 72; }
  uint32_t menu_block() const { return guest::LoadU32(base, address + 80); }
  void set_menu_block(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t menu_block_address() const { return address + 80; }
  // the third choice's MenuList (item 0: IsFirstChoice)
  uint32_t third_address() const { return address + 84; }
  uint32_t third_selection_before() const { return guest::LoadU32(base, address + 104); }
  void set_third_selection_before(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t third_selection_before_address() const { return address + 104; }
  uint32_t third_move() const { return guest::LoadU32(base, address + 108); }
  void set_third_move(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t third_move_address() const { return address + 108; }
  uint32_t third_player() const { return guest::LoadU32(base, address + 144); }
  void set_third_player(uint32_t v) const { guest::StoreU32(base, address + 144, v); }
  uint32_t third_player_address() const { return address + 144; }
  uint32_t third_block() const { return guest::LoadU32(base, address + 152); }
  void set_third_block(uint32_t v) const { guest::StoreU32(base, address + 152, v); }
  uint32_t third_block_address() const { return address + 152; }
  // the second choice's MenuList (5 items, from 2; ThirdChoice reads it)
  uint32_t second_address() const { return address + 156; }
  uint32_t second_move() const { return guest::LoadU32(base, address + 180); }
  void set_second_move(uint32_t v) const { guest::StoreU32(base, address + 180, v); }
  uint32_t second_move_address() const { return address + 180; }
  uint32_t second_player() const { return guest::LoadU32(base, address + 216); }
  void set_second_player(uint32_t v) const { guest::StoreU32(base, address + 216, v); }
  uint32_t second_player_address() const { return address + 216; }
  uint32_t second_block() const { return guest::LoadU32(base, address + 224); }
  void set_second_block(uint32_t v) const { guest::StoreU32(base, address + 224, v); }
  uint32_t second_block_address() const { return address + 224; }
  uint32_t node_set() const { return guest::LoadU32(base, address + 228); }
  void set_node_set(uint32_t v) const { guest::StoreU32(base, address + 228, v); }
  uint32_t node_set_address() const { return address + 228; }
  // 5
  uint32_t cursor_node() const { return guest::LoadU32(base, address + 232); }
  void set_cursor_node(uint32_t v) const { guest::StoreU32(base, address + 232, v); }
  uint32_t cursor_node_address() const { return address + 232; }
  // 7
  uint32_t count_node() const { return guest::LoadU32(base, address + 236); }
  void set_count_node(uint32_t v) const { guest::StoreU32(base, address + 236, v); }
  uint32_t count_node_address() const { return address + 236; }
  // 3: PlaceCursorA's
  uint32_t cursor_a_child() const { return guest::LoadU32(base, address + 240); }
  void set_cursor_a_child(uint32_t v) const { guest::StoreU32(base, address + 240, v); }
  uint32_t cursor_a_child_address() const { return address + 240; }
  // 12: PlaceCursorB's (+68 = 0.0)
  uint32_t slider_child_a() const { return guest::LoadU32(base, address + 244); }
  void set_slider_child_a(uint32_t v) const { guest::StoreU32(base, address + 244, v); }
  uint32_t slider_child_a_address() const { return address + 244; }
  // 13
  uint32_t slider_child_b() const { return guest::LoadU32(base, address + 248); }
  void set_slider_child_b(uint32_t v) const { guest::StoreU32(base, address + 248, v); }
  uint32_t slider_child_b_address() const { return address + 248; }
  // 8: PlaceCursorB's (+108 = 1)
  uint32_t slider_frame_a() const { return guest::LoadU32(base, address + 252); }
  void set_slider_frame_a(uint32_t v) const { guest::StoreU32(base, address + 252, v); }
  uint32_t slider_frame_a_address() const { return address + 252; }
  // 9
  uint32_t slider_frame_b() const { return guest::LoadU32(base, address + 256); }
  void set_slider_frame_b(uint32_t v) const { guest::StoreU32(base, address + 256, v); }
  uint32_t slider_frame_b_address() const { return address + 256; }
  // 6
  uint32_t text_node() const { return guest::LoadU32(base, address + 260); }
  void set_text_node(uint32_t v) const { guest::StoreU32(base, address + 260, v); }
  uint32_t text_node_address() const { return address + 260; }
  // 10
  uint32_t text_child_a() const { return guest::LoadU32(base, address + 264); }
  void set_text_child_a(uint32_t v) const { guest::StoreU32(base, address + 264, v); }
  uint32_t text_child_a_address() const { return address + 264; }
  // 11
  uint32_t text_child_b() const { return guest::LoadU32(base, address + 268); }
  void set_text_child_b(uint32_t v) const { guest::StoreU32(base, address + 268, v); }
  uint32_t text_child_b_address() const { return address + 268; }
  // 8
  uint32_t count_text_node() const { return guest::LoadU32(base, address + 272); }
  void set_count_text_node(uint32_t v) const { guest::StoreU32(base, address + 272, v); }
  uint32_t count_text_node_address() const { return address + 272; }
  // 7: the second choice's number
  uint32_t count_child() const { return guest::LoadU32(base, address + 276); }
  void set_count_child(uint32_t v) const { guest::StoreU32(base, address + 276, v); }
  uint32_t count_child_address() const { return address + 276; }
  // the item menu's items' named animations
  uint32_t item_anims_address() const { return address + 280; }
  uint32_t item_anim0() const { return guest::LoadU32(base, address + 280); }
  void set_item_anim0(uint32_t v) const { guest::StoreU32(base, address + 280, v); }
  uint32_t item_anim0_address() const { return address + 280; }
  uint32_t item_anim1() const { return guest::LoadU32(base, address + 284); }
  void set_item_anim1(uint32_t v) const { guest::StoreU32(base, address + 284, v); }
  uint32_t item_anim1_address() const { return address + 284; }
  uint32_t item_anim2() const { return guest::LoadU32(base, address + 288); }
  void set_item_anim2(uint32_t v) const { guest::StoreU32(base, address + 288, v); }
  uint32_t item_anim2_address() const { return address + 288; }
  uint32_t unknown_292() const { return guest::LoadU32(base, address + 292); }
  void set_unknown_292(uint32_t v) const { guest::StoreU32(base, address + 292, v); }
  uint32_t unknown_292_address() const { return address + 292; }
  uint32_t unknown_296() const { return guest::LoadU32(base, address + 296); }
  void set_unknown_296(uint32_t v) const { guest::StoreU32(base, address + 296, v); }
  uint32_t unknown_296_address() const { return address + 296; }
  uint32_t ready_anim() const { return guest::LoadU32(base, address + 300); }
  void set_ready_anim(uint32_t v) const { guest::StoreU32(base, address + 300, v); }
  uint32_t ready_anim_address() const { return address + 300; }
  uint32_t opening_anim_a() const { return guest::LoadU32(base, address + 304); }
  void set_opening_anim_a(uint32_t v) const { guest::StoreU32(base, address + 304, v); }
  uint32_t opening_anim_a_address() const { return address + 304; }
  uint32_t opening_anim_b() const { return guest::LoadU32(base, address + 308); }
  void set_opening_anim_b(uint32_t v) const { guest::StoreU32(base, address + 308, v); }
  uint32_t opening_anim_b_address() const { return address + 308; }
  uint32_t opening_anim_c() const { return guest::LoadU32(base, address + 312); }
  void set_opening_anim_c(uint32_t v) const { guest::StoreU32(base, address + 312, v); }
  uint32_t opening_anim_c_address() const { return address + 312; }
  // the third choice's items' named animations
  uint32_t third_anims_address() const { return address + 316; }
  uint32_t third_anim0() const { return guest::LoadU32(base, address + 316); }
  void set_third_anim0(uint32_t v) const { guest::StoreU32(base, address + 316, v); }
  uint32_t third_anim0_address() const { return address + 316; }
  uint32_t third_anim1() const { return guest::LoadU32(base, address + 320); }
  void set_third_anim1(uint32_t v) const { guest::StoreU32(base, address + 320, v); }
  uint32_t third_anim1_address() const { return address + 320; }
  // restarted at item 1
  uint32_t item1_anim() const { return guest::LoadU32(base, address + 324); }
  void set_item1_anim(uint32_t v) const { guest::StoreU32(base, address + 324, v); }
  uint32_t item1_anim_address() const { return address + 324; }
  // restarted at item 2
  uint32_t item2_anim() const { return guest::LoadU32(base, address + 328); }
  void set_item2_anim(uint32_t v) const { guest::StoreU32(base, address + 328, v); }
  uint32_t item2_anim_address() const { return address + 328; }
  uint32_t selection_anim_a() const { return guest::LoadU32(base, address + 332); }
  void set_selection_anim_a(uint32_t v) const { guest::StoreU32(base, address + 332, v); }
  uint32_t selection_anim_a_address() const { return address + 332; }
  uint32_t selection_anim_b() const { return guest::LoadU32(base, address + 336); }
  void set_selection_anim_b(uint32_t v) const { guest::StoreU32(base, address + 336, v); }
  uint32_t selection_anim_b_address() const { return address + 336; }
  uint32_t selection_anim_c() const { return guest::LoadU32(base, address + 344); }
  void set_selection_anim_c(uint32_t v) const { guest::StoreU32(base, address + 344, v); }
  uint32_t selection_anim_c_address() const { return address + 344; }
  uint32_t selection_anim_d() const { return guest::LoadU32(base, address + 348); }
  void set_selection_anim_d(uint32_t v) const { guest::StoreU32(base, address + 348, v); }
  uint32_t selection_anim_d_address() const { return address + 348; }
  // 8
  uint32_t cursor_child_a() const { return guest::LoadU32(base, address + 356); }
  void set_cursor_child_a(uint32_t v) const { guest::StoreU32(base, address + 356, v); }
  uint32_t cursor_child_a_address() const { return address + 356; }
  // 9
  uint32_t cursor_child_b() const { return guest::LoadU32(base, address + 360); }
  void set_cursor_child_b(uint32_t v) const { guest::StoreU32(base, address + 360, v); }
  uint32_t cursor_child_b_address() const { return address + 360; }
};

// The image words docs/functions/type8-screens.md's code uses.
struct Type8ScreensGlobals {
  // the two players' garage cars (1,972 bytes each)
  static constexpr uint32_t garage_cars_address() { return 0x82814EB8u; }
  // the player the quit dialog was asked for
  static uint32_t dialog_player(uint8_t* base) { return guest::LoadU32(base, 0x82814E6Cu); }
  static void set_dialog_player(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E6Cu, v); }
  static constexpr uint32_t dialog_player_address() { return 0x82814E6Cu; }
  // the game mode before (1 the race)
  static uint32_t mode_before(uint8_t* base) { return guest::LoadU32(base, 0x826032E0u); }
  static void set_mode_before(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826032E0u, v); }
  static constexpr uint32_t mode_before_address() { return 0x826032E0u; }
  // the passes word ScreenFades::FadeIn is given
  static uint32_t fade_in_passes(uint8_t* base) { return guest::LoadU32(base, 0x82001BECu); }
  static void set_fade_in_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BECu, v); }
  static constexpr uint32_t fade_in_passes_address() { return 0x82001BECu; }
  // the passes word ScreenFades::FadeOut is given
  static uint32_t fade_out_passes(uint8_t* base) { return guest::LoadU32(base, 0x82001BF0u); }
  static void set_fade_out_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BF0u, v); }
  static constexpr uint32_t fade_out_passes_address() { return 0x82001BF0u; }
  // Type8ListBase::Draw's page table (10 member function pointers)
  static constexpr uint32_t list_base_pages_address() { return 0x82815FF0u; }
  // bit 0 once built
  static uint32_t list_base_page_guard(uint8_t* base) { return guest::LoadU32(base, 0x82816040u); }
  static void set_list_base_page_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816040u, v); }
  static constexpr uint32_t list_base_page_guard_address() { return 0x82816040u; }
  // Type8ListBase::Update's step table (6 entries)
  static constexpr uint32_t list_base_steps_address() { return 0x82816044u; }
  // bit 0 once built
  static uint32_t list_base_step_guard(uint8_t* base) { return guest::LoadU32(base, 0x82816074u); }
  static void set_list_base_step_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816074u, v); }
  static constexpr uint32_t list_base_step_guard_address() { return 0x82816074u; }
  // Type8ColorPartInner's steps (2)
  static constexpr uint32_t color_inner_steps_address() { return 0x82637A44u; }
  // Type8ColorPartInner's pages (3)
  static constexpr uint32_t color_inner_pages_address() { return 0x82637A4Cu; }
  // the pad player of each player's menus
  static constexpr uint32_t pad_players_address() { return 0x82637A64u; }
  // Type8ListPartInner's steps (2)
  static constexpr uint32_t list_inner_steps_address() { return 0x82637A6Cu; }
  // Type8ListPartInner's pages (3)
  static constexpr uint32_t list_inner_pages_address() { return 0x82637A74u; }
  // the color list's item string ids (3 a row, rows overlapping)
  static constexpr uint32_t color_item_texts_address() { return 0x82010B90u; }
  // SetDetails' string ids
  static constexpr uint32_t color_detail_texts_address() { return 0x82010BB4u; }
  // SetLabels' string ids
  static constexpr uint32_t color_label_texts_address() { return 0x82010BD4u; }
  // the color list's 15 item records of 28 bytes (+8 a count, +12 on bit numbers)
  static constexpr uint32_t color_item_records_address() { return 0x82010BE0u; }
  // the courses' string ids (6 a row, rows overlapping)
  static constexpr uint32_t course_texts_address() { return 0x82010D70u; }
  // 8 Type8CourseInfos
  static constexpr uint32_t course_infos_address() { return 0x82010D90u; }
  // Type8ColorBlock's id tables (by the player)
  static constexpr uint32_t color_block_ids_address() { return 0x82011348u; }
  // Type8PlayerBlock's id tables (by the player)
  static constexpr uint32_t player_block_ids_address() { return 0x82011430u; }
};

}  // namespace schema
