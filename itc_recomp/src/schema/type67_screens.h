// The views of docs/functions/type67-screens.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/type67-screens.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: ListScreen, ListScreen.+9892
struct ListScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 10144;
  static constexpr uint32_t kVtable = 0x82011218;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // a ScreenMachine (boot-mode.md)
  uint32_t machine_address() const { return address + 4; }
  // the machine's state (its +9604)
  uint32_t state() const { return guest::LoadU32(base, address + 9608); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t state_address() const { return address + 9608; }
  uint32_t movie_loaded() const { return guest::LoadU32(base, address + 9612); }
  void set_movie_loaded(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t movie_loaded_address() const { return address + 9612; }
  // open the movie at the mode's start (else wait for the open message)
  uint32_t open_at_start() const { return guest::LoadU32(base, address + 9616); }
  void set_open_at_start(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t open_at_start_address() const { return address + 9616; }
  // the page Draw runs (-1 while a step runs)
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
  // the step Update runs (-1 none)
  uint32_t step() const { return guest::LoadU32(base, address + 9656); }
  void set_step(uint32_t v) const { guest::StoreU32(base, address + 9656, v); }
  uint32_t step_address() const { return address + 9656; }
  // the step after it (-1 none)
  uint32_t queued_step() const { return guest::LoadU32(base, address + 9660); }
  void set_queued_step(uint32_t v) const { guest::StoreU32(base, address + 9660, v); }
  uint32_t queued_step_address() const { return address + 9660; }
  // the page block's +20: 0 when a queued step starts
  uint32_t unknown_9664() const { return guest::LoadU32(base, address + 9664); }
  void set_unknown_9664(uint32_t v) const { guest::StoreU32(base, address + 9664, v); }
  uint32_t unknown_9664_address() const { return address + 9664; }
  // set by a step once its first call ran; 0 when the block moves on
  uint32_t step_started() const { return guest::LoadU32(base, address + 9668); }
  void set_step_started(uint32_t v) const { guest::StoreU32(base, address + 9668, v); }
  uint32_t step_started_address() const { return address + 9668; }
  // the node table (0 none): the type 6 list's at +10360, the type 7 list's at +10432
  template <typename T = NodeTable>
  T node_table() const { return T(base, guest::LoadU32(base, address + 9672)); }
  void set_node_table(const View& v) const { guest::StoreU32(base, address + 9672, v.address); }
  void set_node_table(uint32_t v) const { guest::StoreU32(base, address + 9672, v); }
  uint32_t node_table_address() const { return address + 9672; }
  // the list part (0 none): +10824 type 6, +11040 type 7
  template <typename T = ListPart>
  T list_part() const { return T(base, guest::LoadU32(base, address + 9676)); }
  void set_list_part(const View& v) const { guest::StoreU32(base, address + 9676, v.address); }
  void set_list_part(uint32_t v) const { guest::StoreU32(base, address + 9676, v); }
  uint32_t list_part_address() const { return address + 9676; }
  // the UI movie's id (1 at the constructor; 25 the node list)
  uint32_t movie() const { return guest::LoadU32(base, address + 9680); }
  void set_movie(uint32_t v) const { guest::StoreU32(base, address + 9680, v); }
  uint32_t movie_address() const { return address + 9680; }
  // 1 at the constructor, 0 type 7
  uint32_t unknown_9684() const { return guest::LoadU32(base, address + 9684); }
  void set_unknown_9684(uint32_t v) const { guest::StoreU32(base, address + 9684, v); }
  uint32_t unknown_9684_address() const { return address + 9684; }
  // 1 when the race was set (the part's yes): the screen ends in state 11
  uint32_t race_set() const { return guest::LoadU32(base, address + 9688); }
  void set_race_set(uint32_t v) const { guest::StoreU32(base, address + 9688, v); }
  uint32_t race_set_address() const { return address + 9688; }
  // 1 once the movie loaded
  uint32_t movie_ready() const { return guest::LoadU32(base, address + 9692); }
  void set_movie_ready(uint32_t v) const { guest::StoreU32(base, address + 9692, v); }
  uint32_t movie_ready_address() const { return address + 9692; }
  // the list part's input on
  uint32_t part_input() const { return guest::LoadU32(base, address + 9696); }
  void set_part_input(uint32_t v) const { guest::StoreU32(base, address + 9696, v); }
  uint32_t part_input_address() const { return address + 9696; }
  // bit 0 of the item info's +12
  uint32_t item_bit_0() const { return guest::LoadU32(base, address + 9700); }
  void set_item_bit_0(uint32_t v) const { guest::StoreU32(base, address + 9700, v); }
  uint32_t item_bit_0_address() const { return address + 9700; }
  // the same test, ANDed with the info's +12
  uint32_t item_bit_1() const { return guest::LoadU32(base, address + 9704); }
  void set_item_bit_1(uint32_t v) const { guest::StoreU32(base, address + 9704, v); }
  uint32_t item_bit_1_address() const { return address + 9704; }
  // type 7: show the part's nodes with it, and skip the leaving step after its yes
  uint32_t part_nodes() const { return guest::LoadU32(base, address + 9708); }
  void set_part_nodes(uint32_t v) const { guest::StoreU32(base, address + 9708, v); }
  uint32_t part_nodes_address() const { return address + 9708; }
  // the title text's child (string 322) on text_sub_scene
  uint32_t title_child() const { return guest::LoadU32(base, address + 9712); }
  void set_title_child(uint32_t v) const { guest::StoreU32(base, address + 9712, v); }
  uint32_t title_child_address() const { return address + 9712; }
  // a text's child (text 0; 0 none)
  uint32_t text_child_a() const { return guest::LoadU32(base, address + 9716); }
  void set_text_child_a(uint32_t v) const { guest::StoreU32(base, address + 9716, v); }
  uint32_t text_child_a_address() const { return address + 9716; }
  // a text's child (text 1; 0 none)
  uint32_t text_child_b() const { return guest::LoadU32(base, address + 9720); }
  void set_text_child_b(uint32_t v) const { guest::StoreU32(base, address + 9720, v); }
  uint32_t text_child_b_address() const { return address + 9720; }
  // the selection nodes' sub-scene (-1 none)
  uint32_t selection_sub_scene() const { return guest::LoadU32(base, address + 9724); }
  void set_selection_sub_scene(uint32_t v) const { guest::StoreU32(base, address + 9724, v); }
  uint32_t selection_sub_scene_address() const { return address + 9724; }
  // the titles' and arrows' sub-scene
  uint32_t text_sub_scene() const { return guest::LoadU32(base, address + 9728); }
  void set_text_sub_scene(uint32_t v) const { guest::StoreU32(base, address + 9728, v); }
  uint32_t text_sub_scene_address() const { return address + 9728; }
  uint32_t icon_sub_scene() const { return guest::LoadU32(base, address + 9732); }
  void set_icon_sub_scene(uint32_t v) const { guest::StoreU32(base, address + 9732, v); }
  uint32_t icon_sub_scene_address() const { return address + 9732; }
  // the color cursor's node; the sub-scene of color_cursor_child
  uint32_t color_cursor_node() const { return guest::LoadU32(base, address + 9736); }
  void set_color_cursor_node(uint32_t v) const { guest::StoreU32(base, address + 9736, v); }
  uint32_t color_cursor_node_address() const { return address + 9736; }
  // the scroll text's sub-scene
  uint32_t scroll_sub_scene() const { return guest::LoadU32(base, address + 9740); }
  void set_scroll_sub_scene(uint32_t v) const { guest::StoreU32(base, address + 9740, v); }
  uint32_t scroll_sub_scene_address() const { return address + 9740; }
  // the item cursor's node
  uint32_t item_cursor_node() const { return guest::LoadU32(base, address + 9744); }
  void set_item_cursor_node(uint32_t v) const { guest::StoreU32(base, address + 9744, v); }
  uint32_t item_cursor_node_address() const { return address + 9744; }
  // a child on selection_sub_scene shown by shown_child_on
  uint32_t shown_child() const { return guest::LoadU32(base, address + 9748); }
  void set_shown_child(uint32_t v) const { guest::StoreU32(base, address + 9748, v); }
  uint32_t shown_child_address() const { return address + 9748; }
  // the arrow SetArrows(0) sets
  uint32_t arrow_child_a() const { return guest::LoadU32(base, address + 9752); }
  void set_arrow_child_a(uint32_t v) const { guest::StoreU32(base, address + 9752, v); }
  uint32_t arrow_child_a_address() const { return address + 9752; }
  // the arrow SetArrows(1) sets
  uint32_t arrow_child_b() const { return guest::LoadU32(base, address + 9756); }
  void set_arrow_child_b(uint32_t v) const { guest::StoreU32(base, address + 9756, v); }
  uint32_t arrow_child_b_address() const { return address + 9756; }
  // a node (no sub-scene) given the selection
  uint32_t selection_node() const { return guest::LoadU32(base, address + 9760); }
  void set_selection_node(uint32_t v) const { guest::StoreU32(base, address + 9760, v); }
  uint32_t selection_node_address() const { return address + 9760; }
  // a child given the previous selection
  uint32_t previous_child() const { return guest::LoadU32(base, address + 9764); }
  void set_previous_child(uint32_t v) const { guest::StoreU32(base, address + 9764, v); }
  uint32_t previous_child_address() const { return address + 9764; }
  // a child given the selection
  uint32_t selection_child() const { return guest::LoadU32(base, address + 9768); }
  void set_selection_child(uint32_t v) const { guest::StoreU32(base, address + 9768, v); }
  uint32_t selection_child_address() const { return address + 9768; }
  // an array of children_count children given the previous selection (0 none)
  uint32_t previous_children() const { return guest::LoadU32(base, address + 9772); }
  void set_previous_children(uint32_t v) const { guest::StoreU32(base, address + 9772, v); }
  uint32_t previous_children_address() const { return address + 9772; }
  // an array given the selection (0 none; type 7 0x826371A4)
  uint32_t selection_children() const { return guest::LoadU32(base, address + 9776); }
  void set_selection_children(uint32_t v) const { guest::StoreU32(base, address + 9776, v); }
  uint32_t selection_children_address() const { return address + 9776; }
  // not written by the constructor
  uint32_t unknown_9780() const { return guest::LoadU32(base, address + 9780); }
  void set_unknown_9780(uint32_t v) const { guest::StoreU32(base, address + 9780, v); }
  uint32_t unknown_9780_address() const { return address + 9780; }
  uint32_t children_count() const { return guest::LoadU32(base, address + 9784); }
  void set_children_count(uint32_t v) const { guest::StoreU32(base, address + 9784, v); }
  uint32_t children_count_address() const { return address + 9784; }
  uint32_t shown_child_on() const { return guest::LoadU32(base, address + 9788); }
  void set_shown_child_on(uint32_t v) const { guest::StoreU32(base, address + 9788, v); }
  uint32_t shown_child_on_address() const { return address + 9788; }
  uint32_t unknown_9792() const { return guest::LoadU32(base, address + 9792); }
  void set_unknown_9792(uint32_t v) const { guest::StoreU32(base, address + 9792, v); }
  uint32_t unknown_9792_address() const { return address + 9792; }
  uint32_t unknown_9796() const { return guest::LoadU32(base, address + 9796); }
  void set_unknown_9796(uint32_t v) const { guest::StoreU32(base, address + 9796, v); }
  uint32_t unknown_9796_address() const { return address + 9796; }
  // the arrows' +108 when on
  uint32_t arrow_on_frame() const { return guest::LoadU32(base, address + 9800); }
  void set_arrow_on_frame(uint32_t v) const { guest::StoreU32(base, address + 9800, v); }
  uint32_t arrow_on_frame_address() const { return address + 9800; }
  // the arrows' +108 when off
  uint32_t arrow_off_frame() const { return guest::LoadU32(base, address + 9804); }
  void set_arrow_off_frame(uint32_t v) const { guest::StoreU32(base, address + 9804, v); }
  uint32_t arrow_off_frame_address() const { return address + 9804; }
  uint32_t color_cursor_child() const { return guest::LoadU32(base, address + 9808); }
  void set_color_cursor_child(uint32_t v) const { guest::StoreU32(base, address + 9808, v); }
  uint32_t color_cursor_child_address() const { return address + 9808; }
  // the first of six icon children (four textures, two hidden)
  uint32_t icon_child() const { return guest::LoadU32(base, address + 9812); }
  void set_icon_child(uint32_t v) const { guest::StoreU32(base, address + 9812, v); }
  uint32_t icon_child_address() const { return address + 9812; }
  // the scroll text's child
  uint32_t scroll_child() const { return guest::LoadU32(base, address + 9816); }
  void set_scroll_child(uint32_t v) const { guest::StoreU32(base, address + 9816, v); }
  uint32_t scroll_child_address() const { return address + 9816; }
  uint32_t unknown_9820() const { return guest::LoadU32(base, address + 9820); }
  void set_unknown_9820(uint32_t v) const { guest::StoreU32(base, address + 9820, v); }
  uint32_t unknown_9820_address() const { return address + 9820; }
  // type 7: a child on text_sub_scene shown with the part
  uint32_t part_child() const { return guest::LoadU32(base, address + 9824); }
  void set_part_child(uint32_t v) const { guest::StoreU32(base, address + 9824, v); }
  uint32_t part_child_address() const { return address + 9824; }
  uint32_t item_cursor_child() const { return guest::LoadU32(base, address + 9828); }
  void set_item_cursor_child(uint32_t v) const { guest::StoreU32(base, address + 9828, v); }
  uint32_t item_cursor_child_address() const { return address + 9828; }
  // the opening: played once
  uint32_t open_anim() const { return guest::LoadU32(base, address + 9832); }
  void set_open_anim(uint32_t v) const { guest::StoreU32(base, address + 9832, v); }
  uint32_t open_anim_address() const { return address + 9832; }
  // the leaving: played once
  uint32_t leave_anim() const { return guest::LoadU32(base, address + 9836); }
  void set_leave_anim(uint32_t v) const { guest::StoreU32(base, address + 9836, v); }
  uint32_t leave_anim_address() const { return address + 9836; }
  // type 7's step 2 (page 2 to 3)
  uint32_t first_to_items_anim() const { return guest::LoadU32(base, address + 9840); }
  void set_first_to_items_anim(uint32_t v) const { guest::StoreU32(base, address + 9840, v); }
  uint32_t first_to_items_anim_address() const { return address + 9840; }
  // type 7's step 3 (page 3 back to 2)
  uint32_t items_to_first_anim() const { return guest::LoadU32(base, address + 9844); }
  void set_items_to_first_anim(uint32_t v) const { guest::StoreU32(base, address + 9844, v); }
  uint32_t items_to_first_anim_address() const { return address + 9844; }
  // type 7's step 4 (page 3 to 4)
  uint32_t items_to_colors_anim() const { return guest::LoadU32(base, address + 9848); }
  void set_items_to_colors_anim(uint32_t v) const { guest::StoreU32(base, address + 9848, v); }
  uint32_t items_to_colors_anim_address() const { return address + 9848; }
  // type 7's step 6 (page 4 back to 3)
  uint32_t colors_to_items_anim() const { return guest::LoadU32(base, address + 9852); }
  void set_colors_to_items_anim(uint32_t v) const { guest::StoreU32(base, address + 9852, v); }
  uint32_t colors_to_items_anim_address() const { return address + 9852; }
  // the opening: played and looped
  uint32_t open_loop_a() const { return guest::LoadU32(base, address + 9856); }
  void set_open_loop_a(uint32_t v) const { guest::StoreU32(base, address + 9856, v); }
  uint32_t open_loop_a_address() const { return address + 9856; }
  // the opening: played and looped
  uint32_t open_loop_b() const { return guest::LoadU32(base, address + 9860); }
  void set_open_loop_b(uint32_t v) const { guest::StoreU32(base, address + 9860, v); }
  uint32_t open_loop_b_address() const { return address + 9860; }
  // the opening: chained
  uint32_t open_anim_b() const { return guest::LoadU32(base, address + 9864); }
  void set_open_anim_b(uint32_t v) const { guest::StoreU32(base, address + 9864, v); }
  uint32_t open_anim_b_address() const { return address + 9864; }
  // after the opening: looped
  uint32_t shown_loop_a() const { return guest::LoadU32(base, address + 9868); }
  void set_shown_loop_a(uint32_t v) const { guest::StoreU32(base, address + 9868, v); }
  uint32_t shown_loop_a_address() const { return address + 9868; }
  // the item list's loop (finished when it leaves)
  uint32_t items_loop() const { return guest::LoadU32(base, address + 9872); }
  void set_items_loop(uint32_t v) const { guest::StoreU32(base, address + 9872, v); }
  uint32_t items_loop_address() const { return address + 9872; }
  // after the opening: looped
  uint32_t shown_loop_b() const { return guest::LoadU32(base, address + 9876); }
  void set_shown_loop_b(uint32_t v) const { guest::StoreU32(base, address + 9876, v); }
  uint32_t shown_loop_b_address() const { return address + 9876; }
  // after the opening: looped
  uint32_t shown_loop_c() const { return guest::LoadU32(base, address + 9880); }
  void set_shown_loop_c(uint32_t v) const { guest::StoreU32(base, address + 9880, v); }
  uint32_t shown_loop_c_address() const { return address + 9880; }
  // after the opening: looped
  uint32_t shown_loop_d() const { return guest::LoadU32(base, address + 9884); }
  void set_shown_loop_d(uint32_t v) const { guest::StoreU32(base, address + 9884, v); }
  uint32_t shown_loop_d_address() const { return address + 9884; }
  // the opening: chained
  uint32_t open_anim_c() const { return guest::LoadU32(base, address + 9888); }
  void set_open_anim_c(uint32_t v) const { guest::StoreU32(base, address + 9888, v); }
  uint32_t open_anim_c_address() const { return address + 9888; }
  // the toggle's animation for its selection 0
  uint32_t toggle_anim_a() const { return guest::LoadU32(base, address + 9892); }
  void set_toggle_anim_a(uint32_t v) const { guest::StoreU32(base, address + 9892, v); }
  uint32_t toggle_anim_a_address() const { return address + 9892; }
  // the toggle's animation for its selection 1
  uint32_t toggle_anim_b() const { return guest::LoadU32(base, address + 9896); }
  void set_toggle_anim_b(uint32_t v) const { guest::StoreU32(base, address + 9896, v); }
  uint32_t toggle_anim_b_address() const { return address + 9896; }
  // replayed by ShowSelection
  uint32_t selection_anim() const { return guest::LoadU32(base, address + 9900); }
  void set_selection_anim(uint32_t v) const { guest::StoreU32(base, address + 9900, v); }
  uint32_t selection_anim_address() const { return address + 9900; }
  // an item list's move up
  uint32_t item_up_anim() const { return guest::LoadU32(base, address + 9904); }
  void set_item_up_anim(uint32_t v) const { guest::StoreU32(base, address + 9904, v); }
  uint32_t item_up_anim_address() const { return address + 9904; }
  // an item list's move down
  uint32_t item_down_anim() const { return guest::LoadU32(base, address + 9908); }
  void set_item_down_anim(uint32_t v) const { guest::StoreU32(base, address + 9908, v); }
  uint32_t item_down_anim_address() const { return address + 9908; }
  // type 7's first list moved back
  uint32_t first_back_anim() const { return guest::LoadU32(base, address + 9912); }
  void set_first_back_anim(uint32_t v) const { guest::StoreU32(base, address + 9912, v); }
  uint32_t first_back_anim_address() const { return address + 9912; }
  // type 7's first list moved forward
  uint32_t first_forward_anim() const { return guest::LoadU32(base, address + 9916); }
  void set_first_forward_anim(uint32_t v) const { guest::StoreU32(base, address + 9916, v); }
  uint32_t first_forward_anim_address() const { return address + 9916; }
  // type 7's color list moved back
  uint32_t color_back_anim() const { return guest::LoadU32(base, address + 9920); }
  void set_color_back_anim(uint32_t v) const { guest::StoreU32(base, address + 9920, v); }
  uint32_t color_back_anim_address() const { return address + 9920; }
  // type 7's color list moved forward
  uint32_t color_forward_anim() const { return guest::LoadU32(base, address + 9924); }
  void set_color_forward_anim(uint32_t v) const { guest::StoreU32(base, address + 9924, v); }
  uint32_t color_forward_anim_address() const { return address + 9924; }
  // a list scrolled down
  uint32_t scroll_down_anim() const { return guest::LoadU32(base, address + 9928); }
  void set_scroll_down_anim(uint32_t v) const { guest::StoreU32(base, address + 9928, v); }
  uint32_t scroll_down_anim_address() const { return address + 9928; }
  // a list scrolled up
  uint32_t scroll_up_anim() const { return guest::LoadU32(base, address + 9932); }
  void set_scroll_up_anim(uint32_t v) const { guest::StoreU32(base, address + 9932, v); }
  uint32_t scroll_up_anim_address() const { return address + 9932; }
  // ten named animations by the item list's selection (LoopItemNode)
  uint32_t item_anims_address() const { return address + 9936; }
  // item_anims[0]
  uint32_t item_anim_0() const { return guest::LoadU32(base, address + 9936); }
  void set_item_anim_0(uint32_t v) const { guest::StoreU32(base, address + 9936, v); }
  uint32_t item_anim_0_address() const { return address + 9936; }
  // ten by the first list's selection (LoopSecondNode)
  uint32_t first_anims_address() const { return address + 9976; }
  // first_anims[0]
  uint32_t first_anim_0() const { return guest::LoadU32(base, address + 9976); }
  void set_first_anim_0(uint32_t v) const { guest::StoreU32(base, address + 9976, v); }
  uint32_t first_anim_0_address() const { return address + 9976; }
  // ten by the color list's selection (LoopThirdNode)
  uint32_t color_anims_address() const { return address + 10016; }
  // color_anims[0]
  uint32_t color_anim_0() const { return guest::LoadU32(base, address + 10016); }
  void set_color_anim_0(uint32_t v) const { guest::StoreU32(base, address + 10016, v); }
  uint32_t color_anim_0_address() const { return address + 10016; }
  // ten by the item's selection (type 7's step 4 plays, step 6 finishes)
  uint32_t item_anims_b_address() const { return address + 10056; }
  // item_anims_b[0]
  uint32_t item_anim_b_0() const { return guest::LoadU32(base, address + 10056); }
  void set_item_anim_b_0(uint32_t v) const { guest::StoreU32(base, address + 10056, v); }
  uint32_t item_anim_b_0_address() const { return address + 10056; }
  // item_anims_b[1]
  uint32_t item_anim_b_1() const { return guest::LoadU32(base, address + 10060); }
  void set_item_anim_b_1(uint32_t v) const { guest::StoreU32(base, address + 10060, v); }
  uint32_t item_anim_b_1_address() const { return address + 10060; }
  // item_anims_b[2]
  uint32_t item_anim_b_2() const { return guest::LoadU32(base, address + 10064); }
  void set_item_anim_b_2(uint32_t v) const { guest::StoreU32(base, address + 10064, v); }
  uint32_t item_anim_b_2_address() const { return address + 10064; }
  // ten by the item's selection (type 7's step 4 finishes, step 6 plays)
  uint32_t item_anims_c_address() const { return address + 10096; }
  // item_anims_c[0]
  uint32_t item_anim_c_0() const { return guest::LoadU32(base, address + 10096); }
  void set_item_anim_c_0(uint32_t v) const { guest::StoreU32(base, address + 10096, v); }
  uint32_t item_anim_c_0_address() const { return address + 10096; }
  // item_anims_c[1]
  uint32_t item_anim_c_1() const { return guest::LoadU32(base, address + 10100); }
  void set_item_anim_c_1(uint32_t v) const { guest::StoreU32(base, address + 10100, v); }
  uint32_t item_anim_c_1_address() const { return address + 10100; }
  // item_anims_c[2]
  uint32_t item_anim_c_2() const { return guest::LoadU32(base, address + 10104); }
  void set_item_anim_c_2(uint32_t v) const { guest::StoreU32(base, address + 10104, v); }
  uint32_t item_anim_c_2_address() const { return address + 10104; }
  // cleared by ClearScrollText
  uint32_t scroll_text_shown() const { return guest::LoadU32(base, address + 10136); }
  void set_scroll_text_shown(uint32_t v) const { guest::StoreU32(base, address + 10136, v); }
  uint32_t scroll_text_shown_address() const { return address + 10136; }
  // the scroll text shown (-1 none)
  uint32_t scroll_text() const { return guest::LoadU32(base, address + 10140); }
  void set_scroll_text(uint32_t v) const { guest::StoreU32(base, address + 10140, v); }
  uint32_t scroll_text_address() const { return address + 10140; }
};

// embedded; measure: NodeListScreen
struct NodeListScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 10820;
  static constexpr uint32_t kVtable = 0x82011558;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // a ScreenMachine (boot-mode.md)
  uint32_t machine_address() const { return address + 4; }
  // the machine's state (its +9604)
  uint32_t state() const { return guest::LoadU32(base, address + 9608); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t state_address() const { return address + 9608; }
  uint32_t movie_loaded() const { return guest::LoadU32(base, address + 9612); }
  void set_movie_loaded(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t movie_loaded_address() const { return address + 9612; }
  // open the movie at the mode's start (else wait for the open message)
  uint32_t open_at_start() const { return guest::LoadU32(base, address + 9616); }
  void set_open_at_start(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t open_at_start_address() const { return address + 9616; }
  // the page Draw runs (-1 while a step runs)
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
  // the step Update runs (-1 none)
  uint32_t step() const { return guest::LoadU32(base, address + 9656); }
  void set_step(uint32_t v) const { guest::StoreU32(base, address + 9656, v); }
  uint32_t step_address() const { return address + 9656; }
  // the step after it (-1 none)
  uint32_t queued_step() const { return guest::LoadU32(base, address + 9660); }
  void set_queued_step(uint32_t v) const { guest::StoreU32(base, address + 9660, v); }
  uint32_t queued_step_address() const { return address + 9660; }
  // the page block's +20: 0 when a queued step starts
  uint32_t unknown_9664() const { return guest::LoadU32(base, address + 9664); }
  void set_unknown_9664(uint32_t v) const { guest::StoreU32(base, address + 9664, v); }
  uint32_t unknown_9664_address() const { return address + 9664; }
  // set by a step once its first call ran; 0 when the block moves on
  uint32_t step_started() const { return guest::LoadU32(base, address + 9668); }
  void set_step_started(uint32_t v) const { guest::StoreU32(base, address + 9668, v); }
  uint32_t step_started_address() const { return address + 9668; }
  // the node table (0 none): the type 6 list's at +10360, the type 7 list's at +10432
  template <typename T = NodeTable>
  T node_table() const { return T(base, guest::LoadU32(base, address + 9672)); }
  void set_node_table(const View& v) const { guest::StoreU32(base, address + 9672, v.address); }
  void set_node_table(uint32_t v) const { guest::StoreU32(base, address + 9672, v); }
  uint32_t node_table_address() const { return address + 9672; }
  // the list part (0 none): +10824 type 6, +11040 type 7
  template <typename T = ListPart>
  T list_part() const { return T(base, guest::LoadU32(base, address + 9676)); }
  void set_list_part(const View& v) const { guest::StoreU32(base, address + 9676, v.address); }
  void set_list_part(uint32_t v) const { guest::StoreU32(base, address + 9676, v); }
  uint32_t list_part_address() const { return address + 9676; }
  // the UI movie's id (1 at the constructor; 25 the node list)
  uint32_t movie() const { return guest::LoadU32(base, address + 9680); }
  void set_movie(uint32_t v) const { guest::StoreU32(base, address + 9680, v); }
  uint32_t movie_address() const { return address + 9680; }
  // 1 at the constructor, 0 type 7
  uint32_t unknown_9684() const { return guest::LoadU32(base, address + 9684); }
  void set_unknown_9684(uint32_t v) const { guest::StoreU32(base, address + 9684, v); }
  uint32_t unknown_9684_address() const { return address + 9684; }
  // 1 when the race was set (the part's yes): the screen ends in state 11
  uint32_t race_set() const { return guest::LoadU32(base, address + 9688); }
  void set_race_set(uint32_t v) const { guest::StoreU32(base, address + 9688, v); }
  uint32_t race_set_address() const { return address + 9688; }
  // 1 once the movie loaded
  uint32_t movie_ready() const { return guest::LoadU32(base, address + 9692); }
  void set_movie_ready(uint32_t v) const { guest::StoreU32(base, address + 9692, v); }
  uint32_t movie_ready_address() const { return address + 9692; }
  // the list part's input on
  uint32_t part_input() const { return guest::LoadU32(base, address + 9696); }
  void set_part_input(uint32_t v) const { guest::StoreU32(base, address + 9696, v); }
  uint32_t part_input_address() const { return address + 9696; }
  // bit 0 of the item info's +12
  uint32_t item_bit_0() const { return guest::LoadU32(base, address + 9700); }
  void set_item_bit_0(uint32_t v) const { guest::StoreU32(base, address + 9700, v); }
  uint32_t item_bit_0_address() const { return address + 9700; }
  // the same test, ANDed with the info's +12
  uint32_t item_bit_1() const { return guest::LoadU32(base, address + 9704); }
  void set_item_bit_1(uint32_t v) const { guest::StoreU32(base, address + 9704, v); }
  uint32_t item_bit_1_address() const { return address + 9704; }
  // type 7: show the part's nodes with it, and skip the leaving step after its yes
  uint32_t part_nodes() const { return guest::LoadU32(base, address + 9708); }
  void set_part_nodes(uint32_t v) const { guest::StoreU32(base, address + 9708, v); }
  uint32_t part_nodes_address() const { return address + 9708; }
  // the title text's child (string 322) on text_sub_scene
  uint32_t title_child() const { return guest::LoadU32(base, address + 9712); }
  void set_title_child(uint32_t v) const { guest::StoreU32(base, address + 9712, v); }
  uint32_t title_child_address() const { return address + 9712; }
  // a text's child (text 0; 0 none)
  uint32_t text_child_a() const { return guest::LoadU32(base, address + 9716); }
  void set_text_child_a(uint32_t v) const { guest::StoreU32(base, address + 9716, v); }
  uint32_t text_child_a_address() const { return address + 9716; }
  // a text's child (text 1; 0 none)
  uint32_t text_child_b() const { return guest::LoadU32(base, address + 9720); }
  void set_text_child_b(uint32_t v) const { guest::StoreU32(base, address + 9720, v); }
  uint32_t text_child_b_address() const { return address + 9720; }
  // the selection nodes' sub-scene (-1 none)
  uint32_t selection_sub_scene() const { return guest::LoadU32(base, address + 9724); }
  void set_selection_sub_scene(uint32_t v) const { guest::StoreU32(base, address + 9724, v); }
  uint32_t selection_sub_scene_address() const { return address + 9724; }
  // the titles' and arrows' sub-scene
  uint32_t text_sub_scene() const { return guest::LoadU32(base, address + 9728); }
  void set_text_sub_scene(uint32_t v) const { guest::StoreU32(base, address + 9728, v); }
  uint32_t text_sub_scene_address() const { return address + 9728; }
  uint32_t icon_sub_scene() const { return guest::LoadU32(base, address + 9732); }
  void set_icon_sub_scene(uint32_t v) const { guest::StoreU32(base, address + 9732, v); }
  uint32_t icon_sub_scene_address() const { return address + 9732; }
  // the color cursor's node; the sub-scene of color_cursor_child
  uint32_t color_cursor_node() const { return guest::LoadU32(base, address + 9736); }
  void set_color_cursor_node(uint32_t v) const { guest::StoreU32(base, address + 9736, v); }
  uint32_t color_cursor_node_address() const { return address + 9736; }
  // the scroll text's sub-scene
  uint32_t scroll_sub_scene() const { return guest::LoadU32(base, address + 9740); }
  void set_scroll_sub_scene(uint32_t v) const { guest::StoreU32(base, address + 9740, v); }
  uint32_t scroll_sub_scene_address() const { return address + 9740; }
  // the item cursor's node
  uint32_t item_cursor_node() const { return guest::LoadU32(base, address + 9744); }
  void set_item_cursor_node(uint32_t v) const { guest::StoreU32(base, address + 9744, v); }
  uint32_t item_cursor_node_address() const { return address + 9744; }
  // a child on selection_sub_scene shown by shown_child_on
  uint32_t shown_child() const { return guest::LoadU32(base, address + 9748); }
  void set_shown_child(uint32_t v) const { guest::StoreU32(base, address + 9748, v); }
  uint32_t shown_child_address() const { return address + 9748; }
  // the arrow SetArrows(0) sets
  uint32_t arrow_child_a() const { return guest::LoadU32(base, address + 9752); }
  void set_arrow_child_a(uint32_t v) const { guest::StoreU32(base, address + 9752, v); }
  uint32_t arrow_child_a_address() const { return address + 9752; }
  // the arrow SetArrows(1) sets
  uint32_t arrow_child_b() const { return guest::LoadU32(base, address + 9756); }
  void set_arrow_child_b(uint32_t v) const { guest::StoreU32(base, address + 9756, v); }
  uint32_t arrow_child_b_address() const { return address + 9756; }
  // a node (no sub-scene) given the selection
  uint32_t selection_node() const { return guest::LoadU32(base, address + 9760); }
  void set_selection_node(uint32_t v) const { guest::StoreU32(base, address + 9760, v); }
  uint32_t selection_node_address() const { return address + 9760; }
  // a child given the previous selection
  uint32_t previous_child() const { return guest::LoadU32(base, address + 9764); }
  void set_previous_child(uint32_t v) const { guest::StoreU32(base, address + 9764, v); }
  uint32_t previous_child_address() const { return address + 9764; }
  // a child given the selection
  uint32_t selection_child() const { return guest::LoadU32(base, address + 9768); }
  void set_selection_child(uint32_t v) const { guest::StoreU32(base, address + 9768, v); }
  uint32_t selection_child_address() const { return address + 9768; }
  // an array of children_count children given the previous selection (0 none)
  uint32_t previous_children() const { return guest::LoadU32(base, address + 9772); }
  void set_previous_children(uint32_t v) const { guest::StoreU32(base, address + 9772, v); }
  uint32_t previous_children_address() const { return address + 9772; }
  // an array given the selection (0 none; type 7 0x826371A4)
  uint32_t selection_children() const { return guest::LoadU32(base, address + 9776); }
  void set_selection_children(uint32_t v) const { guest::StoreU32(base, address + 9776, v); }
  uint32_t selection_children_address() const { return address + 9776; }
  // not written by the constructor
  uint32_t unknown_9780() const { return guest::LoadU32(base, address + 9780); }
  void set_unknown_9780(uint32_t v) const { guest::StoreU32(base, address + 9780, v); }
  uint32_t unknown_9780_address() const { return address + 9780; }
  uint32_t children_count() const { return guest::LoadU32(base, address + 9784); }
  void set_children_count(uint32_t v) const { guest::StoreU32(base, address + 9784, v); }
  uint32_t children_count_address() const { return address + 9784; }
  uint32_t shown_child_on() const { return guest::LoadU32(base, address + 9788); }
  void set_shown_child_on(uint32_t v) const { guest::StoreU32(base, address + 9788, v); }
  uint32_t shown_child_on_address() const { return address + 9788; }
  uint32_t unknown_9792() const { return guest::LoadU32(base, address + 9792); }
  void set_unknown_9792(uint32_t v) const { guest::StoreU32(base, address + 9792, v); }
  uint32_t unknown_9792_address() const { return address + 9792; }
  uint32_t unknown_9796() const { return guest::LoadU32(base, address + 9796); }
  void set_unknown_9796(uint32_t v) const { guest::StoreU32(base, address + 9796, v); }
  uint32_t unknown_9796_address() const { return address + 9796; }
  // the arrows' +108 when on
  uint32_t arrow_on_frame() const { return guest::LoadU32(base, address + 9800); }
  void set_arrow_on_frame(uint32_t v) const { guest::StoreU32(base, address + 9800, v); }
  uint32_t arrow_on_frame_address() const { return address + 9800; }
  // the arrows' +108 when off
  uint32_t arrow_off_frame() const { return guest::LoadU32(base, address + 9804); }
  void set_arrow_off_frame(uint32_t v) const { guest::StoreU32(base, address + 9804, v); }
  uint32_t arrow_off_frame_address() const { return address + 9804; }
  uint32_t color_cursor_child() const { return guest::LoadU32(base, address + 9808); }
  void set_color_cursor_child(uint32_t v) const { guest::StoreU32(base, address + 9808, v); }
  uint32_t color_cursor_child_address() const { return address + 9808; }
  // the first of six icon children (four textures, two hidden)
  uint32_t icon_child() const { return guest::LoadU32(base, address + 9812); }
  void set_icon_child(uint32_t v) const { guest::StoreU32(base, address + 9812, v); }
  uint32_t icon_child_address() const { return address + 9812; }
  // the scroll text's child
  uint32_t scroll_child() const { return guest::LoadU32(base, address + 9816); }
  void set_scroll_child(uint32_t v) const { guest::StoreU32(base, address + 9816, v); }
  uint32_t scroll_child_address() const { return address + 9816; }
  uint32_t unknown_9820() const { return guest::LoadU32(base, address + 9820); }
  void set_unknown_9820(uint32_t v) const { guest::StoreU32(base, address + 9820, v); }
  uint32_t unknown_9820_address() const { return address + 9820; }
  // type 7: a child on text_sub_scene shown with the part
  uint32_t part_child() const { return guest::LoadU32(base, address + 9824); }
  void set_part_child(uint32_t v) const { guest::StoreU32(base, address + 9824, v); }
  uint32_t part_child_address() const { return address + 9824; }
  uint32_t item_cursor_child() const { return guest::LoadU32(base, address + 9828); }
  void set_item_cursor_child(uint32_t v) const { guest::StoreU32(base, address + 9828, v); }
  uint32_t item_cursor_child_address() const { return address + 9828; }
  // the opening: played once
  uint32_t open_anim() const { return guest::LoadU32(base, address + 9832); }
  void set_open_anim(uint32_t v) const { guest::StoreU32(base, address + 9832, v); }
  uint32_t open_anim_address() const { return address + 9832; }
  // the leaving: played once
  uint32_t leave_anim() const { return guest::LoadU32(base, address + 9836); }
  void set_leave_anim(uint32_t v) const { guest::StoreU32(base, address + 9836, v); }
  uint32_t leave_anim_address() const { return address + 9836; }
  // type 7's step 2 (page 2 to 3)
  uint32_t first_to_items_anim() const { return guest::LoadU32(base, address + 9840); }
  void set_first_to_items_anim(uint32_t v) const { guest::StoreU32(base, address + 9840, v); }
  uint32_t first_to_items_anim_address() const { return address + 9840; }
  // type 7's step 3 (page 3 back to 2)
  uint32_t items_to_first_anim() const { return guest::LoadU32(base, address + 9844); }
  void set_items_to_first_anim(uint32_t v) const { guest::StoreU32(base, address + 9844, v); }
  uint32_t items_to_first_anim_address() const { return address + 9844; }
  // type 7's step 4 (page 3 to 4)
  uint32_t items_to_colors_anim() const { return guest::LoadU32(base, address + 9848); }
  void set_items_to_colors_anim(uint32_t v) const { guest::StoreU32(base, address + 9848, v); }
  uint32_t items_to_colors_anim_address() const { return address + 9848; }
  // type 7's step 6 (page 4 back to 3)
  uint32_t colors_to_items_anim() const { return guest::LoadU32(base, address + 9852); }
  void set_colors_to_items_anim(uint32_t v) const { guest::StoreU32(base, address + 9852, v); }
  uint32_t colors_to_items_anim_address() const { return address + 9852; }
  // the opening: played and looped
  uint32_t open_loop_a() const { return guest::LoadU32(base, address + 9856); }
  void set_open_loop_a(uint32_t v) const { guest::StoreU32(base, address + 9856, v); }
  uint32_t open_loop_a_address() const { return address + 9856; }
  // the opening: played and looped
  uint32_t open_loop_b() const { return guest::LoadU32(base, address + 9860); }
  void set_open_loop_b(uint32_t v) const { guest::StoreU32(base, address + 9860, v); }
  uint32_t open_loop_b_address() const { return address + 9860; }
  // the opening: chained
  uint32_t open_anim_b() const { return guest::LoadU32(base, address + 9864); }
  void set_open_anim_b(uint32_t v) const { guest::StoreU32(base, address + 9864, v); }
  uint32_t open_anim_b_address() const { return address + 9864; }
  // after the opening: looped
  uint32_t shown_loop_a() const { return guest::LoadU32(base, address + 9868); }
  void set_shown_loop_a(uint32_t v) const { guest::StoreU32(base, address + 9868, v); }
  uint32_t shown_loop_a_address() const { return address + 9868; }
  // the item list's loop (finished when it leaves)
  uint32_t items_loop() const { return guest::LoadU32(base, address + 9872); }
  void set_items_loop(uint32_t v) const { guest::StoreU32(base, address + 9872, v); }
  uint32_t items_loop_address() const { return address + 9872; }
  // after the opening: looped
  uint32_t shown_loop_b() const { return guest::LoadU32(base, address + 9876); }
  void set_shown_loop_b(uint32_t v) const { guest::StoreU32(base, address + 9876, v); }
  uint32_t shown_loop_b_address() const { return address + 9876; }
  // after the opening: looped
  uint32_t shown_loop_c() const { return guest::LoadU32(base, address + 9880); }
  void set_shown_loop_c(uint32_t v) const { guest::StoreU32(base, address + 9880, v); }
  uint32_t shown_loop_c_address() const { return address + 9880; }
  // after the opening: looped
  uint32_t shown_loop_d() const { return guest::LoadU32(base, address + 9884); }
  void set_shown_loop_d(uint32_t v) const { guest::StoreU32(base, address + 9884, v); }
  uint32_t shown_loop_d_address() const { return address + 9884; }
  // the opening: chained
  uint32_t open_anim_c() const { return guest::LoadU32(base, address + 9888); }
  void set_open_anim_c(uint32_t v) const { guest::StoreU32(base, address + 9888, v); }
  uint32_t open_anim_c_address() const { return address + 9888; }
  // the toggle's animation for its selection 0
  uint32_t toggle_anim_a() const { return guest::LoadU32(base, address + 9892); }
  void set_toggle_anim_a(uint32_t v) const { guest::StoreU32(base, address + 9892, v); }
  uint32_t toggle_anim_a_address() const { return address + 9892; }
  // the toggle's animation for its selection 1
  uint32_t toggle_anim_b() const { return guest::LoadU32(base, address + 9896); }
  void set_toggle_anim_b(uint32_t v) const { guest::StoreU32(base, address + 9896, v); }
  uint32_t toggle_anim_b_address() const { return address + 9896; }
  // replayed by ShowSelection
  uint32_t selection_anim() const { return guest::LoadU32(base, address + 9900); }
  void set_selection_anim(uint32_t v) const { guest::StoreU32(base, address + 9900, v); }
  uint32_t selection_anim_address() const { return address + 9900; }
  // an item list's move up
  uint32_t item_up_anim() const { return guest::LoadU32(base, address + 9904); }
  void set_item_up_anim(uint32_t v) const { guest::StoreU32(base, address + 9904, v); }
  uint32_t item_up_anim_address() const { return address + 9904; }
  // an item list's move down
  uint32_t item_down_anim() const { return guest::LoadU32(base, address + 9908); }
  void set_item_down_anim(uint32_t v) const { guest::StoreU32(base, address + 9908, v); }
  uint32_t item_down_anim_address() const { return address + 9908; }
  // type 7's first list moved back
  uint32_t first_back_anim() const { return guest::LoadU32(base, address + 9912); }
  void set_first_back_anim(uint32_t v) const { guest::StoreU32(base, address + 9912, v); }
  uint32_t first_back_anim_address() const { return address + 9912; }
  // type 7's first list moved forward
  uint32_t first_forward_anim() const { return guest::LoadU32(base, address + 9916); }
  void set_first_forward_anim(uint32_t v) const { guest::StoreU32(base, address + 9916, v); }
  uint32_t first_forward_anim_address() const { return address + 9916; }
  // type 7's color list moved back
  uint32_t color_back_anim() const { return guest::LoadU32(base, address + 9920); }
  void set_color_back_anim(uint32_t v) const { guest::StoreU32(base, address + 9920, v); }
  uint32_t color_back_anim_address() const { return address + 9920; }
  // type 7's color list moved forward
  uint32_t color_forward_anim() const { return guest::LoadU32(base, address + 9924); }
  void set_color_forward_anim(uint32_t v) const { guest::StoreU32(base, address + 9924, v); }
  uint32_t color_forward_anim_address() const { return address + 9924; }
  // a list scrolled down
  uint32_t scroll_down_anim() const { return guest::LoadU32(base, address + 9928); }
  void set_scroll_down_anim(uint32_t v) const { guest::StoreU32(base, address + 9928, v); }
  uint32_t scroll_down_anim_address() const { return address + 9928; }
  // a list scrolled up
  uint32_t scroll_up_anim() const { return guest::LoadU32(base, address + 9932); }
  void set_scroll_up_anim(uint32_t v) const { guest::StoreU32(base, address + 9932, v); }
  uint32_t scroll_up_anim_address() const { return address + 9932; }
  // ten named animations by the item list's selection (LoopItemNode)
  uint32_t item_anims_address() const { return address + 9936; }
  // item_anims[0]
  uint32_t item_anim_0() const { return guest::LoadU32(base, address + 9936); }
  void set_item_anim_0(uint32_t v) const { guest::StoreU32(base, address + 9936, v); }
  uint32_t item_anim_0_address() const { return address + 9936; }
  // ten by the first list's selection (LoopSecondNode)
  uint32_t first_anims_address() const { return address + 9976; }
  // first_anims[0]
  uint32_t first_anim_0() const { return guest::LoadU32(base, address + 9976); }
  void set_first_anim_0(uint32_t v) const { guest::StoreU32(base, address + 9976, v); }
  uint32_t first_anim_0_address() const { return address + 9976; }
  // ten by the color list's selection (LoopThirdNode)
  uint32_t color_anims_address() const { return address + 10016; }
  // color_anims[0]
  uint32_t color_anim_0() const { return guest::LoadU32(base, address + 10016); }
  void set_color_anim_0(uint32_t v) const { guest::StoreU32(base, address + 10016, v); }
  uint32_t color_anim_0_address() const { return address + 10016; }
  // ten by the item's selection (type 7's step 4 plays, step 6 finishes)
  uint32_t item_anims_b_address() const { return address + 10056; }
  // item_anims_b[0]
  uint32_t item_anim_b_0() const { return guest::LoadU32(base, address + 10056); }
  void set_item_anim_b_0(uint32_t v) const { guest::StoreU32(base, address + 10056, v); }
  uint32_t item_anim_b_0_address() const { return address + 10056; }
  // item_anims_b[1]
  uint32_t item_anim_b_1() const { return guest::LoadU32(base, address + 10060); }
  void set_item_anim_b_1(uint32_t v) const { guest::StoreU32(base, address + 10060, v); }
  uint32_t item_anim_b_1_address() const { return address + 10060; }
  // item_anims_b[2]
  uint32_t item_anim_b_2() const { return guest::LoadU32(base, address + 10064); }
  void set_item_anim_b_2(uint32_t v) const { guest::StoreU32(base, address + 10064, v); }
  uint32_t item_anim_b_2_address() const { return address + 10064; }
  // ten by the item's selection (type 7's step 4 finishes, step 6 plays)
  uint32_t item_anims_c_address() const { return address + 10096; }
  // item_anims_c[0]
  uint32_t item_anim_c_0() const { return guest::LoadU32(base, address + 10096); }
  void set_item_anim_c_0(uint32_t v) const { guest::StoreU32(base, address + 10096, v); }
  uint32_t item_anim_c_0_address() const { return address + 10096; }
  // item_anims_c[1]
  uint32_t item_anim_c_1() const { return guest::LoadU32(base, address + 10100); }
  void set_item_anim_c_1(uint32_t v) const { guest::StoreU32(base, address + 10100, v); }
  uint32_t item_anim_c_1_address() const { return address + 10100; }
  // item_anims_c[2]
  uint32_t item_anim_c_2() const { return guest::LoadU32(base, address + 10104); }
  void set_item_anim_c_2(uint32_t v) const { guest::StoreU32(base, address + 10104, v); }
  uint32_t item_anim_c_2_address() const { return address + 10104; }
  // cleared by ClearScrollText
  uint32_t scroll_text_shown() const { return guest::LoadU32(base, address + 10136); }
  void set_scroll_text_shown(uint32_t v) const { guest::StoreU32(base, address + 10136, v); }
  uint32_t scroll_text_shown_address() const { return address + 10136; }
  // the scroll text shown (-1 none)
  uint32_t scroll_text() const { return guest::LoadU32(base, address + 10140); }
  void set_scroll_text(uint32_t v) const { guest::StoreU32(base, address + 10140, v); }
  uint32_t scroll_text_address() const { return address + 10140; }
  // the race kind's list (the race data's +32) (a MenuList, race-screens.toml)
  uint32_t kind_list_address() const { return address + 10144; }
  // the list's +4 (scroll)
  uint32_t kind_list_scroll() const { return guest::LoadU32(base, address + 10148); }
  void set_kind_list_scroll(uint32_t v) const { guest::StoreU32(base, address + 10148, v); }
  uint32_t kind_list_scroll_address() const { return address + 10148; }
  // the list's +8 (scroll_before)
  uint32_t kind_list_scroll_before() const { return guest::LoadU32(base, address + 10152); }
  void set_kind_list_scroll_before(uint32_t v) const { guest::StoreU32(base, address + 10152, v); }
  uint32_t kind_list_scroll_before_address() const { return address + 10152; }
  // the list's +24 (move)
  uint32_t kind_list_move() const { return guest::LoadU32(base, address + 10168); }
  void set_kind_list_move(uint32_t v) const { guest::StoreU32(base, address + 10168, v); }
  uint32_t kind_list_move_address() const { return address + 10168; }
  // the list's +32 (wrapped)
  uint32_t kind_list_wrapped() const { return guest::LoadU32(base, address + 10176); }
  void set_kind_list_wrapped(uint32_t v) const { guest::StoreU32(base, address + 10176, v); }
  uint32_t kind_list_wrapped_address() const { return address + 10176; }
  // the list's +60 (player)
  uint32_t kind_list_player() const { return guest::LoadU32(base, address + 10204); }
  void set_kind_list_player(uint32_t v) const { guest::StoreU32(base, address + 10204, v); }
  uint32_t kind_list_player_address() const { return address + 10204; }
  // the list's +68 (enabled)
  uint32_t kind_list_enabled() const { return guest::LoadU32(base, address + 10212); }
  void set_kind_list_enabled(uint32_t v) const { guest::StoreU32(base, address + 10212, v); }
  uint32_t kind_list_enabled_address() const { return address + 10212; }
  // the course list, 8 items (the route by its scroll and selection) (a MenuList, race-screens.toml)
  uint32_t course_list_address() const { return address + 10216; }
  // the list's +4 (scroll)
  uint32_t course_list_scroll() const { return guest::LoadU32(base, address + 10220); }
  void set_course_list_scroll(uint32_t v) const { guest::StoreU32(base, address + 10220, v); }
  uint32_t course_list_scroll_address() const { return address + 10220; }
  // the list's +8 (scroll_before)
  uint32_t course_list_scroll_before() const { return guest::LoadU32(base, address + 10224); }
  void set_course_list_scroll_before(uint32_t v) const { guest::StoreU32(base, address + 10224, v); }
  uint32_t course_list_scroll_before_address() const { return address + 10224; }
  // the list's +24 (move)
  uint32_t course_list_move() const { return guest::LoadU32(base, address + 10240); }
  void set_course_list_move(uint32_t v) const { guest::StoreU32(base, address + 10240, v); }
  uint32_t course_list_move_address() const { return address + 10240; }
  // the list's +32 (wrapped)
  uint32_t course_list_wrapped() const { return guest::LoadU32(base, address + 10248); }
  void set_course_list_wrapped(uint32_t v) const { guest::StoreU32(base, address + 10248, v); }
  uint32_t course_list_wrapped_address() const { return address + 10248; }
  // the list's +60 (player)
  uint32_t course_list_player() const { return guest::LoadU32(base, address + 10276); }
  void set_course_list_player(uint32_t v) const { guest::StoreU32(base, address + 10276, v); }
  uint32_t course_list_player_address() const { return address + 10276; }
  // the list's +68 (enabled)
  uint32_t course_list_enabled() const { return guest::LoadU32(base, address + 10284); }
  void set_course_list_enabled(uint32_t v) const { guest::StoreU32(base, address + 10284, v); }
  uint32_t course_list_enabled_address() const { return address + 10284; }
  // a two-item toggle (its selection 0: the race data's +0x59618 set) (a MenuList, race-screens.toml)
  uint32_t toggle_address() const { return address + 10288; }
  // the list's +4 (scroll)
  uint32_t toggle_scroll() const { return guest::LoadU32(base, address + 10292); }
  void set_toggle_scroll(uint32_t v) const { guest::StoreU32(base, address + 10292, v); }
  uint32_t toggle_scroll_address() const { return address + 10292; }
  // the list's +8 (scroll_before)
  uint32_t toggle_scroll_before() const { return guest::LoadU32(base, address + 10296); }
  void set_toggle_scroll_before(uint32_t v) const { guest::StoreU32(base, address + 10296, v); }
  uint32_t toggle_scroll_before_address() const { return address + 10296; }
  // the list's +24 (move)
  uint32_t toggle_move() const { return guest::LoadU32(base, address + 10312); }
  void set_toggle_move(uint32_t v) const { guest::StoreU32(base, address + 10312, v); }
  uint32_t toggle_move_address() const { return address + 10312; }
  // the list's +32 (wrapped)
  uint32_t toggle_wrapped() const { return guest::LoadU32(base, address + 10320); }
  void set_toggle_wrapped(uint32_t v) const { guest::StoreU32(base, address + 10320, v); }
  uint32_t toggle_wrapped_address() const { return address + 10320; }
  // the list's +60 (player)
  uint32_t toggle_player() const { return guest::LoadU32(base, address + 10348); }
  void set_toggle_player(uint32_t v) const { guest::StoreU32(base, address + 10348, v); }
  uint32_t toggle_player_address() const { return address + 10348; }
  // the list's +68 (enabled)
  uint32_t toggle_enabled() const { return guest::LoadU32(base, address + 10356); }
  void set_toggle_enabled(uint32_t v) const { guest::StoreU32(base, address + 10356, v); }
  uint32_t toggle_enabled_address() const { return address + 10356; }
  // the PairedNodeTable node_table points to
  uint32_t paired_table_address() const { return address + 10360; }
  // the UI movie's id
  uint32_t table_movie() const { return guest::LoadU32(base, address + 10364); }
  void set_table_movie(uint32_t v) const { guest::StoreU32(base, address + 10364, v); }
  uint32_t table_movie_address() const { return address + 10364; }
  // the sub-scene of the extra nodes (+420, +424, +264)
  uint32_t table_node_set_a() const { return guest::LoadU32(base, address + 10368); }
  void set_table_node_set_a(uint32_t v) const { guest::StoreU32(base, address + 10368, v); }
  uint32_t table_node_set_a_address() const { return address + 10368; }
  // the sub-scene of the items' nodes
  uint32_t table_node_set_b() const { return guest::LoadU32(base, address + 10372); }
  void set_table_node_set_b(uint32_t v) const { guest::StoreU32(base, address + 10372, v); }
  uint32_t table_node_set_b_address() const { return address + 10372; }
  // 27 items' named animations
  uint32_t table_item_anims_address() const { return address + 10376; }
  // eight more (the third pair's, +444)
  uint32_t table_more_anims_address() const { return address + 10484; }
  // 27 items' nodes
  uint32_t table_item_nodes_address() const { return address + 10516; }
  // eight nodes (hidden at the reset)
  uint32_t table_nodes_address() const { return address + 10624; }
  // the four children's indexes (marks and colors)
  uint32_t table_children_address() const { return address + 10656; }
  // 27 items' masks of the four children
  uint32_t table_item_masks_address() const { return address + 10672; }
  // the item nodes' twins on node_set_a (-1 none)
  uint32_t table_extra_node_base() const { return guest::LoadU32(base, address + 10780); }
  void set_table_extra_node_base(uint32_t v) const { guest::StoreU32(base, address + 10780, v); }
  uint32_t table_extra_node_base_address() const { return address + 10780; }
  // four text nodes (strings 402, 401, 407, 408; -1 none)
  uint32_t table_text_node_base() const { return guest::LoadU32(base, address + 10784); }
  void set_table_text_node_base(uint32_t v) const { guest::StoreU32(base, address + 10784, v); }
  uint32_t table_text_node_base_address() const { return address + 10784; }
  // the first pair: the item now
  uint32_t table_item() const { return guest::LoadU32(base, address + 10788); }
  void set_table_item(uint32_t v) const { guest::StoreU32(base, address + 10788, v); }
  uint32_t table_item_address() const { return address + 10788; }
  // the item before
  uint32_t table_item_before() const { return guest::LoadU32(base, address + 10792); }
  void set_table_item_before(uint32_t v) const { guest::StoreU32(base, address + 10792, v); }
  uint32_t table_item_before_address() const { return address + 10792; }
  // the second pair: an item now
  uint32_t table_second() const { return guest::LoadU32(base, address + 10796); }
  void set_table_second(uint32_t v) const { guest::StoreU32(base, address + 10796, v); }
  uint32_t table_second_address() const { return address + 10796; }
  uint32_t table_second_before() const { return guest::LoadU32(base, address + 10800); }
  void set_table_second_before(uint32_t v) const { guest::StoreU32(base, address + 10800, v); }
  uint32_t table_second_before_address() const { return address + 10800; }
  // the third pair: an index into more_anims now
  uint32_t table_third() const { return guest::LoadU32(base, address + 10804); }
  void set_table_third(uint32_t v) const { guest::StoreU32(base, address + 10804, v); }
  uint32_t table_third_address() const { return address + 10804; }
  uint32_t table_third_before() const { return guest::LoadU32(base, address + 10808); }
  void set_table_third_before(uint32_t v) const { guest::StoreU32(base, address + 10808, v); }
  uint32_t table_third_before_address() const { return address + 10808; }
  // the fourth pair: the paired table's mask bit; the color table's grid column
  uint32_t table_fourth() const { return guest::LoadU32(base, address + 10812); }
  void set_table_fourth(uint32_t v) const { guest::StoreU32(base, address + 10812, v); }
  uint32_t table_fourth_address() const { return address + 10812; }
  uint32_t table_fourth_before() const { return guest::LoadU32(base, address + 10816); }
  void set_table_fourth_before(uint32_t v) const { guest::StoreU32(base, address + 10816, v); }
  uint32_t table_fourth_before_address() const { return address + 10816; }
};

// embedded; measure: TitledListScreen, Type6ListScreen
struct Type6ListScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 10964;
  static constexpr uint32_t kVtable = 0x820109A8;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // a ScreenMachine (boot-mode.md)
  uint32_t machine_address() const { return address + 4; }
  // the machine's state (its +9604)
  uint32_t state() const { return guest::LoadU32(base, address + 9608); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t state_address() const { return address + 9608; }
  uint32_t movie_loaded() const { return guest::LoadU32(base, address + 9612); }
  void set_movie_loaded(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t movie_loaded_address() const { return address + 9612; }
  // open the movie at the mode's start (else wait for the open message)
  uint32_t open_at_start() const { return guest::LoadU32(base, address + 9616); }
  void set_open_at_start(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t open_at_start_address() const { return address + 9616; }
  // the page Draw runs (-1 while a step runs)
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
  // the step Update runs (-1 none)
  uint32_t step() const { return guest::LoadU32(base, address + 9656); }
  void set_step(uint32_t v) const { guest::StoreU32(base, address + 9656, v); }
  uint32_t step_address() const { return address + 9656; }
  // the step after it (-1 none)
  uint32_t queued_step() const { return guest::LoadU32(base, address + 9660); }
  void set_queued_step(uint32_t v) const { guest::StoreU32(base, address + 9660, v); }
  uint32_t queued_step_address() const { return address + 9660; }
  // the page block's +20: 0 when a queued step starts
  uint32_t unknown_9664() const { return guest::LoadU32(base, address + 9664); }
  void set_unknown_9664(uint32_t v) const { guest::StoreU32(base, address + 9664, v); }
  uint32_t unknown_9664_address() const { return address + 9664; }
  // set by a step once its first call ran; 0 when the block moves on
  uint32_t step_started() const { return guest::LoadU32(base, address + 9668); }
  void set_step_started(uint32_t v) const { guest::StoreU32(base, address + 9668, v); }
  uint32_t step_started_address() const { return address + 9668; }
  // the node table (0 none): the type 6 list's at +10360, the type 7 list's at +10432
  template <typename T = NodeTable>
  T node_table() const { return T(base, guest::LoadU32(base, address + 9672)); }
  void set_node_table(const View& v) const { guest::StoreU32(base, address + 9672, v.address); }
  void set_node_table(uint32_t v) const { guest::StoreU32(base, address + 9672, v); }
  uint32_t node_table_address() const { return address + 9672; }
  // the list part (0 none): +10824 type 6, +11040 type 7
  template <typename T = ListPart>
  T list_part() const { return T(base, guest::LoadU32(base, address + 9676)); }
  void set_list_part(const View& v) const { guest::StoreU32(base, address + 9676, v.address); }
  void set_list_part(uint32_t v) const { guest::StoreU32(base, address + 9676, v); }
  uint32_t list_part_address() const { return address + 9676; }
  // the UI movie's id (1 at the constructor; 25 the node list)
  uint32_t movie() const { return guest::LoadU32(base, address + 9680); }
  void set_movie(uint32_t v) const { guest::StoreU32(base, address + 9680, v); }
  uint32_t movie_address() const { return address + 9680; }
  // 1 at the constructor, 0 type 7
  uint32_t unknown_9684() const { return guest::LoadU32(base, address + 9684); }
  void set_unknown_9684(uint32_t v) const { guest::StoreU32(base, address + 9684, v); }
  uint32_t unknown_9684_address() const { return address + 9684; }
  // 1 when the race was set (the part's yes): the screen ends in state 11
  uint32_t race_set() const { return guest::LoadU32(base, address + 9688); }
  void set_race_set(uint32_t v) const { guest::StoreU32(base, address + 9688, v); }
  uint32_t race_set_address() const { return address + 9688; }
  // 1 once the movie loaded
  uint32_t movie_ready() const { return guest::LoadU32(base, address + 9692); }
  void set_movie_ready(uint32_t v) const { guest::StoreU32(base, address + 9692, v); }
  uint32_t movie_ready_address() const { return address + 9692; }
  // the list part's input on
  uint32_t part_input() const { return guest::LoadU32(base, address + 9696); }
  void set_part_input(uint32_t v) const { guest::StoreU32(base, address + 9696, v); }
  uint32_t part_input_address() const { return address + 9696; }
  // bit 0 of the item info's +12
  uint32_t item_bit_0() const { return guest::LoadU32(base, address + 9700); }
  void set_item_bit_0(uint32_t v) const { guest::StoreU32(base, address + 9700, v); }
  uint32_t item_bit_0_address() const { return address + 9700; }
  // the same test, ANDed with the info's +12
  uint32_t item_bit_1() const { return guest::LoadU32(base, address + 9704); }
  void set_item_bit_1(uint32_t v) const { guest::StoreU32(base, address + 9704, v); }
  uint32_t item_bit_1_address() const { return address + 9704; }
  // type 7: show the part's nodes with it, and skip the leaving step after its yes
  uint32_t part_nodes() const { return guest::LoadU32(base, address + 9708); }
  void set_part_nodes(uint32_t v) const { guest::StoreU32(base, address + 9708, v); }
  uint32_t part_nodes_address() const { return address + 9708; }
  // the title text's child (string 322) on text_sub_scene
  uint32_t title_child() const { return guest::LoadU32(base, address + 9712); }
  void set_title_child(uint32_t v) const { guest::StoreU32(base, address + 9712, v); }
  uint32_t title_child_address() const { return address + 9712; }
  // a text's child (text 0; 0 none)
  uint32_t text_child_a() const { return guest::LoadU32(base, address + 9716); }
  void set_text_child_a(uint32_t v) const { guest::StoreU32(base, address + 9716, v); }
  uint32_t text_child_a_address() const { return address + 9716; }
  // a text's child (text 1; 0 none)
  uint32_t text_child_b() const { return guest::LoadU32(base, address + 9720); }
  void set_text_child_b(uint32_t v) const { guest::StoreU32(base, address + 9720, v); }
  uint32_t text_child_b_address() const { return address + 9720; }
  // the selection nodes' sub-scene (-1 none)
  uint32_t selection_sub_scene() const { return guest::LoadU32(base, address + 9724); }
  void set_selection_sub_scene(uint32_t v) const { guest::StoreU32(base, address + 9724, v); }
  uint32_t selection_sub_scene_address() const { return address + 9724; }
  // the titles' and arrows' sub-scene
  uint32_t text_sub_scene() const { return guest::LoadU32(base, address + 9728); }
  void set_text_sub_scene(uint32_t v) const { guest::StoreU32(base, address + 9728, v); }
  uint32_t text_sub_scene_address() const { return address + 9728; }
  uint32_t icon_sub_scene() const { return guest::LoadU32(base, address + 9732); }
  void set_icon_sub_scene(uint32_t v) const { guest::StoreU32(base, address + 9732, v); }
  uint32_t icon_sub_scene_address() const { return address + 9732; }
  // the color cursor's node; the sub-scene of color_cursor_child
  uint32_t color_cursor_node() const { return guest::LoadU32(base, address + 9736); }
  void set_color_cursor_node(uint32_t v) const { guest::StoreU32(base, address + 9736, v); }
  uint32_t color_cursor_node_address() const { return address + 9736; }
  // the scroll text's sub-scene
  uint32_t scroll_sub_scene() const { return guest::LoadU32(base, address + 9740); }
  void set_scroll_sub_scene(uint32_t v) const { guest::StoreU32(base, address + 9740, v); }
  uint32_t scroll_sub_scene_address() const { return address + 9740; }
  // the item cursor's node
  uint32_t item_cursor_node() const { return guest::LoadU32(base, address + 9744); }
  void set_item_cursor_node(uint32_t v) const { guest::StoreU32(base, address + 9744, v); }
  uint32_t item_cursor_node_address() const { return address + 9744; }
  // a child on selection_sub_scene shown by shown_child_on
  uint32_t shown_child() const { return guest::LoadU32(base, address + 9748); }
  void set_shown_child(uint32_t v) const { guest::StoreU32(base, address + 9748, v); }
  uint32_t shown_child_address() const { return address + 9748; }
  // the arrow SetArrows(0) sets
  uint32_t arrow_child_a() const { return guest::LoadU32(base, address + 9752); }
  void set_arrow_child_a(uint32_t v) const { guest::StoreU32(base, address + 9752, v); }
  uint32_t arrow_child_a_address() const { return address + 9752; }
  // the arrow SetArrows(1) sets
  uint32_t arrow_child_b() const { return guest::LoadU32(base, address + 9756); }
  void set_arrow_child_b(uint32_t v) const { guest::StoreU32(base, address + 9756, v); }
  uint32_t arrow_child_b_address() const { return address + 9756; }
  // a node (no sub-scene) given the selection
  uint32_t selection_node() const { return guest::LoadU32(base, address + 9760); }
  void set_selection_node(uint32_t v) const { guest::StoreU32(base, address + 9760, v); }
  uint32_t selection_node_address() const { return address + 9760; }
  // a child given the previous selection
  uint32_t previous_child() const { return guest::LoadU32(base, address + 9764); }
  void set_previous_child(uint32_t v) const { guest::StoreU32(base, address + 9764, v); }
  uint32_t previous_child_address() const { return address + 9764; }
  // a child given the selection
  uint32_t selection_child() const { return guest::LoadU32(base, address + 9768); }
  void set_selection_child(uint32_t v) const { guest::StoreU32(base, address + 9768, v); }
  uint32_t selection_child_address() const { return address + 9768; }
  // an array of children_count children given the previous selection (0 none)
  uint32_t previous_children() const { return guest::LoadU32(base, address + 9772); }
  void set_previous_children(uint32_t v) const { guest::StoreU32(base, address + 9772, v); }
  uint32_t previous_children_address() const { return address + 9772; }
  // an array given the selection (0 none; type 7 0x826371A4)
  uint32_t selection_children() const { return guest::LoadU32(base, address + 9776); }
  void set_selection_children(uint32_t v) const { guest::StoreU32(base, address + 9776, v); }
  uint32_t selection_children_address() const { return address + 9776; }
  // not written by the constructor
  uint32_t unknown_9780() const { return guest::LoadU32(base, address + 9780); }
  void set_unknown_9780(uint32_t v) const { guest::StoreU32(base, address + 9780, v); }
  uint32_t unknown_9780_address() const { return address + 9780; }
  uint32_t children_count() const { return guest::LoadU32(base, address + 9784); }
  void set_children_count(uint32_t v) const { guest::StoreU32(base, address + 9784, v); }
  uint32_t children_count_address() const { return address + 9784; }
  uint32_t shown_child_on() const { return guest::LoadU32(base, address + 9788); }
  void set_shown_child_on(uint32_t v) const { guest::StoreU32(base, address + 9788, v); }
  uint32_t shown_child_on_address() const { return address + 9788; }
  uint32_t unknown_9792() const { return guest::LoadU32(base, address + 9792); }
  void set_unknown_9792(uint32_t v) const { guest::StoreU32(base, address + 9792, v); }
  uint32_t unknown_9792_address() const { return address + 9792; }
  uint32_t unknown_9796() const { return guest::LoadU32(base, address + 9796); }
  void set_unknown_9796(uint32_t v) const { guest::StoreU32(base, address + 9796, v); }
  uint32_t unknown_9796_address() const { return address + 9796; }
  // the arrows' +108 when on
  uint32_t arrow_on_frame() const { return guest::LoadU32(base, address + 9800); }
  void set_arrow_on_frame(uint32_t v) const { guest::StoreU32(base, address + 9800, v); }
  uint32_t arrow_on_frame_address() const { return address + 9800; }
  // the arrows' +108 when off
  uint32_t arrow_off_frame() const { return guest::LoadU32(base, address + 9804); }
  void set_arrow_off_frame(uint32_t v) const { guest::StoreU32(base, address + 9804, v); }
  uint32_t arrow_off_frame_address() const { return address + 9804; }
  uint32_t color_cursor_child() const { return guest::LoadU32(base, address + 9808); }
  void set_color_cursor_child(uint32_t v) const { guest::StoreU32(base, address + 9808, v); }
  uint32_t color_cursor_child_address() const { return address + 9808; }
  // the first of six icon children (four textures, two hidden)
  uint32_t icon_child() const { return guest::LoadU32(base, address + 9812); }
  void set_icon_child(uint32_t v) const { guest::StoreU32(base, address + 9812, v); }
  uint32_t icon_child_address() const { return address + 9812; }
  // the scroll text's child
  uint32_t scroll_child() const { return guest::LoadU32(base, address + 9816); }
  void set_scroll_child(uint32_t v) const { guest::StoreU32(base, address + 9816, v); }
  uint32_t scroll_child_address() const { return address + 9816; }
  uint32_t unknown_9820() const { return guest::LoadU32(base, address + 9820); }
  void set_unknown_9820(uint32_t v) const { guest::StoreU32(base, address + 9820, v); }
  uint32_t unknown_9820_address() const { return address + 9820; }
  // type 7: a child on text_sub_scene shown with the part
  uint32_t part_child() const { return guest::LoadU32(base, address + 9824); }
  void set_part_child(uint32_t v) const { guest::StoreU32(base, address + 9824, v); }
  uint32_t part_child_address() const { return address + 9824; }
  uint32_t item_cursor_child() const { return guest::LoadU32(base, address + 9828); }
  void set_item_cursor_child(uint32_t v) const { guest::StoreU32(base, address + 9828, v); }
  uint32_t item_cursor_child_address() const { return address + 9828; }
  // the opening: played once
  uint32_t open_anim() const { return guest::LoadU32(base, address + 9832); }
  void set_open_anim(uint32_t v) const { guest::StoreU32(base, address + 9832, v); }
  uint32_t open_anim_address() const { return address + 9832; }
  // the leaving: played once
  uint32_t leave_anim() const { return guest::LoadU32(base, address + 9836); }
  void set_leave_anim(uint32_t v) const { guest::StoreU32(base, address + 9836, v); }
  uint32_t leave_anim_address() const { return address + 9836; }
  // type 7's step 2 (page 2 to 3)
  uint32_t first_to_items_anim() const { return guest::LoadU32(base, address + 9840); }
  void set_first_to_items_anim(uint32_t v) const { guest::StoreU32(base, address + 9840, v); }
  uint32_t first_to_items_anim_address() const { return address + 9840; }
  // type 7's step 3 (page 3 back to 2)
  uint32_t items_to_first_anim() const { return guest::LoadU32(base, address + 9844); }
  void set_items_to_first_anim(uint32_t v) const { guest::StoreU32(base, address + 9844, v); }
  uint32_t items_to_first_anim_address() const { return address + 9844; }
  // type 7's step 4 (page 3 to 4)
  uint32_t items_to_colors_anim() const { return guest::LoadU32(base, address + 9848); }
  void set_items_to_colors_anim(uint32_t v) const { guest::StoreU32(base, address + 9848, v); }
  uint32_t items_to_colors_anim_address() const { return address + 9848; }
  // type 7's step 6 (page 4 back to 3)
  uint32_t colors_to_items_anim() const { return guest::LoadU32(base, address + 9852); }
  void set_colors_to_items_anim(uint32_t v) const { guest::StoreU32(base, address + 9852, v); }
  uint32_t colors_to_items_anim_address() const { return address + 9852; }
  // the opening: played and looped
  uint32_t open_loop_a() const { return guest::LoadU32(base, address + 9856); }
  void set_open_loop_a(uint32_t v) const { guest::StoreU32(base, address + 9856, v); }
  uint32_t open_loop_a_address() const { return address + 9856; }
  // the opening: played and looped
  uint32_t open_loop_b() const { return guest::LoadU32(base, address + 9860); }
  void set_open_loop_b(uint32_t v) const { guest::StoreU32(base, address + 9860, v); }
  uint32_t open_loop_b_address() const { return address + 9860; }
  // the opening: chained
  uint32_t open_anim_b() const { return guest::LoadU32(base, address + 9864); }
  void set_open_anim_b(uint32_t v) const { guest::StoreU32(base, address + 9864, v); }
  uint32_t open_anim_b_address() const { return address + 9864; }
  // after the opening: looped
  uint32_t shown_loop_a() const { return guest::LoadU32(base, address + 9868); }
  void set_shown_loop_a(uint32_t v) const { guest::StoreU32(base, address + 9868, v); }
  uint32_t shown_loop_a_address() const { return address + 9868; }
  // the item list's loop (finished when it leaves)
  uint32_t items_loop() const { return guest::LoadU32(base, address + 9872); }
  void set_items_loop(uint32_t v) const { guest::StoreU32(base, address + 9872, v); }
  uint32_t items_loop_address() const { return address + 9872; }
  // after the opening: looped
  uint32_t shown_loop_b() const { return guest::LoadU32(base, address + 9876); }
  void set_shown_loop_b(uint32_t v) const { guest::StoreU32(base, address + 9876, v); }
  uint32_t shown_loop_b_address() const { return address + 9876; }
  // after the opening: looped
  uint32_t shown_loop_c() const { return guest::LoadU32(base, address + 9880); }
  void set_shown_loop_c(uint32_t v) const { guest::StoreU32(base, address + 9880, v); }
  uint32_t shown_loop_c_address() const { return address + 9880; }
  // after the opening: looped
  uint32_t shown_loop_d() const { return guest::LoadU32(base, address + 9884); }
  void set_shown_loop_d(uint32_t v) const { guest::StoreU32(base, address + 9884, v); }
  uint32_t shown_loop_d_address() const { return address + 9884; }
  // the opening: chained
  uint32_t open_anim_c() const { return guest::LoadU32(base, address + 9888); }
  void set_open_anim_c(uint32_t v) const { guest::StoreU32(base, address + 9888, v); }
  uint32_t open_anim_c_address() const { return address + 9888; }
  // the toggle's animation for its selection 0
  uint32_t toggle_anim_a() const { return guest::LoadU32(base, address + 9892); }
  void set_toggle_anim_a(uint32_t v) const { guest::StoreU32(base, address + 9892, v); }
  uint32_t toggle_anim_a_address() const { return address + 9892; }
  // the toggle's animation for its selection 1
  uint32_t toggle_anim_b() const { return guest::LoadU32(base, address + 9896); }
  void set_toggle_anim_b(uint32_t v) const { guest::StoreU32(base, address + 9896, v); }
  uint32_t toggle_anim_b_address() const { return address + 9896; }
  // replayed by ShowSelection
  uint32_t selection_anim() const { return guest::LoadU32(base, address + 9900); }
  void set_selection_anim(uint32_t v) const { guest::StoreU32(base, address + 9900, v); }
  uint32_t selection_anim_address() const { return address + 9900; }
  // an item list's move up
  uint32_t item_up_anim() const { return guest::LoadU32(base, address + 9904); }
  void set_item_up_anim(uint32_t v) const { guest::StoreU32(base, address + 9904, v); }
  uint32_t item_up_anim_address() const { return address + 9904; }
  // an item list's move down
  uint32_t item_down_anim() const { return guest::LoadU32(base, address + 9908); }
  void set_item_down_anim(uint32_t v) const { guest::StoreU32(base, address + 9908, v); }
  uint32_t item_down_anim_address() const { return address + 9908; }
  // type 7's first list moved back
  uint32_t first_back_anim() const { return guest::LoadU32(base, address + 9912); }
  void set_first_back_anim(uint32_t v) const { guest::StoreU32(base, address + 9912, v); }
  uint32_t first_back_anim_address() const { return address + 9912; }
  // type 7's first list moved forward
  uint32_t first_forward_anim() const { return guest::LoadU32(base, address + 9916); }
  void set_first_forward_anim(uint32_t v) const { guest::StoreU32(base, address + 9916, v); }
  uint32_t first_forward_anim_address() const { return address + 9916; }
  // type 7's color list moved back
  uint32_t color_back_anim() const { return guest::LoadU32(base, address + 9920); }
  void set_color_back_anim(uint32_t v) const { guest::StoreU32(base, address + 9920, v); }
  uint32_t color_back_anim_address() const { return address + 9920; }
  // type 7's color list moved forward
  uint32_t color_forward_anim() const { return guest::LoadU32(base, address + 9924); }
  void set_color_forward_anim(uint32_t v) const { guest::StoreU32(base, address + 9924, v); }
  uint32_t color_forward_anim_address() const { return address + 9924; }
  // a list scrolled down
  uint32_t scroll_down_anim() const { return guest::LoadU32(base, address + 9928); }
  void set_scroll_down_anim(uint32_t v) const { guest::StoreU32(base, address + 9928, v); }
  uint32_t scroll_down_anim_address() const { return address + 9928; }
  // a list scrolled up
  uint32_t scroll_up_anim() const { return guest::LoadU32(base, address + 9932); }
  void set_scroll_up_anim(uint32_t v) const { guest::StoreU32(base, address + 9932, v); }
  uint32_t scroll_up_anim_address() const { return address + 9932; }
  // ten named animations by the item list's selection (LoopItemNode)
  uint32_t item_anims_address() const { return address + 9936; }
  // item_anims[0]
  uint32_t item_anim_0() const { return guest::LoadU32(base, address + 9936); }
  void set_item_anim_0(uint32_t v) const { guest::StoreU32(base, address + 9936, v); }
  uint32_t item_anim_0_address() const { return address + 9936; }
  // ten by the first list's selection (LoopSecondNode)
  uint32_t first_anims_address() const { return address + 9976; }
  // first_anims[0]
  uint32_t first_anim_0() const { return guest::LoadU32(base, address + 9976); }
  void set_first_anim_0(uint32_t v) const { guest::StoreU32(base, address + 9976, v); }
  uint32_t first_anim_0_address() const { return address + 9976; }
  // ten by the color list's selection (LoopThirdNode)
  uint32_t color_anims_address() const { return address + 10016; }
  // color_anims[0]
  uint32_t color_anim_0() const { return guest::LoadU32(base, address + 10016); }
  void set_color_anim_0(uint32_t v) const { guest::StoreU32(base, address + 10016, v); }
  uint32_t color_anim_0_address() const { return address + 10016; }
  // ten by the item's selection (type 7's step 4 plays, step 6 finishes)
  uint32_t item_anims_b_address() const { return address + 10056; }
  // item_anims_b[0]
  uint32_t item_anim_b_0() const { return guest::LoadU32(base, address + 10056); }
  void set_item_anim_b_0(uint32_t v) const { guest::StoreU32(base, address + 10056, v); }
  uint32_t item_anim_b_0_address() const { return address + 10056; }
  // item_anims_b[1]
  uint32_t item_anim_b_1() const { return guest::LoadU32(base, address + 10060); }
  void set_item_anim_b_1(uint32_t v) const { guest::StoreU32(base, address + 10060, v); }
  uint32_t item_anim_b_1_address() const { return address + 10060; }
  // item_anims_b[2]
  uint32_t item_anim_b_2() const { return guest::LoadU32(base, address + 10064); }
  void set_item_anim_b_2(uint32_t v) const { guest::StoreU32(base, address + 10064, v); }
  uint32_t item_anim_b_2_address() const { return address + 10064; }
  // ten by the item's selection (type 7's step 4 finishes, step 6 plays)
  uint32_t item_anims_c_address() const { return address + 10096; }
  // item_anims_c[0]
  uint32_t item_anim_c_0() const { return guest::LoadU32(base, address + 10096); }
  void set_item_anim_c_0(uint32_t v) const { guest::StoreU32(base, address + 10096, v); }
  uint32_t item_anim_c_0_address() const { return address + 10096; }
  // item_anims_c[1]
  uint32_t item_anim_c_1() const { return guest::LoadU32(base, address + 10100); }
  void set_item_anim_c_1(uint32_t v) const { guest::StoreU32(base, address + 10100, v); }
  uint32_t item_anim_c_1_address() const { return address + 10100; }
  // item_anims_c[2]
  uint32_t item_anim_c_2() const { return guest::LoadU32(base, address + 10104); }
  void set_item_anim_c_2(uint32_t v) const { guest::StoreU32(base, address + 10104, v); }
  uint32_t item_anim_c_2_address() const { return address + 10104; }
  // cleared by ClearScrollText
  uint32_t scroll_text_shown() const { return guest::LoadU32(base, address + 10136); }
  void set_scroll_text_shown(uint32_t v) const { guest::StoreU32(base, address + 10136, v); }
  uint32_t scroll_text_shown_address() const { return address + 10136; }
  // the scroll text shown (-1 none)
  uint32_t scroll_text() const { return guest::LoadU32(base, address + 10140); }
  void set_scroll_text(uint32_t v) const { guest::StoreU32(base, address + 10140, v); }
  uint32_t scroll_text_address() const { return address + 10140; }
  // the race kind's list (the race data's +32) (a MenuList, race-screens.toml)
  uint32_t kind_list_address() const { return address + 10144; }
  // the list's +4 (scroll)
  uint32_t kind_list_scroll() const { return guest::LoadU32(base, address + 10148); }
  void set_kind_list_scroll(uint32_t v) const { guest::StoreU32(base, address + 10148, v); }
  uint32_t kind_list_scroll_address() const { return address + 10148; }
  // the list's +8 (scroll_before)
  uint32_t kind_list_scroll_before() const { return guest::LoadU32(base, address + 10152); }
  void set_kind_list_scroll_before(uint32_t v) const { guest::StoreU32(base, address + 10152, v); }
  uint32_t kind_list_scroll_before_address() const { return address + 10152; }
  // the list's +24 (move)
  uint32_t kind_list_move() const { return guest::LoadU32(base, address + 10168); }
  void set_kind_list_move(uint32_t v) const { guest::StoreU32(base, address + 10168, v); }
  uint32_t kind_list_move_address() const { return address + 10168; }
  // the list's +32 (wrapped)
  uint32_t kind_list_wrapped() const { return guest::LoadU32(base, address + 10176); }
  void set_kind_list_wrapped(uint32_t v) const { guest::StoreU32(base, address + 10176, v); }
  uint32_t kind_list_wrapped_address() const { return address + 10176; }
  // the list's +60 (player)
  uint32_t kind_list_player() const { return guest::LoadU32(base, address + 10204); }
  void set_kind_list_player(uint32_t v) const { guest::StoreU32(base, address + 10204, v); }
  uint32_t kind_list_player_address() const { return address + 10204; }
  // the list's +68 (enabled)
  uint32_t kind_list_enabled() const { return guest::LoadU32(base, address + 10212); }
  void set_kind_list_enabled(uint32_t v) const { guest::StoreU32(base, address + 10212, v); }
  uint32_t kind_list_enabled_address() const { return address + 10212; }
  // the course list, 8 items (the route by its scroll and selection) (a MenuList, race-screens.toml)
  uint32_t course_list_address() const { return address + 10216; }
  // the list's +4 (scroll)
  uint32_t course_list_scroll() const { return guest::LoadU32(base, address + 10220); }
  void set_course_list_scroll(uint32_t v) const { guest::StoreU32(base, address + 10220, v); }
  uint32_t course_list_scroll_address() const { return address + 10220; }
  // the list's +8 (scroll_before)
  uint32_t course_list_scroll_before() const { return guest::LoadU32(base, address + 10224); }
  void set_course_list_scroll_before(uint32_t v) const { guest::StoreU32(base, address + 10224, v); }
  uint32_t course_list_scroll_before_address() const { return address + 10224; }
  // the list's +24 (move)
  uint32_t course_list_move() const { return guest::LoadU32(base, address + 10240); }
  void set_course_list_move(uint32_t v) const { guest::StoreU32(base, address + 10240, v); }
  uint32_t course_list_move_address() const { return address + 10240; }
  // the list's +32 (wrapped)
  uint32_t course_list_wrapped() const { return guest::LoadU32(base, address + 10248); }
  void set_course_list_wrapped(uint32_t v) const { guest::StoreU32(base, address + 10248, v); }
  uint32_t course_list_wrapped_address() const { return address + 10248; }
  // the list's +60 (player)
  uint32_t course_list_player() const { return guest::LoadU32(base, address + 10276); }
  void set_course_list_player(uint32_t v) const { guest::StoreU32(base, address + 10276, v); }
  uint32_t course_list_player_address() const { return address + 10276; }
  // the list's +68 (enabled)
  uint32_t course_list_enabled() const { return guest::LoadU32(base, address + 10284); }
  void set_course_list_enabled(uint32_t v) const { guest::StoreU32(base, address + 10284, v); }
  uint32_t course_list_enabled_address() const { return address + 10284; }
  // a two-item toggle (its selection 0: the race data's +0x59618 set) (a MenuList, race-screens.toml)
  uint32_t toggle_address() const { return address + 10288; }
  // the list's +4 (scroll)
  uint32_t toggle_scroll() const { return guest::LoadU32(base, address + 10292); }
  void set_toggle_scroll(uint32_t v) const { guest::StoreU32(base, address + 10292, v); }
  uint32_t toggle_scroll_address() const { return address + 10292; }
  // the list's +8 (scroll_before)
  uint32_t toggle_scroll_before() const { return guest::LoadU32(base, address + 10296); }
  void set_toggle_scroll_before(uint32_t v) const { guest::StoreU32(base, address + 10296, v); }
  uint32_t toggle_scroll_before_address() const { return address + 10296; }
  // the list's +24 (move)
  uint32_t toggle_move() const { return guest::LoadU32(base, address + 10312); }
  void set_toggle_move(uint32_t v) const { guest::StoreU32(base, address + 10312, v); }
  uint32_t toggle_move_address() const { return address + 10312; }
  // the list's +32 (wrapped)
  uint32_t toggle_wrapped() const { return guest::LoadU32(base, address + 10320); }
  void set_toggle_wrapped(uint32_t v) const { guest::StoreU32(base, address + 10320, v); }
  uint32_t toggle_wrapped_address() const { return address + 10320; }
  // the list's +60 (player)
  uint32_t toggle_player() const { return guest::LoadU32(base, address + 10348); }
  void set_toggle_player(uint32_t v) const { guest::StoreU32(base, address + 10348, v); }
  uint32_t toggle_player_address() const { return address + 10348; }
  // the list's +68 (enabled)
  uint32_t toggle_enabled() const { return guest::LoadU32(base, address + 10356); }
  void set_toggle_enabled(uint32_t v) const { guest::StoreU32(base, address + 10356, v); }
  uint32_t toggle_enabled_address() const { return address + 10356; }
  // the PairedNodeTable node_table points to
  uint32_t paired_table_address() const { return address + 10360; }
  // the UI movie's id
  uint32_t table_movie() const { return guest::LoadU32(base, address + 10364); }
  void set_table_movie(uint32_t v) const { guest::StoreU32(base, address + 10364, v); }
  uint32_t table_movie_address() const { return address + 10364; }
  // the sub-scene of the extra nodes (+420, +424, +264)
  uint32_t table_node_set_a() const { return guest::LoadU32(base, address + 10368); }
  void set_table_node_set_a(uint32_t v) const { guest::StoreU32(base, address + 10368, v); }
  uint32_t table_node_set_a_address() const { return address + 10368; }
  // the sub-scene of the items' nodes
  uint32_t table_node_set_b() const { return guest::LoadU32(base, address + 10372); }
  void set_table_node_set_b(uint32_t v) const { guest::StoreU32(base, address + 10372, v); }
  uint32_t table_node_set_b_address() const { return address + 10372; }
  // 27 items' named animations
  uint32_t table_item_anims_address() const { return address + 10376; }
  // eight more (the third pair's, +444)
  uint32_t table_more_anims_address() const { return address + 10484; }
  // 27 items' nodes
  uint32_t table_item_nodes_address() const { return address + 10516; }
  // eight nodes (hidden at the reset)
  uint32_t table_nodes_address() const { return address + 10624; }
  // the four children's indexes (marks and colors)
  uint32_t table_children_address() const { return address + 10656; }
  // 27 items' masks of the four children
  uint32_t table_item_masks_address() const { return address + 10672; }
  // the item nodes' twins on node_set_a (-1 none)
  uint32_t table_extra_node_base() const { return guest::LoadU32(base, address + 10780); }
  void set_table_extra_node_base(uint32_t v) const { guest::StoreU32(base, address + 10780, v); }
  uint32_t table_extra_node_base_address() const { return address + 10780; }
  // four text nodes (strings 402, 401, 407, 408; -1 none)
  uint32_t table_text_node_base() const { return guest::LoadU32(base, address + 10784); }
  void set_table_text_node_base(uint32_t v) const { guest::StoreU32(base, address + 10784, v); }
  uint32_t table_text_node_base_address() const { return address + 10784; }
  // the first pair: the item now
  uint32_t table_item() const { return guest::LoadU32(base, address + 10788); }
  void set_table_item(uint32_t v) const { guest::StoreU32(base, address + 10788, v); }
  uint32_t table_item_address() const { return address + 10788; }
  // the item before
  uint32_t table_item_before() const { return guest::LoadU32(base, address + 10792); }
  void set_table_item_before(uint32_t v) const { guest::StoreU32(base, address + 10792, v); }
  uint32_t table_item_before_address() const { return address + 10792; }
  // the second pair: an item now
  uint32_t table_second() const { return guest::LoadU32(base, address + 10796); }
  void set_table_second(uint32_t v) const { guest::StoreU32(base, address + 10796, v); }
  uint32_t table_second_address() const { return address + 10796; }
  uint32_t table_second_before() const { return guest::LoadU32(base, address + 10800); }
  void set_table_second_before(uint32_t v) const { guest::StoreU32(base, address + 10800, v); }
  uint32_t table_second_before_address() const { return address + 10800; }
  // the third pair: an index into more_anims now
  uint32_t table_third() const { return guest::LoadU32(base, address + 10804); }
  void set_table_third(uint32_t v) const { guest::StoreU32(base, address + 10804, v); }
  uint32_t table_third_address() const { return address + 10804; }
  uint32_t table_third_before() const { return guest::LoadU32(base, address + 10808); }
  void set_table_third_before(uint32_t v) const { guest::StoreU32(base, address + 10808, v); }
  uint32_t table_third_before_address() const { return address + 10808; }
  // the fourth pair: the paired table's mask bit; the color table's grid column
  uint32_t table_fourth() const { return guest::LoadU32(base, address + 10812); }
  void set_table_fourth(uint32_t v) const { guest::StoreU32(base, address + 10812, v); }
  uint32_t table_fourth_address() const { return address + 10812; }
  uint32_t table_fourth_before() const { return guest::LoadU32(base, address + 10816); }
  void set_table_fourth_before(uint32_t v) const { guest::StoreU32(base, address + 10816, v); }
  uint32_t table_fourth_before_address() const { return address + 10816; }
  // the scroll text the opening sets (923)
  uint32_t scroll_text_id() const { return guest::LoadU32(base, address + 10820); }
  void set_scroll_text_id(uint32_t v) const { guest::StoreU32(base, address + 10820, v); }
  uint32_t scroll_text_id_address() const { return address + 10820; }
  // the ListPart list_part points to
  uint32_t part_address() const { return address + 10824; }
};

// embedded; measure: ColorListScreen
struct ColorListScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 11040;
  static constexpr uint32_t kVtable = 0x820114E0;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // a ScreenMachine (boot-mode.md)
  uint32_t machine_address() const { return address + 4; }
  // the machine's state (its +9604)
  uint32_t state() const { return guest::LoadU32(base, address + 9608); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t state_address() const { return address + 9608; }
  uint32_t movie_loaded() const { return guest::LoadU32(base, address + 9612); }
  void set_movie_loaded(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t movie_loaded_address() const { return address + 9612; }
  // open the movie at the mode's start (else wait for the open message)
  uint32_t open_at_start() const { return guest::LoadU32(base, address + 9616); }
  void set_open_at_start(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t open_at_start_address() const { return address + 9616; }
  // the page Draw runs (-1 while a step runs)
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
  // the step Update runs (-1 none)
  uint32_t step() const { return guest::LoadU32(base, address + 9656); }
  void set_step(uint32_t v) const { guest::StoreU32(base, address + 9656, v); }
  uint32_t step_address() const { return address + 9656; }
  // the step after it (-1 none)
  uint32_t queued_step() const { return guest::LoadU32(base, address + 9660); }
  void set_queued_step(uint32_t v) const { guest::StoreU32(base, address + 9660, v); }
  uint32_t queued_step_address() const { return address + 9660; }
  // the page block's +20: 0 when a queued step starts
  uint32_t unknown_9664() const { return guest::LoadU32(base, address + 9664); }
  void set_unknown_9664(uint32_t v) const { guest::StoreU32(base, address + 9664, v); }
  uint32_t unknown_9664_address() const { return address + 9664; }
  // set by a step once its first call ran; 0 when the block moves on
  uint32_t step_started() const { return guest::LoadU32(base, address + 9668); }
  void set_step_started(uint32_t v) const { guest::StoreU32(base, address + 9668, v); }
  uint32_t step_started_address() const { return address + 9668; }
  // the node table (0 none): the type 6 list's at +10360, the type 7 list's at +10432
  template <typename T = NodeTable>
  T node_table() const { return T(base, guest::LoadU32(base, address + 9672)); }
  void set_node_table(const View& v) const { guest::StoreU32(base, address + 9672, v.address); }
  void set_node_table(uint32_t v) const { guest::StoreU32(base, address + 9672, v); }
  uint32_t node_table_address() const { return address + 9672; }
  // the list part (0 none): +10824 type 6, +11040 type 7
  template <typename T = ListPart>
  T list_part() const { return T(base, guest::LoadU32(base, address + 9676)); }
  void set_list_part(const View& v) const { guest::StoreU32(base, address + 9676, v.address); }
  void set_list_part(uint32_t v) const { guest::StoreU32(base, address + 9676, v); }
  uint32_t list_part_address() const { return address + 9676; }
  // the UI movie's id (1 at the constructor; 25 the node list)
  uint32_t movie() const { return guest::LoadU32(base, address + 9680); }
  void set_movie(uint32_t v) const { guest::StoreU32(base, address + 9680, v); }
  uint32_t movie_address() const { return address + 9680; }
  // 1 at the constructor, 0 type 7
  uint32_t unknown_9684() const { return guest::LoadU32(base, address + 9684); }
  void set_unknown_9684(uint32_t v) const { guest::StoreU32(base, address + 9684, v); }
  uint32_t unknown_9684_address() const { return address + 9684; }
  // 1 when the race was set (the part's yes): the screen ends in state 11
  uint32_t race_set() const { return guest::LoadU32(base, address + 9688); }
  void set_race_set(uint32_t v) const { guest::StoreU32(base, address + 9688, v); }
  uint32_t race_set_address() const { return address + 9688; }
  // 1 once the movie loaded
  uint32_t movie_ready() const { return guest::LoadU32(base, address + 9692); }
  void set_movie_ready(uint32_t v) const { guest::StoreU32(base, address + 9692, v); }
  uint32_t movie_ready_address() const { return address + 9692; }
  // the list part's input on
  uint32_t part_input() const { return guest::LoadU32(base, address + 9696); }
  void set_part_input(uint32_t v) const { guest::StoreU32(base, address + 9696, v); }
  uint32_t part_input_address() const { return address + 9696; }
  // bit 0 of the item info's +12
  uint32_t item_bit_0() const { return guest::LoadU32(base, address + 9700); }
  void set_item_bit_0(uint32_t v) const { guest::StoreU32(base, address + 9700, v); }
  uint32_t item_bit_0_address() const { return address + 9700; }
  // the same test, ANDed with the info's +12
  uint32_t item_bit_1() const { return guest::LoadU32(base, address + 9704); }
  void set_item_bit_1(uint32_t v) const { guest::StoreU32(base, address + 9704, v); }
  uint32_t item_bit_1_address() const { return address + 9704; }
  // type 7: show the part's nodes with it, and skip the leaving step after its yes
  uint32_t part_nodes() const { return guest::LoadU32(base, address + 9708); }
  void set_part_nodes(uint32_t v) const { guest::StoreU32(base, address + 9708, v); }
  uint32_t part_nodes_address() const { return address + 9708; }
  // the title text's child (string 322) on text_sub_scene
  uint32_t title_child() const { return guest::LoadU32(base, address + 9712); }
  void set_title_child(uint32_t v) const { guest::StoreU32(base, address + 9712, v); }
  uint32_t title_child_address() const { return address + 9712; }
  // a text's child (text 0; 0 none)
  uint32_t text_child_a() const { return guest::LoadU32(base, address + 9716); }
  void set_text_child_a(uint32_t v) const { guest::StoreU32(base, address + 9716, v); }
  uint32_t text_child_a_address() const { return address + 9716; }
  // a text's child (text 1; 0 none)
  uint32_t text_child_b() const { return guest::LoadU32(base, address + 9720); }
  void set_text_child_b(uint32_t v) const { guest::StoreU32(base, address + 9720, v); }
  uint32_t text_child_b_address() const { return address + 9720; }
  // the selection nodes' sub-scene (-1 none)
  uint32_t selection_sub_scene() const { return guest::LoadU32(base, address + 9724); }
  void set_selection_sub_scene(uint32_t v) const { guest::StoreU32(base, address + 9724, v); }
  uint32_t selection_sub_scene_address() const { return address + 9724; }
  // the titles' and arrows' sub-scene
  uint32_t text_sub_scene() const { return guest::LoadU32(base, address + 9728); }
  void set_text_sub_scene(uint32_t v) const { guest::StoreU32(base, address + 9728, v); }
  uint32_t text_sub_scene_address() const { return address + 9728; }
  uint32_t icon_sub_scene() const { return guest::LoadU32(base, address + 9732); }
  void set_icon_sub_scene(uint32_t v) const { guest::StoreU32(base, address + 9732, v); }
  uint32_t icon_sub_scene_address() const { return address + 9732; }
  // the color cursor's node; the sub-scene of color_cursor_child
  uint32_t color_cursor_node() const { return guest::LoadU32(base, address + 9736); }
  void set_color_cursor_node(uint32_t v) const { guest::StoreU32(base, address + 9736, v); }
  uint32_t color_cursor_node_address() const { return address + 9736; }
  // the scroll text's sub-scene
  uint32_t scroll_sub_scene() const { return guest::LoadU32(base, address + 9740); }
  void set_scroll_sub_scene(uint32_t v) const { guest::StoreU32(base, address + 9740, v); }
  uint32_t scroll_sub_scene_address() const { return address + 9740; }
  // the item cursor's node
  uint32_t item_cursor_node() const { return guest::LoadU32(base, address + 9744); }
  void set_item_cursor_node(uint32_t v) const { guest::StoreU32(base, address + 9744, v); }
  uint32_t item_cursor_node_address() const { return address + 9744; }
  // a child on selection_sub_scene shown by shown_child_on
  uint32_t shown_child() const { return guest::LoadU32(base, address + 9748); }
  void set_shown_child(uint32_t v) const { guest::StoreU32(base, address + 9748, v); }
  uint32_t shown_child_address() const { return address + 9748; }
  // the arrow SetArrows(0) sets
  uint32_t arrow_child_a() const { return guest::LoadU32(base, address + 9752); }
  void set_arrow_child_a(uint32_t v) const { guest::StoreU32(base, address + 9752, v); }
  uint32_t arrow_child_a_address() const { return address + 9752; }
  // the arrow SetArrows(1) sets
  uint32_t arrow_child_b() const { return guest::LoadU32(base, address + 9756); }
  void set_arrow_child_b(uint32_t v) const { guest::StoreU32(base, address + 9756, v); }
  uint32_t arrow_child_b_address() const { return address + 9756; }
  // a node (no sub-scene) given the selection
  uint32_t selection_node() const { return guest::LoadU32(base, address + 9760); }
  void set_selection_node(uint32_t v) const { guest::StoreU32(base, address + 9760, v); }
  uint32_t selection_node_address() const { return address + 9760; }
  // a child given the previous selection
  uint32_t previous_child() const { return guest::LoadU32(base, address + 9764); }
  void set_previous_child(uint32_t v) const { guest::StoreU32(base, address + 9764, v); }
  uint32_t previous_child_address() const { return address + 9764; }
  // a child given the selection
  uint32_t selection_child() const { return guest::LoadU32(base, address + 9768); }
  void set_selection_child(uint32_t v) const { guest::StoreU32(base, address + 9768, v); }
  uint32_t selection_child_address() const { return address + 9768; }
  // an array of children_count children given the previous selection (0 none)
  uint32_t previous_children() const { return guest::LoadU32(base, address + 9772); }
  void set_previous_children(uint32_t v) const { guest::StoreU32(base, address + 9772, v); }
  uint32_t previous_children_address() const { return address + 9772; }
  // an array given the selection (0 none; type 7 0x826371A4)
  uint32_t selection_children() const { return guest::LoadU32(base, address + 9776); }
  void set_selection_children(uint32_t v) const { guest::StoreU32(base, address + 9776, v); }
  uint32_t selection_children_address() const { return address + 9776; }
  // not written by the constructor
  uint32_t unknown_9780() const { return guest::LoadU32(base, address + 9780); }
  void set_unknown_9780(uint32_t v) const { guest::StoreU32(base, address + 9780, v); }
  uint32_t unknown_9780_address() const { return address + 9780; }
  uint32_t children_count() const { return guest::LoadU32(base, address + 9784); }
  void set_children_count(uint32_t v) const { guest::StoreU32(base, address + 9784, v); }
  uint32_t children_count_address() const { return address + 9784; }
  uint32_t shown_child_on() const { return guest::LoadU32(base, address + 9788); }
  void set_shown_child_on(uint32_t v) const { guest::StoreU32(base, address + 9788, v); }
  uint32_t shown_child_on_address() const { return address + 9788; }
  uint32_t unknown_9792() const { return guest::LoadU32(base, address + 9792); }
  void set_unknown_9792(uint32_t v) const { guest::StoreU32(base, address + 9792, v); }
  uint32_t unknown_9792_address() const { return address + 9792; }
  uint32_t unknown_9796() const { return guest::LoadU32(base, address + 9796); }
  void set_unknown_9796(uint32_t v) const { guest::StoreU32(base, address + 9796, v); }
  uint32_t unknown_9796_address() const { return address + 9796; }
  // the arrows' +108 when on
  uint32_t arrow_on_frame() const { return guest::LoadU32(base, address + 9800); }
  void set_arrow_on_frame(uint32_t v) const { guest::StoreU32(base, address + 9800, v); }
  uint32_t arrow_on_frame_address() const { return address + 9800; }
  // the arrows' +108 when off
  uint32_t arrow_off_frame() const { return guest::LoadU32(base, address + 9804); }
  void set_arrow_off_frame(uint32_t v) const { guest::StoreU32(base, address + 9804, v); }
  uint32_t arrow_off_frame_address() const { return address + 9804; }
  uint32_t color_cursor_child() const { return guest::LoadU32(base, address + 9808); }
  void set_color_cursor_child(uint32_t v) const { guest::StoreU32(base, address + 9808, v); }
  uint32_t color_cursor_child_address() const { return address + 9808; }
  // the first of six icon children (four textures, two hidden)
  uint32_t icon_child() const { return guest::LoadU32(base, address + 9812); }
  void set_icon_child(uint32_t v) const { guest::StoreU32(base, address + 9812, v); }
  uint32_t icon_child_address() const { return address + 9812; }
  // the scroll text's child
  uint32_t scroll_child() const { return guest::LoadU32(base, address + 9816); }
  void set_scroll_child(uint32_t v) const { guest::StoreU32(base, address + 9816, v); }
  uint32_t scroll_child_address() const { return address + 9816; }
  uint32_t unknown_9820() const { return guest::LoadU32(base, address + 9820); }
  void set_unknown_9820(uint32_t v) const { guest::StoreU32(base, address + 9820, v); }
  uint32_t unknown_9820_address() const { return address + 9820; }
  // type 7: a child on text_sub_scene shown with the part
  uint32_t part_child() const { return guest::LoadU32(base, address + 9824); }
  void set_part_child(uint32_t v) const { guest::StoreU32(base, address + 9824, v); }
  uint32_t part_child_address() const { return address + 9824; }
  uint32_t item_cursor_child() const { return guest::LoadU32(base, address + 9828); }
  void set_item_cursor_child(uint32_t v) const { guest::StoreU32(base, address + 9828, v); }
  uint32_t item_cursor_child_address() const { return address + 9828; }
  // the opening: played once
  uint32_t open_anim() const { return guest::LoadU32(base, address + 9832); }
  void set_open_anim(uint32_t v) const { guest::StoreU32(base, address + 9832, v); }
  uint32_t open_anim_address() const { return address + 9832; }
  // the leaving: played once
  uint32_t leave_anim() const { return guest::LoadU32(base, address + 9836); }
  void set_leave_anim(uint32_t v) const { guest::StoreU32(base, address + 9836, v); }
  uint32_t leave_anim_address() const { return address + 9836; }
  // type 7's step 2 (page 2 to 3)
  uint32_t first_to_items_anim() const { return guest::LoadU32(base, address + 9840); }
  void set_first_to_items_anim(uint32_t v) const { guest::StoreU32(base, address + 9840, v); }
  uint32_t first_to_items_anim_address() const { return address + 9840; }
  // type 7's step 3 (page 3 back to 2)
  uint32_t items_to_first_anim() const { return guest::LoadU32(base, address + 9844); }
  void set_items_to_first_anim(uint32_t v) const { guest::StoreU32(base, address + 9844, v); }
  uint32_t items_to_first_anim_address() const { return address + 9844; }
  // type 7's step 4 (page 3 to 4)
  uint32_t items_to_colors_anim() const { return guest::LoadU32(base, address + 9848); }
  void set_items_to_colors_anim(uint32_t v) const { guest::StoreU32(base, address + 9848, v); }
  uint32_t items_to_colors_anim_address() const { return address + 9848; }
  // type 7's step 6 (page 4 back to 3)
  uint32_t colors_to_items_anim() const { return guest::LoadU32(base, address + 9852); }
  void set_colors_to_items_anim(uint32_t v) const { guest::StoreU32(base, address + 9852, v); }
  uint32_t colors_to_items_anim_address() const { return address + 9852; }
  // the opening: played and looped
  uint32_t open_loop_a() const { return guest::LoadU32(base, address + 9856); }
  void set_open_loop_a(uint32_t v) const { guest::StoreU32(base, address + 9856, v); }
  uint32_t open_loop_a_address() const { return address + 9856; }
  // the opening: played and looped
  uint32_t open_loop_b() const { return guest::LoadU32(base, address + 9860); }
  void set_open_loop_b(uint32_t v) const { guest::StoreU32(base, address + 9860, v); }
  uint32_t open_loop_b_address() const { return address + 9860; }
  // the opening: chained
  uint32_t open_anim_b() const { return guest::LoadU32(base, address + 9864); }
  void set_open_anim_b(uint32_t v) const { guest::StoreU32(base, address + 9864, v); }
  uint32_t open_anim_b_address() const { return address + 9864; }
  // after the opening: looped
  uint32_t shown_loop_a() const { return guest::LoadU32(base, address + 9868); }
  void set_shown_loop_a(uint32_t v) const { guest::StoreU32(base, address + 9868, v); }
  uint32_t shown_loop_a_address() const { return address + 9868; }
  // the item list's loop (finished when it leaves)
  uint32_t items_loop() const { return guest::LoadU32(base, address + 9872); }
  void set_items_loop(uint32_t v) const { guest::StoreU32(base, address + 9872, v); }
  uint32_t items_loop_address() const { return address + 9872; }
  // after the opening: looped
  uint32_t shown_loop_b() const { return guest::LoadU32(base, address + 9876); }
  void set_shown_loop_b(uint32_t v) const { guest::StoreU32(base, address + 9876, v); }
  uint32_t shown_loop_b_address() const { return address + 9876; }
  // after the opening: looped
  uint32_t shown_loop_c() const { return guest::LoadU32(base, address + 9880); }
  void set_shown_loop_c(uint32_t v) const { guest::StoreU32(base, address + 9880, v); }
  uint32_t shown_loop_c_address() const { return address + 9880; }
  // after the opening: looped
  uint32_t shown_loop_d() const { return guest::LoadU32(base, address + 9884); }
  void set_shown_loop_d(uint32_t v) const { guest::StoreU32(base, address + 9884, v); }
  uint32_t shown_loop_d_address() const { return address + 9884; }
  // the opening: chained
  uint32_t open_anim_c() const { return guest::LoadU32(base, address + 9888); }
  void set_open_anim_c(uint32_t v) const { guest::StoreU32(base, address + 9888, v); }
  uint32_t open_anim_c_address() const { return address + 9888; }
  // the toggle's animation for its selection 0
  uint32_t toggle_anim_a() const { return guest::LoadU32(base, address + 9892); }
  void set_toggle_anim_a(uint32_t v) const { guest::StoreU32(base, address + 9892, v); }
  uint32_t toggle_anim_a_address() const { return address + 9892; }
  // the toggle's animation for its selection 1
  uint32_t toggle_anim_b() const { return guest::LoadU32(base, address + 9896); }
  void set_toggle_anim_b(uint32_t v) const { guest::StoreU32(base, address + 9896, v); }
  uint32_t toggle_anim_b_address() const { return address + 9896; }
  // replayed by ShowSelection
  uint32_t selection_anim() const { return guest::LoadU32(base, address + 9900); }
  void set_selection_anim(uint32_t v) const { guest::StoreU32(base, address + 9900, v); }
  uint32_t selection_anim_address() const { return address + 9900; }
  // an item list's move up
  uint32_t item_up_anim() const { return guest::LoadU32(base, address + 9904); }
  void set_item_up_anim(uint32_t v) const { guest::StoreU32(base, address + 9904, v); }
  uint32_t item_up_anim_address() const { return address + 9904; }
  // an item list's move down
  uint32_t item_down_anim() const { return guest::LoadU32(base, address + 9908); }
  void set_item_down_anim(uint32_t v) const { guest::StoreU32(base, address + 9908, v); }
  uint32_t item_down_anim_address() const { return address + 9908; }
  // type 7's first list moved back
  uint32_t first_back_anim() const { return guest::LoadU32(base, address + 9912); }
  void set_first_back_anim(uint32_t v) const { guest::StoreU32(base, address + 9912, v); }
  uint32_t first_back_anim_address() const { return address + 9912; }
  // type 7's first list moved forward
  uint32_t first_forward_anim() const { return guest::LoadU32(base, address + 9916); }
  void set_first_forward_anim(uint32_t v) const { guest::StoreU32(base, address + 9916, v); }
  uint32_t first_forward_anim_address() const { return address + 9916; }
  // type 7's color list moved back
  uint32_t color_back_anim() const { return guest::LoadU32(base, address + 9920); }
  void set_color_back_anim(uint32_t v) const { guest::StoreU32(base, address + 9920, v); }
  uint32_t color_back_anim_address() const { return address + 9920; }
  // type 7's color list moved forward
  uint32_t color_forward_anim() const { return guest::LoadU32(base, address + 9924); }
  void set_color_forward_anim(uint32_t v) const { guest::StoreU32(base, address + 9924, v); }
  uint32_t color_forward_anim_address() const { return address + 9924; }
  // a list scrolled down
  uint32_t scroll_down_anim() const { return guest::LoadU32(base, address + 9928); }
  void set_scroll_down_anim(uint32_t v) const { guest::StoreU32(base, address + 9928, v); }
  uint32_t scroll_down_anim_address() const { return address + 9928; }
  // a list scrolled up
  uint32_t scroll_up_anim() const { return guest::LoadU32(base, address + 9932); }
  void set_scroll_up_anim(uint32_t v) const { guest::StoreU32(base, address + 9932, v); }
  uint32_t scroll_up_anim_address() const { return address + 9932; }
  // ten named animations by the item list's selection (LoopItemNode)
  uint32_t item_anims_address() const { return address + 9936; }
  // item_anims[0]
  uint32_t item_anim_0() const { return guest::LoadU32(base, address + 9936); }
  void set_item_anim_0(uint32_t v) const { guest::StoreU32(base, address + 9936, v); }
  uint32_t item_anim_0_address() const { return address + 9936; }
  // ten by the first list's selection (LoopSecondNode)
  uint32_t first_anims_address() const { return address + 9976; }
  // first_anims[0]
  uint32_t first_anim_0() const { return guest::LoadU32(base, address + 9976); }
  void set_first_anim_0(uint32_t v) const { guest::StoreU32(base, address + 9976, v); }
  uint32_t first_anim_0_address() const { return address + 9976; }
  // ten by the color list's selection (LoopThirdNode)
  uint32_t color_anims_address() const { return address + 10016; }
  // color_anims[0]
  uint32_t color_anim_0() const { return guest::LoadU32(base, address + 10016); }
  void set_color_anim_0(uint32_t v) const { guest::StoreU32(base, address + 10016, v); }
  uint32_t color_anim_0_address() const { return address + 10016; }
  // ten by the item's selection (type 7's step 4 plays, step 6 finishes)
  uint32_t item_anims_b_address() const { return address + 10056; }
  // item_anims_b[0]
  uint32_t item_anim_b_0() const { return guest::LoadU32(base, address + 10056); }
  void set_item_anim_b_0(uint32_t v) const { guest::StoreU32(base, address + 10056, v); }
  uint32_t item_anim_b_0_address() const { return address + 10056; }
  // item_anims_b[1]
  uint32_t item_anim_b_1() const { return guest::LoadU32(base, address + 10060); }
  void set_item_anim_b_1(uint32_t v) const { guest::StoreU32(base, address + 10060, v); }
  uint32_t item_anim_b_1_address() const { return address + 10060; }
  // item_anims_b[2]
  uint32_t item_anim_b_2() const { return guest::LoadU32(base, address + 10064); }
  void set_item_anim_b_2(uint32_t v) const { guest::StoreU32(base, address + 10064, v); }
  uint32_t item_anim_b_2_address() const { return address + 10064; }
  // ten by the item's selection (type 7's step 4 finishes, step 6 plays)
  uint32_t item_anims_c_address() const { return address + 10096; }
  // item_anims_c[0]
  uint32_t item_anim_c_0() const { return guest::LoadU32(base, address + 10096); }
  void set_item_anim_c_0(uint32_t v) const { guest::StoreU32(base, address + 10096, v); }
  uint32_t item_anim_c_0_address() const { return address + 10096; }
  // item_anims_c[1]
  uint32_t item_anim_c_1() const { return guest::LoadU32(base, address + 10100); }
  void set_item_anim_c_1(uint32_t v) const { guest::StoreU32(base, address + 10100, v); }
  uint32_t item_anim_c_1_address() const { return address + 10100; }
  // item_anims_c[2]
  uint32_t item_anim_c_2() const { return guest::LoadU32(base, address + 10104); }
  void set_item_anim_c_2(uint32_t v) const { guest::StoreU32(base, address + 10104, v); }
  uint32_t item_anim_c_2_address() const { return address + 10104; }
  // cleared by ClearScrollText
  uint32_t scroll_text_shown() const { return guest::LoadU32(base, address + 10136); }
  void set_scroll_text_shown(uint32_t v) const { guest::StoreU32(base, address + 10136, v); }
  uint32_t scroll_text_shown_address() const { return address + 10136; }
  // the scroll text shown (-1 none)
  uint32_t scroll_text() const { return guest::LoadU32(base, address + 10140); }
  void set_scroll_text(uint32_t v) const { guest::StoreU32(base, address + 10140, v); }
  uint32_t scroll_text_address() const { return address + 10140; }
  // the item list: the course, 15 items (the route by its scroll and selection) (a MenuList, race-screens.toml)
  uint32_t course_list_address() const { return address + 10144; }
  // the list's +4 (scroll)
  uint32_t course_list_scroll() const { return guest::LoadU32(base, address + 10148); }
  void set_course_list_scroll(uint32_t v) const { guest::StoreU32(base, address + 10148, v); }
  uint32_t course_list_scroll_address() const { return address + 10148; }
  // the list's +8 (scroll_before)
  uint32_t course_list_scroll_before() const { return guest::LoadU32(base, address + 10152); }
  void set_course_list_scroll_before(uint32_t v) const { guest::StoreU32(base, address + 10152, v); }
  uint32_t course_list_scroll_before_address() const { return address + 10152; }
  // the list's +24 (move)
  uint32_t course_list_move() const { return guest::LoadU32(base, address + 10168); }
  void set_course_list_move(uint32_t v) const { guest::StoreU32(base, address + 10168, v); }
  uint32_t course_list_move_address() const { return address + 10168; }
  // the list's +32 (wrapped)
  uint32_t course_list_wrapped() const { return guest::LoadU32(base, address + 10176); }
  void set_course_list_wrapped(uint32_t v) const { guest::StoreU32(base, address + 10176, v); }
  uint32_t course_list_wrapped_address() const { return address + 10176; }
  // the list's +60 (player)
  uint32_t course_list_player() const { return guest::LoadU32(base, address + 10204); }
  void set_course_list_player(uint32_t v) const { guest::StoreU32(base, address + 10204, v); }
  uint32_t course_list_player_address() const { return address + 10204; }
  // the list's +68 (enabled)
  uint32_t course_list_enabled() const { return guest::LoadU32(base, address + 10212); }
  void set_course_list_enabled(uint32_t v) const { guest::StoreU32(base, address + 10212, v); }
  uint32_t course_list_enabled_address() const { return address + 10212; }
  // the color list (count from the item's info) (a MenuList, race-screens.toml)
  uint32_t color_list_address() const { return address + 10216; }
  // the list's +4 (scroll)
  uint32_t color_list_scroll() const { return guest::LoadU32(base, address + 10220); }
  void set_color_list_scroll(uint32_t v) const { guest::StoreU32(base, address + 10220, v); }
  uint32_t color_list_scroll_address() const { return address + 10220; }
  // the list's +8 (scroll_before)
  uint32_t color_list_scroll_before() const { return guest::LoadU32(base, address + 10224); }
  void set_color_list_scroll_before(uint32_t v) const { guest::StoreU32(base, address + 10224, v); }
  uint32_t color_list_scroll_before_address() const { return address + 10224; }
  // the list's +24 (move)
  uint32_t color_list_move() const { return guest::LoadU32(base, address + 10240); }
  void set_color_list_move(uint32_t v) const { guest::StoreU32(base, address + 10240, v); }
  uint32_t color_list_move_address() const { return address + 10240; }
  // the list's +32 (wrapped)
  uint32_t color_list_wrapped() const { return guest::LoadU32(base, address + 10248); }
  void set_color_list_wrapped(uint32_t v) const { guest::StoreU32(base, address + 10248, v); }
  uint32_t color_list_wrapped_address() const { return address + 10248; }
  // the list's +60 (player)
  uint32_t color_list_player() const { return guest::LoadU32(base, address + 10276); }
  void set_color_list_player(uint32_t v) const { guest::StoreU32(base, address + 10276, v); }
  uint32_t color_list_player_address() const { return address + 10276; }
  // the list's +68 (enabled)
  uint32_t color_list_enabled() const { return guest::LoadU32(base, address + 10284); }
  void set_color_list_enabled(uint32_t v) const { guest::StoreU32(base, address + 10284, v); }
  uint32_t color_list_enabled_address() const { return address + 10284; }
  // a two-item toggle (its selection 0: the race data's +0x59618 = 4) (a MenuList, race-screens.toml)
  uint32_t toggle_address() const { return address + 10288; }
  // the list's +4 (scroll)
  uint32_t toggle_scroll() const { return guest::LoadU32(base, address + 10292); }
  void set_toggle_scroll(uint32_t v) const { guest::StoreU32(base, address + 10292, v); }
  uint32_t toggle_scroll_address() const { return address + 10292; }
  // the list's +8 (scroll_before)
  uint32_t toggle_scroll_before() const { return guest::LoadU32(base, address + 10296); }
  void set_toggle_scroll_before(uint32_t v) const { guest::StoreU32(base, address + 10296, v); }
  uint32_t toggle_scroll_before_address() const { return address + 10296; }
  // the list's +24 (move)
  uint32_t toggle_move() const { return guest::LoadU32(base, address + 10312); }
  void set_toggle_move(uint32_t v) const { guest::StoreU32(base, address + 10312, v); }
  uint32_t toggle_move_address() const { return address + 10312; }
  // the list's +32 (wrapped)
  uint32_t toggle_wrapped() const { return guest::LoadU32(base, address + 10320); }
  void set_toggle_wrapped(uint32_t v) const { guest::StoreU32(base, address + 10320, v); }
  uint32_t toggle_wrapped_address() const { return address + 10320; }
  // the list's +60 (player)
  uint32_t toggle_player() const { return guest::LoadU32(base, address + 10348); }
  void set_toggle_player(uint32_t v) const { guest::StoreU32(base, address + 10348, v); }
  uint32_t toggle_player_address() const { return address + 10348; }
  // the list's +68 (enabled)
  uint32_t toggle_enabled() const { return guest::LoadU32(base, address + 10356); }
  void set_toggle_enabled(uint32_t v) const { guest::StoreU32(base, address + 10356, v); }
  uint32_t toggle_enabled_address() const { return address + 10356; }
  // the first list: the race kind (the race data's +32) (a MenuList, race-screens.toml)
  uint32_t kind_list_address() const { return address + 10360; }
  // the list's +4 (scroll)
  uint32_t kind_list_scroll() const { return guest::LoadU32(base, address + 10364); }
  void set_kind_list_scroll(uint32_t v) const { guest::StoreU32(base, address + 10364, v); }
  uint32_t kind_list_scroll_address() const { return address + 10364; }
  // the list's +8 (scroll_before)
  uint32_t kind_list_scroll_before() const { return guest::LoadU32(base, address + 10368); }
  void set_kind_list_scroll_before(uint32_t v) const { guest::StoreU32(base, address + 10368, v); }
  uint32_t kind_list_scroll_before_address() const { return address + 10368; }
  // the list's +24 (move)
  uint32_t kind_list_move() const { return guest::LoadU32(base, address + 10384); }
  void set_kind_list_move(uint32_t v) const { guest::StoreU32(base, address + 10384, v); }
  uint32_t kind_list_move_address() const { return address + 10384; }
  // the list's +32 (wrapped)
  uint32_t kind_list_wrapped() const { return guest::LoadU32(base, address + 10392); }
  void set_kind_list_wrapped(uint32_t v) const { guest::StoreU32(base, address + 10392, v); }
  uint32_t kind_list_wrapped_address() const { return address + 10392; }
  // the list's +60 (player)
  uint32_t kind_list_player() const { return guest::LoadU32(base, address + 10420); }
  void set_kind_list_player(uint32_t v) const { guest::StoreU32(base, address + 10420, v); }
  uint32_t kind_list_player_address() const { return address + 10420; }
  // the list's +68 (enabled)
  uint32_t kind_list_enabled() const { return guest::LoadU32(base, address + 10428); }
  void set_kind_list_enabled(uint32_t v) const { guest::StoreU32(base, address + 10428, v); }
  uint32_t kind_list_enabled_address() const { return address + 10428; }
  // the ColorNodeTable node_table points to
  uint32_t color_table_address() const { return address + 10432; }
  // the color table's movie
  uint32_t table_movie() const { return guest::LoadU32(base, address + 10436); }
  void set_table_movie(uint32_t v) const { guest::StoreU32(base, address + 10436, v); }
  uint32_t table_movie_address() const { return address + 10436; }
  uint32_t table_node_set_a() const { return guest::LoadU32(base, address + 10440); }
  void set_table_node_set_a(uint32_t v) const { guest::StoreU32(base, address + 10440, v); }
  uint32_t table_node_set_a_address() const { return address + 10440; }
  uint32_t table_node_set_b() const { return guest::LoadU32(base, address + 10444); }
  void set_table_node_set_b(uint32_t v) const { guest::StoreU32(base, address + 10444, v); }
  uint32_t table_node_set_b_address() const { return address + 10444; }
  // the color table's item_anims[0]
  uint32_t table_item_anim_0() const { return guest::LoadU32(base, address + 10448); }
  void set_table_item_anim_0(uint32_t v) const { guest::StoreU32(base, address + 10448, v); }
  uint32_t table_item_anim_0_address() const { return address + 10448; }
  // the color table's item_nodes[0]
  uint32_t table_item_node_0() const { return guest::LoadU32(base, address + 10588); }
  void set_table_item_node_0(uint32_t v) const { guest::StoreU32(base, address + 10588, v); }
  uint32_t table_item_node_0_address() const { return address + 10588; }
  // the color table's item_masks[0]
  uint32_t table_item_mask_0() const { return guest::LoadU32(base, address + 10744); }
  void set_table_item_mask_0(uint32_t v) const { guest::StoreU32(base, address + 10744, v); }
  uint32_t table_item_mask_0_address() const { return address + 10744; }
  uint32_t table_extra_node_base() const { return guest::LoadU32(base, address + 10852); }
  void set_table_extra_node_base(uint32_t v) const { guest::StoreU32(base, address + 10852, v); }
  uint32_t table_extra_node_base_address() const { return address + 10852; }
  uint32_t table_text_node_base() const { return guest::LoadU32(base, address + 10856); }
  void set_table_text_node_base(uint32_t v) const { guest::StoreU32(base, address + 10856, v); }
  uint32_t table_text_node_base_address() const { return address + 10856; }
  // the color table's color_child: LoadItem's color, -1 none
  uint32_t table_color_child() const { return guest::LoadU32(base, address + 10908); }
  void set_table_color_child(uint32_t v) const { guest::StoreU32(base, address + 10908, v); }
  uint32_t table_color_child_address() const { return address + 10908; }
  // the color table's grid_a and grid_b
  uint32_t table_grids_address() const { return address + 10912; }
  // the color table's +484
  uint32_t table_word_10916() const { return guest::LoadU32(base, address + 10916); }
  void set_table_word_10916(uint32_t v) const { guest::StoreU32(base, address + 10916, v); }
  uint32_t table_word_10916_address() const { return address + 10916; }
  // the color table's +488
  uint32_t table_word_10920() const { return guest::LoadU32(base, address + 10920); }
  void set_table_word_10920(uint32_t v) const { guest::StoreU32(base, address + 10920, v); }
  uint32_t table_word_10920_address() const { return address + 10920; }
  // the color table's +492
  uint32_t table_word_10924() const { return guest::LoadU32(base, address + 10924); }
  void set_table_word_10924(uint32_t v) const { guest::StoreU32(base, address + 10924, v); }
  uint32_t table_word_10924_address() const { return address + 10924; }
  // the color table's +500
  uint32_t table_word_10932() const { return guest::LoadU32(base, address + 10932); }
  void set_table_word_10932(uint32_t v) const { guest::StoreU32(base, address + 10932, v); }
  uint32_t table_word_10932_address() const { return address + 10932; }
  // the color table's +504
  uint32_t table_word_10936() const { return guest::LoadU32(base, address + 10936); }
  void set_table_word_10936(uint32_t v) const { guest::StoreU32(base, address + 10936, v); }
  uint32_t table_word_10936_address() const { return address + 10936; }
  // the color table's +508
  uint32_t table_word_10940() const { return guest::LoadU32(base, address + 10940); }
  void set_table_word_10940(uint32_t v) const { guest::StoreU32(base, address + 10940, v); }
  uint32_t table_word_10940_address() const { return address + 10940; }
  // the color table's +520
  uint32_t table_word_10952() const { return guest::LoadU32(base, address + 10952); }
  void set_table_word_10952(uint32_t v) const { guest::StoreU32(base, address + 10952, v); }
  uint32_t table_word_10952_address() const { return address + 10952; }
  // the color table's +524
  uint32_t table_word_10956() const { return guest::LoadU32(base, address + 10956); }
  void set_table_word_10956(uint32_t v) const { guest::StoreU32(base, address + 10956, v); }
  uint32_t table_word_10956_address() const { return address + 10956; }
  // the color table's +528
  uint32_t table_word_10960() const { return guest::LoadU32(base, address + 10960); }
  void set_table_word_10960(uint32_t v) const { guest::StoreU32(base, address + 10960, v); }
  uint32_t table_word_10960_address() const { return address + 10960; }
  // the color table's +536
  uint32_t table_word_10968() const { return guest::LoadU32(base, address + 10968); }
  void set_table_word_10968(uint32_t v) const { guest::StoreU32(base, address + 10968, v); }
  uint32_t table_word_10968_address() const { return address + 10968; }
  // the color table's +540
  uint32_t table_word_10972() const { return guest::LoadU32(base, address + 10972); }
  void set_table_word_10972(uint32_t v) const { guest::StoreU32(base, address + 10972, v); }
  uint32_t table_word_10972_address() const { return address + 10972; }
  // the color table's +544
  uint32_t table_word_10976() const { return guest::LoadU32(base, address + 10976); }
  void set_table_word_10976(uint32_t v) const { guest::StoreU32(base, address + 10976, v); }
  uint32_t table_word_10976_address() const { return address + 10976; }
  // the color table's color_a and color_b
  uint32_t table_colors_address() const { return address + 10984; }
  // the scroll texts' string ids (545 to 549, then -1)
  uint32_t text_items() const { return guest::LoadU32(base, address + 11016); }
  void set_text_items(uint32_t v) const { guest::StoreU32(base, address + 11016, v); }
  uint32_t text_items_address() const { return address + 11016; }
  uint32_t unknown_11020() const { return guest::LoadU32(base, address + 11020); }
  void set_unknown_11020(uint32_t v) const { guest::StoreU32(base, address + 11020, v); }
  uint32_t unknown_11020_address() const { return address + 11020; }
  uint32_t unknown_11024() const { return guest::LoadU32(base, address + 11024); }
  void set_unknown_11024(uint32_t v) const { guest::StoreU32(base, address + 11024, v); }
  uint32_t unknown_11024_address() const { return address + 11024; }
  // steps 2 and 6
  uint32_t text_item_moved() const { return guest::LoadU32(base, address + 11028); }
  void set_text_item_moved(uint32_t v) const { guest::StoreU32(base, address + 11028, v); }
  uint32_t text_item_moved_address() const { return address + 11028; }
  // step 4, and back from the list part
  uint32_t text_colors() const { return guest::LoadU32(base, address + 11032); }
  void set_text_colors(uint32_t v) const { guest::StoreU32(base, address + 11032, v); }
  uint32_t text_colors_address() const { return address + 11032; }
  // page 5 (the list part)
  uint32_t text_part() const { return guest::LoadU32(base, address + 11036); }
  void set_text_part(uint32_t v) const { guest::StoreU32(base, address + 11036, v); }
  uint32_t text_part_address() const { return address + 11036; }
};

// embedded; measure: Type7ListScreen
struct Type7ListScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 11180;
  static constexpr uint32_t kVtable = 0x82010F18;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // a ScreenMachine (boot-mode.md)
  uint32_t machine_address() const { return address + 4; }
  // the machine's state (its +9604)
  uint32_t state() const { return guest::LoadU32(base, address + 9608); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t state_address() const { return address + 9608; }
  uint32_t movie_loaded() const { return guest::LoadU32(base, address + 9612); }
  void set_movie_loaded(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t movie_loaded_address() const { return address + 9612; }
  // open the movie at the mode's start (else wait for the open message)
  uint32_t open_at_start() const { return guest::LoadU32(base, address + 9616); }
  void set_open_at_start(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t open_at_start_address() const { return address + 9616; }
  // the page Draw runs (-1 while a step runs)
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
  // the step Update runs (-1 none)
  uint32_t step() const { return guest::LoadU32(base, address + 9656); }
  void set_step(uint32_t v) const { guest::StoreU32(base, address + 9656, v); }
  uint32_t step_address() const { return address + 9656; }
  // the step after it (-1 none)
  uint32_t queued_step() const { return guest::LoadU32(base, address + 9660); }
  void set_queued_step(uint32_t v) const { guest::StoreU32(base, address + 9660, v); }
  uint32_t queued_step_address() const { return address + 9660; }
  // the page block's +20: 0 when a queued step starts
  uint32_t unknown_9664() const { return guest::LoadU32(base, address + 9664); }
  void set_unknown_9664(uint32_t v) const { guest::StoreU32(base, address + 9664, v); }
  uint32_t unknown_9664_address() const { return address + 9664; }
  // set by a step once its first call ran; 0 when the block moves on
  uint32_t step_started() const { return guest::LoadU32(base, address + 9668); }
  void set_step_started(uint32_t v) const { guest::StoreU32(base, address + 9668, v); }
  uint32_t step_started_address() const { return address + 9668; }
  // the node table (0 none): the type 6 list's at +10360, the type 7 list's at +10432
  template <typename T = NodeTable>
  T node_table() const { return T(base, guest::LoadU32(base, address + 9672)); }
  void set_node_table(const View& v) const { guest::StoreU32(base, address + 9672, v.address); }
  void set_node_table(uint32_t v) const { guest::StoreU32(base, address + 9672, v); }
  uint32_t node_table_address() const { return address + 9672; }
  // the list part (0 none): +10824 type 6, +11040 type 7
  template <typename T = ListPart>
  T list_part() const { return T(base, guest::LoadU32(base, address + 9676)); }
  void set_list_part(const View& v) const { guest::StoreU32(base, address + 9676, v.address); }
  void set_list_part(uint32_t v) const { guest::StoreU32(base, address + 9676, v); }
  uint32_t list_part_address() const { return address + 9676; }
  // the UI movie's id (1 at the constructor; 25 the node list)
  uint32_t movie() const { return guest::LoadU32(base, address + 9680); }
  void set_movie(uint32_t v) const { guest::StoreU32(base, address + 9680, v); }
  uint32_t movie_address() const { return address + 9680; }
  // 1 at the constructor, 0 type 7
  uint32_t unknown_9684() const { return guest::LoadU32(base, address + 9684); }
  void set_unknown_9684(uint32_t v) const { guest::StoreU32(base, address + 9684, v); }
  uint32_t unknown_9684_address() const { return address + 9684; }
  // 1 when the race was set (the part's yes): the screen ends in state 11
  uint32_t race_set() const { return guest::LoadU32(base, address + 9688); }
  void set_race_set(uint32_t v) const { guest::StoreU32(base, address + 9688, v); }
  uint32_t race_set_address() const { return address + 9688; }
  // 1 once the movie loaded
  uint32_t movie_ready() const { return guest::LoadU32(base, address + 9692); }
  void set_movie_ready(uint32_t v) const { guest::StoreU32(base, address + 9692, v); }
  uint32_t movie_ready_address() const { return address + 9692; }
  // the list part's input on
  uint32_t part_input() const { return guest::LoadU32(base, address + 9696); }
  void set_part_input(uint32_t v) const { guest::StoreU32(base, address + 9696, v); }
  uint32_t part_input_address() const { return address + 9696; }
  // bit 0 of the item info's +12
  uint32_t item_bit_0() const { return guest::LoadU32(base, address + 9700); }
  void set_item_bit_0(uint32_t v) const { guest::StoreU32(base, address + 9700, v); }
  uint32_t item_bit_0_address() const { return address + 9700; }
  // the same test, ANDed with the info's +12
  uint32_t item_bit_1() const { return guest::LoadU32(base, address + 9704); }
  void set_item_bit_1(uint32_t v) const { guest::StoreU32(base, address + 9704, v); }
  uint32_t item_bit_1_address() const { return address + 9704; }
  // type 7: show the part's nodes with it, and skip the leaving step after its yes
  uint32_t part_nodes() const { return guest::LoadU32(base, address + 9708); }
  void set_part_nodes(uint32_t v) const { guest::StoreU32(base, address + 9708, v); }
  uint32_t part_nodes_address() const { return address + 9708; }
  // the title text's child (string 322) on text_sub_scene
  uint32_t title_child() const { return guest::LoadU32(base, address + 9712); }
  void set_title_child(uint32_t v) const { guest::StoreU32(base, address + 9712, v); }
  uint32_t title_child_address() const { return address + 9712; }
  // a text's child (text 0; 0 none)
  uint32_t text_child_a() const { return guest::LoadU32(base, address + 9716); }
  void set_text_child_a(uint32_t v) const { guest::StoreU32(base, address + 9716, v); }
  uint32_t text_child_a_address() const { return address + 9716; }
  // a text's child (text 1; 0 none)
  uint32_t text_child_b() const { return guest::LoadU32(base, address + 9720); }
  void set_text_child_b(uint32_t v) const { guest::StoreU32(base, address + 9720, v); }
  uint32_t text_child_b_address() const { return address + 9720; }
  // the selection nodes' sub-scene (-1 none)
  uint32_t selection_sub_scene() const { return guest::LoadU32(base, address + 9724); }
  void set_selection_sub_scene(uint32_t v) const { guest::StoreU32(base, address + 9724, v); }
  uint32_t selection_sub_scene_address() const { return address + 9724; }
  // the titles' and arrows' sub-scene
  uint32_t text_sub_scene() const { return guest::LoadU32(base, address + 9728); }
  void set_text_sub_scene(uint32_t v) const { guest::StoreU32(base, address + 9728, v); }
  uint32_t text_sub_scene_address() const { return address + 9728; }
  uint32_t icon_sub_scene() const { return guest::LoadU32(base, address + 9732); }
  void set_icon_sub_scene(uint32_t v) const { guest::StoreU32(base, address + 9732, v); }
  uint32_t icon_sub_scene_address() const { return address + 9732; }
  // the color cursor's node; the sub-scene of color_cursor_child
  uint32_t color_cursor_node() const { return guest::LoadU32(base, address + 9736); }
  void set_color_cursor_node(uint32_t v) const { guest::StoreU32(base, address + 9736, v); }
  uint32_t color_cursor_node_address() const { return address + 9736; }
  // the scroll text's sub-scene
  uint32_t scroll_sub_scene() const { return guest::LoadU32(base, address + 9740); }
  void set_scroll_sub_scene(uint32_t v) const { guest::StoreU32(base, address + 9740, v); }
  uint32_t scroll_sub_scene_address() const { return address + 9740; }
  // the item cursor's node
  uint32_t item_cursor_node() const { return guest::LoadU32(base, address + 9744); }
  void set_item_cursor_node(uint32_t v) const { guest::StoreU32(base, address + 9744, v); }
  uint32_t item_cursor_node_address() const { return address + 9744; }
  // a child on selection_sub_scene shown by shown_child_on
  uint32_t shown_child() const { return guest::LoadU32(base, address + 9748); }
  void set_shown_child(uint32_t v) const { guest::StoreU32(base, address + 9748, v); }
  uint32_t shown_child_address() const { return address + 9748; }
  // the arrow SetArrows(0) sets
  uint32_t arrow_child_a() const { return guest::LoadU32(base, address + 9752); }
  void set_arrow_child_a(uint32_t v) const { guest::StoreU32(base, address + 9752, v); }
  uint32_t arrow_child_a_address() const { return address + 9752; }
  // the arrow SetArrows(1) sets
  uint32_t arrow_child_b() const { return guest::LoadU32(base, address + 9756); }
  void set_arrow_child_b(uint32_t v) const { guest::StoreU32(base, address + 9756, v); }
  uint32_t arrow_child_b_address() const { return address + 9756; }
  // a node (no sub-scene) given the selection
  uint32_t selection_node() const { return guest::LoadU32(base, address + 9760); }
  void set_selection_node(uint32_t v) const { guest::StoreU32(base, address + 9760, v); }
  uint32_t selection_node_address() const { return address + 9760; }
  // a child given the previous selection
  uint32_t previous_child() const { return guest::LoadU32(base, address + 9764); }
  void set_previous_child(uint32_t v) const { guest::StoreU32(base, address + 9764, v); }
  uint32_t previous_child_address() const { return address + 9764; }
  // a child given the selection
  uint32_t selection_child() const { return guest::LoadU32(base, address + 9768); }
  void set_selection_child(uint32_t v) const { guest::StoreU32(base, address + 9768, v); }
  uint32_t selection_child_address() const { return address + 9768; }
  // an array of children_count children given the previous selection (0 none)
  uint32_t previous_children() const { return guest::LoadU32(base, address + 9772); }
  void set_previous_children(uint32_t v) const { guest::StoreU32(base, address + 9772, v); }
  uint32_t previous_children_address() const { return address + 9772; }
  // an array given the selection (0 none; type 7 0x826371A4)
  uint32_t selection_children() const { return guest::LoadU32(base, address + 9776); }
  void set_selection_children(uint32_t v) const { guest::StoreU32(base, address + 9776, v); }
  uint32_t selection_children_address() const { return address + 9776; }
  // not written by the constructor
  uint32_t unknown_9780() const { return guest::LoadU32(base, address + 9780); }
  void set_unknown_9780(uint32_t v) const { guest::StoreU32(base, address + 9780, v); }
  uint32_t unknown_9780_address() const { return address + 9780; }
  uint32_t children_count() const { return guest::LoadU32(base, address + 9784); }
  void set_children_count(uint32_t v) const { guest::StoreU32(base, address + 9784, v); }
  uint32_t children_count_address() const { return address + 9784; }
  uint32_t shown_child_on() const { return guest::LoadU32(base, address + 9788); }
  void set_shown_child_on(uint32_t v) const { guest::StoreU32(base, address + 9788, v); }
  uint32_t shown_child_on_address() const { return address + 9788; }
  uint32_t unknown_9792() const { return guest::LoadU32(base, address + 9792); }
  void set_unknown_9792(uint32_t v) const { guest::StoreU32(base, address + 9792, v); }
  uint32_t unknown_9792_address() const { return address + 9792; }
  uint32_t unknown_9796() const { return guest::LoadU32(base, address + 9796); }
  void set_unknown_9796(uint32_t v) const { guest::StoreU32(base, address + 9796, v); }
  uint32_t unknown_9796_address() const { return address + 9796; }
  // the arrows' +108 when on
  uint32_t arrow_on_frame() const { return guest::LoadU32(base, address + 9800); }
  void set_arrow_on_frame(uint32_t v) const { guest::StoreU32(base, address + 9800, v); }
  uint32_t arrow_on_frame_address() const { return address + 9800; }
  // the arrows' +108 when off
  uint32_t arrow_off_frame() const { return guest::LoadU32(base, address + 9804); }
  void set_arrow_off_frame(uint32_t v) const { guest::StoreU32(base, address + 9804, v); }
  uint32_t arrow_off_frame_address() const { return address + 9804; }
  uint32_t color_cursor_child() const { return guest::LoadU32(base, address + 9808); }
  void set_color_cursor_child(uint32_t v) const { guest::StoreU32(base, address + 9808, v); }
  uint32_t color_cursor_child_address() const { return address + 9808; }
  // the first of six icon children (four textures, two hidden)
  uint32_t icon_child() const { return guest::LoadU32(base, address + 9812); }
  void set_icon_child(uint32_t v) const { guest::StoreU32(base, address + 9812, v); }
  uint32_t icon_child_address() const { return address + 9812; }
  // the scroll text's child
  uint32_t scroll_child() const { return guest::LoadU32(base, address + 9816); }
  void set_scroll_child(uint32_t v) const { guest::StoreU32(base, address + 9816, v); }
  uint32_t scroll_child_address() const { return address + 9816; }
  uint32_t unknown_9820() const { return guest::LoadU32(base, address + 9820); }
  void set_unknown_9820(uint32_t v) const { guest::StoreU32(base, address + 9820, v); }
  uint32_t unknown_9820_address() const { return address + 9820; }
  // type 7: a child on text_sub_scene shown with the part
  uint32_t part_child() const { return guest::LoadU32(base, address + 9824); }
  void set_part_child(uint32_t v) const { guest::StoreU32(base, address + 9824, v); }
  uint32_t part_child_address() const { return address + 9824; }
  uint32_t item_cursor_child() const { return guest::LoadU32(base, address + 9828); }
  void set_item_cursor_child(uint32_t v) const { guest::StoreU32(base, address + 9828, v); }
  uint32_t item_cursor_child_address() const { return address + 9828; }
  // the opening: played once
  uint32_t open_anim() const { return guest::LoadU32(base, address + 9832); }
  void set_open_anim(uint32_t v) const { guest::StoreU32(base, address + 9832, v); }
  uint32_t open_anim_address() const { return address + 9832; }
  // the leaving: played once
  uint32_t leave_anim() const { return guest::LoadU32(base, address + 9836); }
  void set_leave_anim(uint32_t v) const { guest::StoreU32(base, address + 9836, v); }
  uint32_t leave_anim_address() const { return address + 9836; }
  // type 7's step 2 (page 2 to 3)
  uint32_t first_to_items_anim() const { return guest::LoadU32(base, address + 9840); }
  void set_first_to_items_anim(uint32_t v) const { guest::StoreU32(base, address + 9840, v); }
  uint32_t first_to_items_anim_address() const { return address + 9840; }
  // type 7's step 3 (page 3 back to 2)
  uint32_t items_to_first_anim() const { return guest::LoadU32(base, address + 9844); }
  void set_items_to_first_anim(uint32_t v) const { guest::StoreU32(base, address + 9844, v); }
  uint32_t items_to_first_anim_address() const { return address + 9844; }
  // type 7's step 4 (page 3 to 4)
  uint32_t items_to_colors_anim() const { return guest::LoadU32(base, address + 9848); }
  void set_items_to_colors_anim(uint32_t v) const { guest::StoreU32(base, address + 9848, v); }
  uint32_t items_to_colors_anim_address() const { return address + 9848; }
  // type 7's step 6 (page 4 back to 3)
  uint32_t colors_to_items_anim() const { return guest::LoadU32(base, address + 9852); }
  void set_colors_to_items_anim(uint32_t v) const { guest::StoreU32(base, address + 9852, v); }
  uint32_t colors_to_items_anim_address() const { return address + 9852; }
  // the opening: played and looped
  uint32_t open_loop_a() const { return guest::LoadU32(base, address + 9856); }
  void set_open_loop_a(uint32_t v) const { guest::StoreU32(base, address + 9856, v); }
  uint32_t open_loop_a_address() const { return address + 9856; }
  // the opening: played and looped
  uint32_t open_loop_b() const { return guest::LoadU32(base, address + 9860); }
  void set_open_loop_b(uint32_t v) const { guest::StoreU32(base, address + 9860, v); }
  uint32_t open_loop_b_address() const { return address + 9860; }
  // the opening: chained
  uint32_t open_anim_b() const { return guest::LoadU32(base, address + 9864); }
  void set_open_anim_b(uint32_t v) const { guest::StoreU32(base, address + 9864, v); }
  uint32_t open_anim_b_address() const { return address + 9864; }
  // after the opening: looped
  uint32_t shown_loop_a() const { return guest::LoadU32(base, address + 9868); }
  void set_shown_loop_a(uint32_t v) const { guest::StoreU32(base, address + 9868, v); }
  uint32_t shown_loop_a_address() const { return address + 9868; }
  // the item list's loop (finished when it leaves)
  uint32_t items_loop() const { return guest::LoadU32(base, address + 9872); }
  void set_items_loop(uint32_t v) const { guest::StoreU32(base, address + 9872, v); }
  uint32_t items_loop_address() const { return address + 9872; }
  // after the opening: looped
  uint32_t shown_loop_b() const { return guest::LoadU32(base, address + 9876); }
  void set_shown_loop_b(uint32_t v) const { guest::StoreU32(base, address + 9876, v); }
  uint32_t shown_loop_b_address() const { return address + 9876; }
  // after the opening: looped
  uint32_t shown_loop_c() const { return guest::LoadU32(base, address + 9880); }
  void set_shown_loop_c(uint32_t v) const { guest::StoreU32(base, address + 9880, v); }
  uint32_t shown_loop_c_address() const { return address + 9880; }
  // after the opening: looped
  uint32_t shown_loop_d() const { return guest::LoadU32(base, address + 9884); }
  void set_shown_loop_d(uint32_t v) const { guest::StoreU32(base, address + 9884, v); }
  uint32_t shown_loop_d_address() const { return address + 9884; }
  // the opening: chained
  uint32_t open_anim_c() const { return guest::LoadU32(base, address + 9888); }
  void set_open_anim_c(uint32_t v) const { guest::StoreU32(base, address + 9888, v); }
  uint32_t open_anim_c_address() const { return address + 9888; }
  // the toggle's animation for its selection 0
  uint32_t toggle_anim_a() const { return guest::LoadU32(base, address + 9892); }
  void set_toggle_anim_a(uint32_t v) const { guest::StoreU32(base, address + 9892, v); }
  uint32_t toggle_anim_a_address() const { return address + 9892; }
  // the toggle's animation for its selection 1
  uint32_t toggle_anim_b() const { return guest::LoadU32(base, address + 9896); }
  void set_toggle_anim_b(uint32_t v) const { guest::StoreU32(base, address + 9896, v); }
  uint32_t toggle_anim_b_address() const { return address + 9896; }
  // replayed by ShowSelection
  uint32_t selection_anim() const { return guest::LoadU32(base, address + 9900); }
  void set_selection_anim(uint32_t v) const { guest::StoreU32(base, address + 9900, v); }
  uint32_t selection_anim_address() const { return address + 9900; }
  // an item list's move up
  uint32_t item_up_anim() const { return guest::LoadU32(base, address + 9904); }
  void set_item_up_anim(uint32_t v) const { guest::StoreU32(base, address + 9904, v); }
  uint32_t item_up_anim_address() const { return address + 9904; }
  // an item list's move down
  uint32_t item_down_anim() const { return guest::LoadU32(base, address + 9908); }
  void set_item_down_anim(uint32_t v) const { guest::StoreU32(base, address + 9908, v); }
  uint32_t item_down_anim_address() const { return address + 9908; }
  // type 7's first list moved back
  uint32_t first_back_anim() const { return guest::LoadU32(base, address + 9912); }
  void set_first_back_anim(uint32_t v) const { guest::StoreU32(base, address + 9912, v); }
  uint32_t first_back_anim_address() const { return address + 9912; }
  // type 7's first list moved forward
  uint32_t first_forward_anim() const { return guest::LoadU32(base, address + 9916); }
  void set_first_forward_anim(uint32_t v) const { guest::StoreU32(base, address + 9916, v); }
  uint32_t first_forward_anim_address() const { return address + 9916; }
  // type 7's color list moved back
  uint32_t color_back_anim() const { return guest::LoadU32(base, address + 9920); }
  void set_color_back_anim(uint32_t v) const { guest::StoreU32(base, address + 9920, v); }
  uint32_t color_back_anim_address() const { return address + 9920; }
  // type 7's color list moved forward
  uint32_t color_forward_anim() const { return guest::LoadU32(base, address + 9924); }
  void set_color_forward_anim(uint32_t v) const { guest::StoreU32(base, address + 9924, v); }
  uint32_t color_forward_anim_address() const { return address + 9924; }
  // a list scrolled down
  uint32_t scroll_down_anim() const { return guest::LoadU32(base, address + 9928); }
  void set_scroll_down_anim(uint32_t v) const { guest::StoreU32(base, address + 9928, v); }
  uint32_t scroll_down_anim_address() const { return address + 9928; }
  // a list scrolled up
  uint32_t scroll_up_anim() const { return guest::LoadU32(base, address + 9932); }
  void set_scroll_up_anim(uint32_t v) const { guest::StoreU32(base, address + 9932, v); }
  uint32_t scroll_up_anim_address() const { return address + 9932; }
  // ten named animations by the item list's selection (LoopItemNode)
  uint32_t item_anims_address() const { return address + 9936; }
  // item_anims[0]
  uint32_t item_anim_0() const { return guest::LoadU32(base, address + 9936); }
  void set_item_anim_0(uint32_t v) const { guest::StoreU32(base, address + 9936, v); }
  uint32_t item_anim_0_address() const { return address + 9936; }
  // ten by the first list's selection (LoopSecondNode)
  uint32_t first_anims_address() const { return address + 9976; }
  // first_anims[0]
  uint32_t first_anim_0() const { return guest::LoadU32(base, address + 9976); }
  void set_first_anim_0(uint32_t v) const { guest::StoreU32(base, address + 9976, v); }
  uint32_t first_anim_0_address() const { return address + 9976; }
  // ten by the color list's selection (LoopThirdNode)
  uint32_t color_anims_address() const { return address + 10016; }
  // color_anims[0]
  uint32_t color_anim_0() const { return guest::LoadU32(base, address + 10016); }
  void set_color_anim_0(uint32_t v) const { guest::StoreU32(base, address + 10016, v); }
  uint32_t color_anim_0_address() const { return address + 10016; }
  // ten by the item's selection (type 7's step 4 plays, step 6 finishes)
  uint32_t item_anims_b_address() const { return address + 10056; }
  // item_anims_b[0]
  uint32_t item_anim_b_0() const { return guest::LoadU32(base, address + 10056); }
  void set_item_anim_b_0(uint32_t v) const { guest::StoreU32(base, address + 10056, v); }
  uint32_t item_anim_b_0_address() const { return address + 10056; }
  // item_anims_b[1]
  uint32_t item_anim_b_1() const { return guest::LoadU32(base, address + 10060); }
  void set_item_anim_b_1(uint32_t v) const { guest::StoreU32(base, address + 10060, v); }
  uint32_t item_anim_b_1_address() const { return address + 10060; }
  // item_anims_b[2]
  uint32_t item_anim_b_2() const { return guest::LoadU32(base, address + 10064); }
  void set_item_anim_b_2(uint32_t v) const { guest::StoreU32(base, address + 10064, v); }
  uint32_t item_anim_b_2_address() const { return address + 10064; }
  // ten by the item's selection (type 7's step 4 finishes, step 6 plays)
  uint32_t item_anims_c_address() const { return address + 10096; }
  // item_anims_c[0]
  uint32_t item_anim_c_0() const { return guest::LoadU32(base, address + 10096); }
  void set_item_anim_c_0(uint32_t v) const { guest::StoreU32(base, address + 10096, v); }
  uint32_t item_anim_c_0_address() const { return address + 10096; }
  // item_anims_c[1]
  uint32_t item_anim_c_1() const { return guest::LoadU32(base, address + 10100); }
  void set_item_anim_c_1(uint32_t v) const { guest::StoreU32(base, address + 10100, v); }
  uint32_t item_anim_c_1_address() const { return address + 10100; }
  // item_anims_c[2]
  uint32_t item_anim_c_2() const { return guest::LoadU32(base, address + 10104); }
  void set_item_anim_c_2(uint32_t v) const { guest::StoreU32(base, address + 10104, v); }
  uint32_t item_anim_c_2_address() const { return address + 10104; }
  // cleared by ClearScrollText
  uint32_t scroll_text_shown() const { return guest::LoadU32(base, address + 10136); }
  void set_scroll_text_shown(uint32_t v) const { guest::StoreU32(base, address + 10136, v); }
  uint32_t scroll_text_shown_address() const { return address + 10136; }
  // the scroll text shown (-1 none)
  uint32_t scroll_text() const { return guest::LoadU32(base, address + 10140); }
  void set_scroll_text(uint32_t v) const { guest::StoreU32(base, address + 10140, v); }
  uint32_t scroll_text_address() const { return address + 10140; }
  // the item list: the course, 15 items (the route by its scroll and selection) (a MenuList, race-screens.toml)
  uint32_t course_list_address() const { return address + 10144; }
  // the list's +4 (scroll)
  uint32_t course_list_scroll() const { return guest::LoadU32(base, address + 10148); }
  void set_course_list_scroll(uint32_t v) const { guest::StoreU32(base, address + 10148, v); }
  uint32_t course_list_scroll_address() const { return address + 10148; }
  // the list's +8 (scroll_before)
  uint32_t course_list_scroll_before() const { return guest::LoadU32(base, address + 10152); }
  void set_course_list_scroll_before(uint32_t v) const { guest::StoreU32(base, address + 10152, v); }
  uint32_t course_list_scroll_before_address() const { return address + 10152; }
  // the list's +24 (move)
  uint32_t course_list_move() const { return guest::LoadU32(base, address + 10168); }
  void set_course_list_move(uint32_t v) const { guest::StoreU32(base, address + 10168, v); }
  uint32_t course_list_move_address() const { return address + 10168; }
  // the list's +32 (wrapped)
  uint32_t course_list_wrapped() const { return guest::LoadU32(base, address + 10176); }
  void set_course_list_wrapped(uint32_t v) const { guest::StoreU32(base, address + 10176, v); }
  uint32_t course_list_wrapped_address() const { return address + 10176; }
  // the list's +60 (player)
  uint32_t course_list_player() const { return guest::LoadU32(base, address + 10204); }
  void set_course_list_player(uint32_t v) const { guest::StoreU32(base, address + 10204, v); }
  uint32_t course_list_player_address() const { return address + 10204; }
  // the list's +68 (enabled)
  uint32_t course_list_enabled() const { return guest::LoadU32(base, address + 10212); }
  void set_course_list_enabled(uint32_t v) const { guest::StoreU32(base, address + 10212, v); }
  uint32_t course_list_enabled_address() const { return address + 10212; }
  // the color list (count from the item's info) (a MenuList, race-screens.toml)
  uint32_t color_list_address() const { return address + 10216; }
  // the list's +4 (scroll)
  uint32_t color_list_scroll() const { return guest::LoadU32(base, address + 10220); }
  void set_color_list_scroll(uint32_t v) const { guest::StoreU32(base, address + 10220, v); }
  uint32_t color_list_scroll_address() const { return address + 10220; }
  // the list's +8 (scroll_before)
  uint32_t color_list_scroll_before() const { return guest::LoadU32(base, address + 10224); }
  void set_color_list_scroll_before(uint32_t v) const { guest::StoreU32(base, address + 10224, v); }
  uint32_t color_list_scroll_before_address() const { return address + 10224; }
  // the list's +24 (move)
  uint32_t color_list_move() const { return guest::LoadU32(base, address + 10240); }
  void set_color_list_move(uint32_t v) const { guest::StoreU32(base, address + 10240, v); }
  uint32_t color_list_move_address() const { return address + 10240; }
  // the list's +32 (wrapped)
  uint32_t color_list_wrapped() const { return guest::LoadU32(base, address + 10248); }
  void set_color_list_wrapped(uint32_t v) const { guest::StoreU32(base, address + 10248, v); }
  uint32_t color_list_wrapped_address() const { return address + 10248; }
  // the list's +60 (player)
  uint32_t color_list_player() const { return guest::LoadU32(base, address + 10276); }
  void set_color_list_player(uint32_t v) const { guest::StoreU32(base, address + 10276, v); }
  uint32_t color_list_player_address() const { return address + 10276; }
  // the list's +68 (enabled)
  uint32_t color_list_enabled() const { return guest::LoadU32(base, address + 10284); }
  void set_color_list_enabled(uint32_t v) const { guest::StoreU32(base, address + 10284, v); }
  uint32_t color_list_enabled_address() const { return address + 10284; }
  // a two-item toggle (its selection 0: the race data's +0x59618 = 4) (a MenuList, race-screens.toml)
  uint32_t toggle_address() const { return address + 10288; }
  // the list's +4 (scroll)
  uint32_t toggle_scroll() const { return guest::LoadU32(base, address + 10292); }
  void set_toggle_scroll(uint32_t v) const { guest::StoreU32(base, address + 10292, v); }
  uint32_t toggle_scroll_address() const { return address + 10292; }
  // the list's +8 (scroll_before)
  uint32_t toggle_scroll_before() const { return guest::LoadU32(base, address + 10296); }
  void set_toggle_scroll_before(uint32_t v) const { guest::StoreU32(base, address + 10296, v); }
  uint32_t toggle_scroll_before_address() const { return address + 10296; }
  // the list's +24 (move)
  uint32_t toggle_move() const { return guest::LoadU32(base, address + 10312); }
  void set_toggle_move(uint32_t v) const { guest::StoreU32(base, address + 10312, v); }
  uint32_t toggle_move_address() const { return address + 10312; }
  // the list's +32 (wrapped)
  uint32_t toggle_wrapped() const { return guest::LoadU32(base, address + 10320); }
  void set_toggle_wrapped(uint32_t v) const { guest::StoreU32(base, address + 10320, v); }
  uint32_t toggle_wrapped_address() const { return address + 10320; }
  // the list's +60 (player)
  uint32_t toggle_player() const { return guest::LoadU32(base, address + 10348); }
  void set_toggle_player(uint32_t v) const { guest::StoreU32(base, address + 10348, v); }
  uint32_t toggle_player_address() const { return address + 10348; }
  // the list's +68 (enabled)
  uint32_t toggle_enabled() const { return guest::LoadU32(base, address + 10356); }
  void set_toggle_enabled(uint32_t v) const { guest::StoreU32(base, address + 10356, v); }
  uint32_t toggle_enabled_address() const { return address + 10356; }
  // the first list: the race kind (the race data's +32) (a MenuList, race-screens.toml)
  uint32_t kind_list_address() const { return address + 10360; }
  // the list's +4 (scroll)
  uint32_t kind_list_scroll() const { return guest::LoadU32(base, address + 10364); }
  void set_kind_list_scroll(uint32_t v) const { guest::StoreU32(base, address + 10364, v); }
  uint32_t kind_list_scroll_address() const { return address + 10364; }
  // the list's +8 (scroll_before)
  uint32_t kind_list_scroll_before() const { return guest::LoadU32(base, address + 10368); }
  void set_kind_list_scroll_before(uint32_t v) const { guest::StoreU32(base, address + 10368, v); }
  uint32_t kind_list_scroll_before_address() const { return address + 10368; }
  // the list's +24 (move)
  uint32_t kind_list_move() const { return guest::LoadU32(base, address + 10384); }
  void set_kind_list_move(uint32_t v) const { guest::StoreU32(base, address + 10384, v); }
  uint32_t kind_list_move_address() const { return address + 10384; }
  // the list's +32 (wrapped)
  uint32_t kind_list_wrapped() const { return guest::LoadU32(base, address + 10392); }
  void set_kind_list_wrapped(uint32_t v) const { guest::StoreU32(base, address + 10392, v); }
  uint32_t kind_list_wrapped_address() const { return address + 10392; }
  // the list's +60 (player)
  uint32_t kind_list_player() const { return guest::LoadU32(base, address + 10420); }
  void set_kind_list_player(uint32_t v) const { guest::StoreU32(base, address + 10420, v); }
  uint32_t kind_list_player_address() const { return address + 10420; }
  // the list's +68 (enabled)
  uint32_t kind_list_enabled() const { return guest::LoadU32(base, address + 10428); }
  void set_kind_list_enabled(uint32_t v) const { guest::StoreU32(base, address + 10428, v); }
  uint32_t kind_list_enabled_address() const { return address + 10428; }
  // the ColorNodeTable node_table points to
  uint32_t color_table_address() const { return address + 10432; }
  // the color table's movie
  uint32_t table_movie() const { return guest::LoadU32(base, address + 10436); }
  void set_table_movie(uint32_t v) const { guest::StoreU32(base, address + 10436, v); }
  uint32_t table_movie_address() const { return address + 10436; }
  uint32_t table_node_set_a() const { return guest::LoadU32(base, address + 10440); }
  void set_table_node_set_a(uint32_t v) const { guest::StoreU32(base, address + 10440, v); }
  uint32_t table_node_set_a_address() const { return address + 10440; }
  uint32_t table_node_set_b() const { return guest::LoadU32(base, address + 10444); }
  void set_table_node_set_b(uint32_t v) const { guest::StoreU32(base, address + 10444, v); }
  uint32_t table_node_set_b_address() const { return address + 10444; }
  // the color table's item_anims[0]
  uint32_t table_item_anim_0() const { return guest::LoadU32(base, address + 10448); }
  void set_table_item_anim_0(uint32_t v) const { guest::StoreU32(base, address + 10448, v); }
  uint32_t table_item_anim_0_address() const { return address + 10448; }
  // the color table's item_nodes[0]
  uint32_t table_item_node_0() const { return guest::LoadU32(base, address + 10588); }
  void set_table_item_node_0(uint32_t v) const { guest::StoreU32(base, address + 10588, v); }
  uint32_t table_item_node_0_address() const { return address + 10588; }
  // the color table's item_masks[0]
  uint32_t table_item_mask_0() const { return guest::LoadU32(base, address + 10744); }
  void set_table_item_mask_0(uint32_t v) const { guest::StoreU32(base, address + 10744, v); }
  uint32_t table_item_mask_0_address() const { return address + 10744; }
  uint32_t table_extra_node_base() const { return guest::LoadU32(base, address + 10852); }
  void set_table_extra_node_base(uint32_t v) const { guest::StoreU32(base, address + 10852, v); }
  uint32_t table_extra_node_base_address() const { return address + 10852; }
  uint32_t table_text_node_base() const { return guest::LoadU32(base, address + 10856); }
  void set_table_text_node_base(uint32_t v) const { guest::StoreU32(base, address + 10856, v); }
  uint32_t table_text_node_base_address() const { return address + 10856; }
  // the color table's color_child: LoadItem's color, -1 none
  uint32_t table_color_child() const { return guest::LoadU32(base, address + 10908); }
  void set_table_color_child(uint32_t v) const { guest::StoreU32(base, address + 10908, v); }
  uint32_t table_color_child_address() const { return address + 10908; }
  // the color table's grid_a and grid_b
  uint32_t table_grids_address() const { return address + 10912; }
  // the color table's +484
  uint32_t table_word_10916() const { return guest::LoadU32(base, address + 10916); }
  void set_table_word_10916(uint32_t v) const { guest::StoreU32(base, address + 10916, v); }
  uint32_t table_word_10916_address() const { return address + 10916; }
  // the color table's +488
  uint32_t table_word_10920() const { return guest::LoadU32(base, address + 10920); }
  void set_table_word_10920(uint32_t v) const { guest::StoreU32(base, address + 10920, v); }
  uint32_t table_word_10920_address() const { return address + 10920; }
  // the color table's +492
  uint32_t table_word_10924() const { return guest::LoadU32(base, address + 10924); }
  void set_table_word_10924(uint32_t v) const { guest::StoreU32(base, address + 10924, v); }
  uint32_t table_word_10924_address() const { return address + 10924; }
  // the color table's +500
  uint32_t table_word_10932() const { return guest::LoadU32(base, address + 10932); }
  void set_table_word_10932(uint32_t v) const { guest::StoreU32(base, address + 10932, v); }
  uint32_t table_word_10932_address() const { return address + 10932; }
  // the color table's +504
  uint32_t table_word_10936() const { return guest::LoadU32(base, address + 10936); }
  void set_table_word_10936(uint32_t v) const { guest::StoreU32(base, address + 10936, v); }
  uint32_t table_word_10936_address() const { return address + 10936; }
  // the color table's +508
  uint32_t table_word_10940() const { return guest::LoadU32(base, address + 10940); }
  void set_table_word_10940(uint32_t v) const { guest::StoreU32(base, address + 10940, v); }
  uint32_t table_word_10940_address() const { return address + 10940; }
  // the color table's +520
  uint32_t table_word_10952() const { return guest::LoadU32(base, address + 10952); }
  void set_table_word_10952(uint32_t v) const { guest::StoreU32(base, address + 10952, v); }
  uint32_t table_word_10952_address() const { return address + 10952; }
  // the color table's +524
  uint32_t table_word_10956() const { return guest::LoadU32(base, address + 10956); }
  void set_table_word_10956(uint32_t v) const { guest::StoreU32(base, address + 10956, v); }
  uint32_t table_word_10956_address() const { return address + 10956; }
  // the color table's +528
  uint32_t table_word_10960() const { return guest::LoadU32(base, address + 10960); }
  void set_table_word_10960(uint32_t v) const { guest::StoreU32(base, address + 10960, v); }
  uint32_t table_word_10960_address() const { return address + 10960; }
  // the color table's +536
  uint32_t table_word_10968() const { return guest::LoadU32(base, address + 10968); }
  void set_table_word_10968(uint32_t v) const { guest::StoreU32(base, address + 10968, v); }
  uint32_t table_word_10968_address() const { return address + 10968; }
  // the color table's +540
  uint32_t table_word_10972() const { return guest::LoadU32(base, address + 10972); }
  void set_table_word_10972(uint32_t v) const { guest::StoreU32(base, address + 10972, v); }
  uint32_t table_word_10972_address() const { return address + 10972; }
  // the color table's +544
  uint32_t table_word_10976() const { return guest::LoadU32(base, address + 10976); }
  void set_table_word_10976(uint32_t v) const { guest::StoreU32(base, address + 10976, v); }
  uint32_t table_word_10976_address() const { return address + 10976; }
  // the color table's color_a and color_b
  uint32_t table_colors_address() const { return address + 10984; }
  // the scroll texts' string ids (545 to 549, then -1)
  uint32_t text_items() const { return guest::LoadU32(base, address + 11016); }
  void set_text_items(uint32_t v) const { guest::StoreU32(base, address + 11016, v); }
  uint32_t text_items_address() const { return address + 11016; }
  uint32_t unknown_11020() const { return guest::LoadU32(base, address + 11020); }
  void set_unknown_11020(uint32_t v) const { guest::StoreU32(base, address + 11020, v); }
  uint32_t unknown_11020_address() const { return address + 11020; }
  uint32_t unknown_11024() const { return guest::LoadU32(base, address + 11024); }
  void set_unknown_11024(uint32_t v) const { guest::StoreU32(base, address + 11024, v); }
  uint32_t unknown_11024_address() const { return address + 11024; }
  // steps 2 and 6
  uint32_t text_item_moved() const { return guest::LoadU32(base, address + 11028); }
  void set_text_item_moved(uint32_t v) const { guest::StoreU32(base, address + 11028, v); }
  uint32_t text_item_moved_address() const { return address + 11028; }
  // step 4, and back from the list part
  uint32_t text_colors() const { return guest::LoadU32(base, address + 11032); }
  void set_text_colors(uint32_t v) const { guest::StoreU32(base, address + 11032, v); }
  uint32_t text_colors_address() const { return address + 11032; }
  // page 5 (the list part)
  uint32_t text_part() const { return guest::LoadU32(base, address + 11036); }
  void set_text_part(uint32_t v) const { guest::StoreU32(base, address + 11036, v); }
  uint32_t text_part_address() const { return address + 11036; }
  // the ListPart list_part points to
  uint32_t part_address() const { return address + 11040; }
};

// image; measure: ListScreen.+0, NodeListScreen.+0, TitledListScreen.+0, Type6ListScreen.+0, ColorListScreen.+0, Type7ListScreen.+0
struct ListScreenVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 112;
  uint32_t deleting_destructor() const { return guest::LoadU32(base, address + 0); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t deleting_destructor_address() const { return address + 0; }
  // UiMovies::IsLoaded of the movie
  uint32_t is_loaded() const { return guest::LoadU32(base, address + 4); }
  void set_is_loaded(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t is_loaded_address() const { return address + 4; }
  // type 6 SetItemTexts, type 7 SetItemText
  uint32_t set_item_texts() const { return guest::LoadU32(base, address + 8); }
  void set_set_item_texts(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t set_item_texts_address() const { return address + 8; }
  // type 7 ClearItemText
  uint32_t clear_item_text() const { return guest::LoadU32(base, address + 12); }
  void set_clear_item_text(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t clear_item_text_address() const { return address + 12; }
  uint32_t slot4() const { return guest::LoadU32(base, address + 16); }
  void set_slot4(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t slot4_address() const { return address + 16; }
  uint32_t slot5() const { return guest::LoadU32(base, address + 20); }
  void set_slot5(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t slot5_address() const { return address + 20; }
  uint32_t back() const { return guest::LoadU32(base, address + 24); }
  void set_back(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t back_address() const { return address + 24; }
  // the race set up (slot 7)
  uint32_t set_up_race() const { return guest::LoadU32(base, address + 28); }
  void set_set_up_race(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t set_up_race_address() const { return address + 28; }
  // from OnEnter
  uint32_t on_enter() const { return guest::LoadU32(base, address + 32); }
  void set_on_enter(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t on_enter_address() const { return address + 32; }
  // from OnLeft
  uint32_t on_left() const { return guest::LoadU32(base, address + 36); }
  void set_on_left(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t on_left_address() const { return address + 36; }
  // after draw_page, from Draw
  uint32_t draw_more() const { return guest::LoadU32(base, address + 40); }
  void set_draw_more(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t draw_more_address() const { return address + 40; }
  uint32_t slot11() const { return guest::LoadU32(base, address + 44); }
  void set_slot11(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t slot11_address() const { return address + 44; }
  uint32_t open_movie() const { return guest::LoadU32(base, address + 48); }
  void set_open_movie(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t open_movie_address() const { return address + 48; }
  // from WaitForMovie once loaded
  uint32_t on_movie_loaded() const { return guest::LoadU32(base, address + 52); }
  void set_on_movie_loaded(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t on_movie_loaded_address() const { return address + 52; }
  uint32_t set_up_menus() const { return guest::LoadU32(base, address + 56); }
  void set_set_up_menus(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t set_up_menus_address() const { return address + 56; }
  uint32_t open() const { return guest::LoadU32(base, address + 60); }
  void set_open(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t open_address() const { return address + 60; }
  uint32_t suspend() const { return guest::LoadU32(base, address + 64); }
  void set_suspend(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t suspend_address() const { return address + 64; }
  uint32_t resume() const { return guest::LoadU32(base, address + 68); }
  void set_resume(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t resume_address() const { return address + 68; }
  // deletes the movie (from Leave)
  uint32_t release() const { return guest::LoadU32(base, address + 72); }
  void set_release(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t release_address() const { return address + 72; }
  uint32_t slot19() const { return guest::LoadU32(base, address + 76); }
  void set_slot19(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t slot19_address() const { return address + 76; }
  uint32_t slot20() const { return guest::LoadU32(base, address + 80); }
  void set_slot20(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t slot20_address() const { return address + 80; }
  uint32_t slot21() const { return guest::LoadU32(base, address + 84); }
  void set_slot21(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t slot21_address() const { return address + 84; }
  // the page block's reset (from Show)
  uint32_t reset() const { return guest::LoadU32(base, address + 88); }
  void set_reset(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t reset_address() const { return address + 88; }
  // runs the page
  uint32_t draw_page() const { return guest::LoadU32(base, address + 92); }
  void set_draw_page(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t draw_page_address() const { return address + 92; }
  // runs the step
  uint32_t update_step() const { return guest::LoadU32(base, address + 96); }
  void set_update_step(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t update_step_address() const { return address + 96; }
  // the item's info into r5
  uint32_t item_info() const { return guest::LoadU32(base, address + 100); }
  void set_item_info(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t item_info_address() const { return address + 100; }
  // type 6 ShowMarks, type 7 SetDetails
  uint32_t show_marks() const { return guest::LoadU32(base, address + 104); }
  void set_show_marks(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t show_marks_address() const { return address + 104; }
  // type 7 ShowMarks
  uint32_t show_marks_b() const { return guest::LoadU32(base, address + 108); }
  void set_show_marks_b(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t show_marks_b_address() const { return address + 108; }
};

// embedded; measure: NodeScreen, Type6NodeScreen, sub_822370A0.r3, NodeScreen.+10324
struct NodeScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 10552;
  static constexpr uint32_t kVtable = 0x82010A80;
  // 0x82010A80 (Type6NodeScreen's); NodeScreen's 0x820116B8
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the UI movie's id
  uint32_t table_movie() const { return guest::LoadU32(base, address + 4); }
  void set_table_movie(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t table_movie_address() const { return address + 4; }
  // the sub-scene of the extra nodes (+420, +424, +264)
  uint32_t table_node_set_a() const { return guest::LoadU32(base, address + 8); }
  void set_table_node_set_a(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t table_node_set_a_address() const { return address + 8; }
  // the sub-scene of the items' nodes
  uint32_t table_node_set_b() const { return guest::LoadU32(base, address + 12); }
  void set_table_node_set_b(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t table_node_set_b_address() const { return address + 12; }
  // 27 items' named animations
  uint32_t table_item_anims_address() const { return address + 16; }
  // eight more (the third pair's, +444)
  uint32_t table_more_anims_address() const { return address + 124; }
  // 27 items' nodes
  uint32_t table_item_nodes_address() const { return address + 156; }
  // eight nodes (hidden at the reset)
  uint32_t table_nodes_address() const { return address + 264; }
  // the four children's indexes (marks and colors)
  uint32_t table_children_address() const { return address + 296; }
  // 27 items' masks of the four children
  uint32_t table_item_masks_address() const { return address + 312; }
  // the item nodes' twins on node_set_a (-1 none)
  uint32_t table_extra_node_base() const { return guest::LoadU32(base, address + 420); }
  void set_table_extra_node_base(uint32_t v) const { guest::StoreU32(base, address + 420, v); }
  uint32_t table_extra_node_base_address() const { return address + 420; }
  // four text nodes (strings 402, 401, 407, 408; -1 none)
  uint32_t table_text_node_base() const { return guest::LoadU32(base, address + 424); }
  void set_table_text_node_base(uint32_t v) const { guest::StoreU32(base, address + 424, v); }
  uint32_t table_text_node_base_address() const { return address + 424; }
  // the first pair: the item now
  uint32_t table_item() const { return guest::LoadU32(base, address + 428); }
  void set_table_item(uint32_t v) const { guest::StoreU32(base, address + 428, v); }
  uint32_t table_item_address() const { return address + 428; }
  // the item before
  uint32_t table_item_before() const { return guest::LoadU32(base, address + 432); }
  void set_table_item_before(uint32_t v) const { guest::StoreU32(base, address + 432, v); }
  uint32_t table_item_before_address() const { return address + 432; }
  // the second pair: an item now
  uint32_t table_second() const { return guest::LoadU32(base, address + 436); }
  void set_table_second(uint32_t v) const { guest::StoreU32(base, address + 436, v); }
  uint32_t table_second_address() const { return address + 436; }
  uint32_t table_second_before() const { return guest::LoadU32(base, address + 440); }
  void set_table_second_before(uint32_t v) const { guest::StoreU32(base, address + 440, v); }
  uint32_t table_second_before_address() const { return address + 440; }
  // the third pair: an index into more_anims now
  uint32_t table_third() const { return guest::LoadU32(base, address + 444); }
  void set_table_third(uint32_t v) const { guest::StoreU32(base, address + 444, v); }
  uint32_t table_third_address() const { return address + 444; }
  uint32_t table_third_before() const { return guest::LoadU32(base, address + 448); }
  void set_table_third_before(uint32_t v) const { guest::StoreU32(base, address + 448, v); }
  uint32_t table_third_before_address() const { return address + 448; }
  // the fourth pair: the paired table's mask bit; the color table's grid column
  uint32_t table_fourth() const { return guest::LoadU32(base, address + 452); }
  void set_table_fourth(uint32_t v) const { guest::StoreU32(base, address + 452, v); }
  uint32_t table_fourth_address() const { return address + 452; }
  uint32_t table_fourth_before() const { return guest::LoadU32(base, address + 456); }
  void set_table_fourth_before(uint32_t v) const { guest::StoreU32(base, address + 456, v); }
  uint32_t table_fourth_before_address() const { return address + 456; }
  // a ScreenMachine (boot-mode.md)
  uint32_t machine_address() const { return address + 460; }
  // the machine's state (its +9604)
  uint32_t state() const { return guest::LoadU32(base, address + 10064); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 10064, v); }
  uint32_t state_address() const { return address + 10064; }
  uint32_t movie_loaded() const { return guest::LoadU32(base, address + 10068); }
  void set_movie_loaded(uint32_t v) const { guest::StoreU32(base, address + 10068, v); }
  uint32_t movie_loaded_address() const { return address + 10068; }
  // open the movie at the mode's start (else wait for the open message)
  uint32_t open_at_start() const { return guest::LoadU32(base, address + 10072); }
  void set_open_at_start(uint32_t v) const { guest::StoreU32(base, address + 10072, v); }
  uint32_t open_at_start_address() const { return address + 10072; }
  // the UI movie's id (15)
  uint32_t movie() const { return guest::LoadU32(base, address + 10100); }
  void set_movie(uint32_t v) const { guest::StoreU32(base, address + 10100, v); }
  uint32_t movie_address() const { return address + 10100; }
  // the page Draw runs (-1 while a step runs)
  uint32_t page() const { return guest::LoadU32(base, address + 10104); }
  void set_page(uint32_t v) const { guest::StoreU32(base, address + 10104, v); }
  uint32_t page_address() const { return address + 10104; }
  // the page after the step
  uint32_t next_page() const { return guest::LoadU32(base, address + 10108); }
  void set_next_page(uint32_t v) const { guest::StoreU32(base, address + 10108, v); }
  uint32_t next_page_address() const { return address + 10108; }
  uint32_t previous_page() const { return guest::LoadU32(base, address + 10112); }
  void set_previous_page(uint32_t v) const { guest::StoreU32(base, address + 10112, v); }
  uint32_t previous_page_address() const { return address + 10112; }
  // the step Update runs (-1 none)
  uint32_t step() const { return guest::LoadU32(base, address + 10116); }
  void set_step(uint32_t v) const { guest::StoreU32(base, address + 10116, v); }
  uint32_t step_address() const { return address + 10116; }
  // the step after it (-1 none)
  uint32_t queued_step() const { return guest::LoadU32(base, address + 10120); }
  void set_queued_step(uint32_t v) const { guest::StoreU32(base, address + 10120, v); }
  uint32_t queued_step_address() const { return address + 10120; }
  // the page block's +20: 0 when a queued step starts
  uint32_t unknown_10124() const { return guest::LoadU32(base, address + 10124); }
  void set_unknown_10124(uint32_t v) const { guest::StoreU32(base, address + 10124, v); }
  uint32_t unknown_10124_address() const { return address + 10124; }
  // set by a step once its first call ran; 0 when the block moves on
  uint32_t step_started() const { return guest::LoadU32(base, address + 10128); }
  void set_step_started(uint32_t v) const { guest::StoreU32(base, address + 10128, v); }
  uint32_t step_started_address() const { return address + 10128; }
  // the course list (8 items, 6 shown) (a MenuList, race-screens.toml)
  uint32_t course_list_address() const { return address + 10132; }
  // the list's +4 (scroll)
  uint32_t course_list_scroll() const { return guest::LoadU32(base, address + 10136); }
  void set_course_list_scroll(uint32_t v) const { guest::StoreU32(base, address + 10136, v); }
  uint32_t course_list_scroll_address() const { return address + 10136; }
  // the list's +8 (scroll_before)
  uint32_t course_list_scroll_before() const { return guest::LoadU32(base, address + 10140); }
  void set_course_list_scroll_before(uint32_t v) const { guest::StoreU32(base, address + 10140, v); }
  uint32_t course_list_scroll_before_address() const { return address + 10140; }
  // the list's +24 (move)
  uint32_t course_list_move() const { return guest::LoadU32(base, address + 10156); }
  void set_course_list_move(uint32_t v) const { guest::StoreU32(base, address + 10156, v); }
  uint32_t course_list_move_address() const { return address + 10156; }
  // the list's +32 (wrapped)
  uint32_t course_list_wrapped() const { return guest::LoadU32(base, address + 10164); }
  void set_course_list_wrapped(uint32_t v) const { guest::StoreU32(base, address + 10164, v); }
  uint32_t course_list_wrapped_address() const { return address + 10164; }
  // the list's +60 (player)
  uint32_t course_list_player() const { return guest::LoadU32(base, address + 10192); }
  void set_course_list_player(uint32_t v) const { guest::StoreU32(base, address + 10192, v); }
  uint32_t course_list_player_address() const { return address + 10192; }
  // the list's +68 (enabled)
  uint32_t course_list_enabled() const { return guest::LoadU32(base, address + 10200); }
  void set_course_list_enabled(uint32_t v) const { guest::StoreU32(base, address + 10200, v); }
  uint32_t course_list_enabled_address() const { return address + 10200; }
  // a two-item toggle (the traffic: ShowRecords' r5) (a MenuList, race-screens.toml)
  uint32_t toggle_address() const { return address + 10204; }
  // the list's +4 (scroll)
  uint32_t toggle_scroll() const { return guest::LoadU32(base, address + 10208); }
  void set_toggle_scroll(uint32_t v) const { guest::StoreU32(base, address + 10208, v); }
  uint32_t toggle_scroll_address() const { return address + 10208; }
  // the list's +8 (scroll_before)
  uint32_t toggle_scroll_before() const { return guest::LoadU32(base, address + 10212); }
  void set_toggle_scroll_before(uint32_t v) const { guest::StoreU32(base, address + 10212, v); }
  uint32_t toggle_scroll_before_address() const { return address + 10212; }
  // the list's +24 (move)
  uint32_t toggle_move() const { return guest::LoadU32(base, address + 10228); }
  void set_toggle_move(uint32_t v) const { guest::StoreU32(base, address + 10228, v); }
  uint32_t toggle_move_address() const { return address + 10228; }
  // the list's +32 (wrapped)
  uint32_t toggle_wrapped() const { return guest::LoadU32(base, address + 10236); }
  void set_toggle_wrapped(uint32_t v) const { guest::StoreU32(base, address + 10236, v); }
  uint32_t toggle_wrapped_address() const { return address + 10236; }
  // the list's +60 (player)
  uint32_t toggle_player() const { return guest::LoadU32(base, address + 10264); }
  void set_toggle_player(uint32_t v) const { guest::StoreU32(base, address + 10264, v); }
  uint32_t toggle_player_address() const { return address + 10264; }
  // the list's +68 (enabled)
  uint32_t toggle_enabled() const { return guest::LoadU32(base, address + 10272); }
  void set_toggle_enabled(uint32_t v) const { guest::StoreU32(base, address + 10272, v); }
  uint32_t toggle_enabled_address() const { return address + 10272; }
  // the texts' and arrows' sub-scene
  uint32_t sub_scene() const { return guest::LoadU32(base, address + 10276); }
  void set_sub_scene(uint32_t v) const { guest::StoreU32(base, address + 10276, v); }
  uint32_t sub_scene_address() const { return address + 10276; }
  uint32_t cursor_node() const { return guest::LoadU32(base, address + 10280); }
  void set_cursor_node(uint32_t v) const { guest::StoreU32(base, address + 10280, v); }
  uint32_t cursor_node_address() const { return address + 10280; }
  // the opening: played once
  uint32_t open_anim() const { return guest::LoadU32(base, address + 10284); }
  void set_open_anim(uint32_t v) const { guest::StoreU32(base, address + 10284, v); }
  uint32_t open_anim_address() const { return address + 10284; }
  // the leaving: played once
  uint32_t leave_anim() const { return guest::LoadU32(base, address + 10288); }
  void set_leave_anim(uint32_t v) const { guest::StoreU32(base, address + 10288, v); }
  uint32_t leave_anim_address() const { return address + 10288; }
  // the opening: played and looped
  uint32_t open_loop_a() const { return guest::LoadU32(base, address + 10292); }
  void set_open_loop_a(uint32_t v) const { guest::StoreU32(base, address + 10292, v); }
  uint32_t open_loop_a_address() const { return address + 10292; }
  // the opening: played and looped
  uint32_t open_loop_b() const { return guest::LoadU32(base, address + 10296); }
  void set_open_loop_b(uint32_t v) const { guest::StoreU32(base, address + 10296, v); }
  uint32_t open_loop_b_address() const { return address + 10296; }
  // the opening: chained
  uint32_t open_anim_b() const { return guest::LoadU32(base, address + 10300); }
  void set_open_anim_b(uint32_t v) const { guest::StoreU32(base, address + 10300, v); }
  uint32_t open_anim_b_address() const { return address + 10300; }
  // after the opening: looped
  uint32_t shown_loop_a() const { return guest::LoadU32(base, address + 10304); }
  void set_shown_loop_a(uint32_t v) const { guest::StoreU32(base, address + 10304, v); }
  uint32_t shown_loop_a_address() const { return address + 10304; }
  // after the opening: looped
  uint32_t shown_loop_b() const { return guest::LoadU32(base, address + 10308); }
  void set_shown_loop_b(uint32_t v) const { guest::StoreU32(base, address + 10308, v); }
  uint32_t shown_loop_b_address() const { return address + 10308; }
  // after the opening: looped
  uint32_t shown_loop_c() const { return guest::LoadU32(base, address + 10312); }
  void set_shown_loop_c(uint32_t v) const { guest::StoreU32(base, address + 10312, v); }
  uint32_t shown_loop_c_address() const { return address + 10312; }
  // after the opening: looped
  uint32_t shown_loop_d() const { return guest::LoadU32(base, address + 10316); }
  void set_shown_loop_d(uint32_t v) const { guest::StoreU32(base, address + 10316, v); }
  uint32_t shown_loop_d_address() const { return address + 10316; }
  // the opening: chained
  uint32_t open_anim_c() const { return guest::LoadU32(base, address + 10320); }
  void set_open_anim_c(uint32_t v) const { guest::StoreU32(base, address + 10320, v); }
  uint32_t open_anim_c_address() const { return address + 10320; }
  // the toggle's animation for its selection 0
  uint32_t toggle_anim_a() const { return guest::LoadU32(base, address + 10324); }
  void set_toggle_anim_a(uint32_t v) const { guest::StoreU32(base, address + 10324, v); }
  uint32_t toggle_anim_a_address() const { return address + 10324; }
  // the toggle's animation for its selection 1
  uint32_t toggle_anim_b() const { return guest::LoadU32(base, address + 10328); }
  void set_toggle_anim_b(uint32_t v) const { guest::StoreU32(base, address + 10328, v); }
  uint32_t toggle_anim_b_address() const { return address + 10328; }
  uint32_t unknown_10332() const { return guest::LoadU32(base, address + 10332); }
  void set_unknown_10332(uint32_t v) const { guest::StoreU32(base, address + 10332, v); }
  uint32_t unknown_10332_address() const { return address + 10332; }
  uint32_t item_up_anim() const { return guest::LoadU32(base, address + 10336); }
  void set_item_up_anim(uint32_t v) const { guest::StoreU32(base, address + 10336, v); }
  uint32_t item_up_anim_address() const { return address + 10336; }
  uint32_t item_down_anim() const { return guest::LoadU32(base, address + 10340); }
  void set_item_down_anim(uint32_t v) const { guest::StoreU32(base, address + 10340, v); }
  uint32_t item_down_anim_address() const { return address + 10340; }
  uint32_t scroll_down_anim() const { return guest::LoadU32(base, address + 10344); }
  void set_scroll_down_anim(uint32_t v) const { guest::StoreU32(base, address + 10344, v); }
  uint32_t scroll_down_anim_address() const { return address + 10344; }
  uint32_t scroll_up_anim() const { return guest::LoadU32(base, address + 10348); }
  void set_scroll_up_anim(uint32_t v) const { guest::StoreU32(base, address + 10348, v); }
  uint32_t scroll_up_anim_address() const { return address + 10348; }
  // ten named animations by the list's selection
  uint32_t item_anims_address() const { return address + 10352; }
  // item_anims[0]
  uint32_t item_anim_0() const { return guest::LoadU32(base, address + 10352); }
  void set_item_anim_0(uint32_t v) const { guest::StoreU32(base, address + 10352, v); }
  uint32_t item_anim_0_address() const { return address + 10352; }
  // played and looped by the opening unless active
  uint32_t records_anim() const { return guest::LoadU32(base, address + 10392); }
  void set_records_anim(uint32_t v) const { guest::StoreU32(base, address + 10392, v); }
  uint32_t records_anim_address() const { return address + 10392; }
  uint32_t arrow_child_a() const { return guest::LoadU32(base, address + 10396); }
  void set_arrow_child_a(uint32_t v) const { guest::StoreU32(base, address + 10396, v); }
  uint32_t arrow_child_a_address() const { return address + 10396; }
  uint32_t arrow_child_b() const { return guest::LoadU32(base, address + 10400); }
  void set_arrow_child_b(uint32_t v) const { guest::StoreU32(base, address + 10400, v); }
  uint32_t arrow_child_b_address() const { return address + 10400; }
  // the title text's child (string 322)
  uint32_t title_child() const { return guest::LoadU32(base, address + 10404); }
  void set_title_child(uint32_t v) const { guest::StoreU32(base, address + 10404, v); }
  uint32_t title_child_address() const { return address + 10404; }
  uint32_t text_child_a() const { return guest::LoadU32(base, address + 10408); }
  void set_text_child_a(uint32_t v) const { guest::StoreU32(base, address + 10408, v); }
  uint32_t text_child_a_address() const { return address + 10408; }
  uint32_t text_child_b() const { return guest::LoadU32(base, address + 10412); }
  void set_text_child_b(uint32_t v) const { guest::StoreU32(base, address + 10412, v); }
  uint32_t text_child_b_address() const { return address + 10412; }
  // three columns of ten text children: the times, then two strings
  uint32_t record_texts_address() const { return address + 10416; }
  uint32_t arrow_on_frame() const { return guest::LoadU32(base, address + 10536); }
  void set_arrow_on_frame(uint32_t v) const { guest::StoreU32(base, address + 10536, v); }
  uint32_t arrow_on_frame_address() const { return address + 10536; }
  uint32_t arrow_off_frame() const { return guest::LoadU32(base, address + 10540); }
  void set_arrow_off_frame(uint32_t v) const { guest::StoreU32(base, address + 10540, v); }
  uint32_t arrow_off_frame_address() const { return address + 10540; }
  // the node given the course's texture
  uint32_t course_node() const { return guest::LoadU32(base, address + 10544); }
  void set_course_node(uint32_t v) const { guest::StoreU32(base, address + 10544, v); }
  uint32_t course_node_address() const { return address + 10544; }
  uint32_t cursor_child() const { return guest::LoadU32(base, address + 10548); }
  void set_cursor_child(uint32_t v) const { guest::StoreU32(base, address + 10548, v); }
  uint32_t cursor_child_address() const { return address + 10548; }
};

// image; measure: NodeScreen.+0, Type6NodeScreen.+0
struct NodeScreenVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 92;
  uint32_t deleting_destructor() const { return guest::LoadU32(base, address + 0); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t deleting_destructor_address() const { return address + 0; }
  uint32_t set_up_texts() const { return guest::LoadU32(base, address + 4); }
  void set_set_up_texts(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t set_up_texts_address() const { return address + 4; }
  uint32_t set_text() const { return guest::LoadU32(base, address + 8); }
  void set_set_text(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t set_text_address() const { return address + 8; }
  uint32_t is_loaded() const { return guest::LoadU32(base, address + 12); }
  void set_is_loaded(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t is_loaded_address() const { return address + 12; }
  uint32_t item_info() const { return guest::LoadU32(base, address + 16); }
  void set_item_info(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t item_info_address() const { return address + 16; }
  uint32_t slot5() const { return guest::LoadU32(base, address + 20); }
  void set_slot5(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t slot5_address() const { return address + 20; }
  uint32_t back() const { return guest::LoadU32(base, address + 24); }
  void set_back(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t back_address() const { return address + 24; }
  uint32_t slot7() const { return guest::LoadU32(base, address + 28); }
  void set_slot7(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t slot7_address() const { return address + 28; }
  uint32_t slot8() const { return guest::LoadU32(base, address + 32); }
  void set_slot8(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t slot8_address() const { return address + 32; }
  uint32_t on_enter() const { return guest::LoadU32(base, address + 36); }
  void set_on_enter(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t on_enter_address() const { return address + 36; }
  uint32_t on_left() const { return guest::LoadU32(base, address + 40); }
  void set_on_left(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t on_left_address() const { return address + 40; }
  uint32_t slot11() const { return guest::LoadU32(base, address + 44); }
  void set_slot11(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t slot11_address() const { return address + 44; }
  uint32_t slot12() const { return guest::LoadU32(base, address + 48); }
  void set_slot12(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t slot12_address() const { return address + 48; }
  uint32_t open_movie() const { return guest::LoadU32(base, address + 52); }
  void set_open_movie(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t open_movie_address() const { return address + 52; }
  uint32_t on_movie_loaded() const { return guest::LoadU32(base, address + 56); }
  void set_on_movie_loaded(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t on_movie_loaded_address() const { return address + 56; }
  uint32_t set_up_menus() const { return guest::LoadU32(base, address + 60); }
  void set_set_up_menus(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t set_up_menus_address() const { return address + 60; }
  uint32_t slot16() const { return guest::LoadU32(base, address + 64); }
  void set_slot16(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t slot16_address() const { return address + 64; }
  uint32_t slot17() const { return guest::LoadU32(base, address + 68); }
  void set_slot17(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t slot17_address() const { return address + 68; }
  uint32_t slot18() const { return guest::LoadU32(base, address + 72); }
  void set_slot18(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t slot18_address() const { return address + 72; }
  uint32_t release() const { return guest::LoadU32(base, address + 76); }
  void set_release(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t release_address() const { return address + 76; }
  uint32_t set_icons() const { return guest::LoadU32(base, address + 80); }
  void set_set_icons(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t set_icons_address() const { return address + 80; }
  uint32_t set_scroll_text() const { return guest::LoadU32(base, address + 84); }
  void set_set_scroll_text(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t set_scroll_text_address() const { return address + 84; }
  uint32_t show_marks() const { return guest::LoadU32(base, address + 88); }
  void set_show_marks(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t show_marks_address() const { return address + 88; }
};

// embedded; measure: PanelScreen
struct PanelScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 11684;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // a ScreenMachine (boot-mode.md)
  uint32_t machine_address() const { return address + 4; }
  // the machine's state (its +9604)
  uint32_t state() const { return guest::LoadU32(base, address + 9608); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t state_address() const { return address + 9608; }
  uint32_t movie_loaded() const { return guest::LoadU32(base, address + 9612); }
  void set_movie_loaded(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t movie_loaded_address() const { return address + 9612; }
  // open the movie at the mode's start (else wait for the open message)
  uint32_t open_at_start() const { return guest::LoadU32(base, address + 9616); }
  void set_open_at_start(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t open_at_start_address() const { return address + 9616; }
  // the car list (car-list.toml's PanelSet; types 6 and 7: at +9752)
  template <typename T = PanelSet>
  T car_list() const { return T(base, guest::LoadU32(base, address + 9644)); }
  void set_car_list(const View& v) const { guest::StoreU32(base, address + 9644, v.address); }
  void set_car_list(uint32_t v) const { guest::StoreU32(base, address + 9644, v); }
  uint32_t car_list_address() const { return address + 9644; }
  // the group pair (types 6 and 7: at +11728)
  template <typename T = GroupPair>
  T group_pair() const { return T(base, guest::LoadU32(base, address + 9648)); }
  void set_group_pair(const View& v) const { guest::StoreU32(base, address + 9648, v.address); }
  void set_group_pair(uint32_t v) const { guest::StoreU32(base, address + 9648, v); }
  uint32_t group_pair_address() const { return address + 9648; }
  // skip the car list's set-up on show and go to state 3 once loaded
  uint32_t opens_itself() const { return guest::LoadU32(base, address + 9652); }
  void set_opens_itself(uint32_t v) const { guest::StoreU32(base, address + 9652, v); }
  uint32_t opens_itself_address() const { return address + 9652; }
  // 0 none, 1 asked, 2 yes (fade out, then state 10)
  uint32_t quit_dialog() const { return guest::LoadU32(base, address + 9656); }
  void set_quit_dialog(uint32_t v) const { guest::StoreU32(base, address + 9656, v); }
  uint32_t quit_dialog_address() const { return address + 9656; }
  // -1 none, 0 yes, 1 no, 2 back
  uint32_t dialog_answer() const { return guest::LoadU32(base, address + 9660); }
  void set_dialog_answer(uint32_t v) const { guest::StoreU32(base, address + 9660, v); }
  uint32_t dialog_answer_address() const { return address + 9660; }
  // the car list's +1920 (types 6 and 7)
  uint32_t set_unknown_1920() const { return guest::LoadU32(base, address + 11672); }
  void set_set_unknown_1920(uint32_t v) const { guest::StoreU32(base, address + 11672, v); }
  uint32_t set_unknown_1920_address() const { return address + 11672; }
  // the car list's +1928 (types 6 and 7)
  uint32_t set_unknown_1928() const { return guest::LoadU32(base, address + 11680); }
  void set_set_unknown_1928(uint32_t v) const { guest::StoreU32(base, address + 11680, v); }
  uint32_t set_unknown_1928_address() const { return address + 11680; }
};

// embedded; measure: Type6PanelScreen, Type7PanelScreen
struct Type67PanelScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 11744;
  static constexpr uint32_t kVtable = 0x82010E90;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // a ScreenMachine (boot-mode.md)
  uint32_t machine_address() const { return address + 4; }
  // the machine's state (its +9604)
  uint32_t state() const { return guest::LoadU32(base, address + 9608); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t state_address() const { return address + 9608; }
  uint32_t movie_loaded() const { return guest::LoadU32(base, address + 9612); }
  void set_movie_loaded(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t movie_loaded_address() const { return address + 9612; }
  // open the movie at the mode's start (else wait for the open message)
  uint32_t open_at_start() const { return guest::LoadU32(base, address + 9616); }
  void set_open_at_start(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t open_at_start_address() const { return address + 9616; }
  // the car list (car-list.toml's PanelSet; types 6 and 7: at +9752)
  template <typename T = PanelSet>
  T car_list() const { return T(base, guest::LoadU32(base, address + 9644)); }
  void set_car_list(const View& v) const { guest::StoreU32(base, address + 9644, v.address); }
  void set_car_list(uint32_t v) const { guest::StoreU32(base, address + 9644, v); }
  uint32_t car_list_address() const { return address + 9644; }
  // the group pair (types 6 and 7: at +11728)
  template <typename T = GroupPair>
  T group_pair() const { return T(base, guest::LoadU32(base, address + 9648)); }
  void set_group_pair(const View& v) const { guest::StoreU32(base, address + 9648, v.address); }
  void set_group_pair(uint32_t v) const { guest::StoreU32(base, address + 9648, v); }
  uint32_t group_pair_address() const { return address + 9648; }
  // skip the car list's set-up on show and go to state 3 once loaded
  uint32_t opens_itself() const { return guest::LoadU32(base, address + 9652); }
  void set_opens_itself(uint32_t v) const { guest::StoreU32(base, address + 9652, v); }
  uint32_t opens_itself_address() const { return address + 9652; }
  // 0 none, 1 asked, 2 yes (fade out, then state 10)
  uint32_t quit_dialog() const { return guest::LoadU32(base, address + 9656); }
  void set_quit_dialog(uint32_t v) const { guest::StoreU32(base, address + 9656, v); }
  uint32_t quit_dialog_address() const { return address + 9656; }
  // -1 none, 0 yes, 1 no, 2 back
  uint32_t dialog_answer() const { return guest::LoadU32(base, address + 9660); }
  void set_dialog_answer(uint32_t v) const { guest::StoreU32(base, address + 9660, v); }
  uint32_t dialog_answer_address() const { return address + 9660; }
  // PanelGroup A (UI movie 4)
  uint32_t group_a_address() const { return address + 9664; }
  // group A's vtable (0x82010E8C)
  uint32_t group_a_vtable() const { return guest::LoadU32(base, address + 9664); }
  void set_group_a_vtable(uint32_t v) const { guest::StoreU32(base, address + 9664, v); }
  uint32_t group_a_vtable_address() const { return address + 9664; }
  // PanelGroup B (UI movie 28)
  uint32_t group_b_address() const { return address + 9708; }
  // the car list (car-list.toml's PanelSet)
  uint32_t set_address() const { return address + 9752; }
  // the car list's +1896: the group pair (+11728)
  uint32_t set_group_pair() const { return guest::LoadU32(base, address + 11648); }
  void set_set_group_pair(uint32_t v) const { guest::StoreU32(base, address + 11648, v); }
  uint32_t set_group_pair_address() const { return address + 11648; }
  // the car list's +1920 (its base's +1916)
  uint32_t set_unknown_1920() const { return guest::LoadU32(base, address + 11672); }
  void set_set_unknown_1920(uint32_t v) const { guest::StoreU32(base, address + 11672, v); }
  uint32_t set_unknown_1920_address() const { return address + 11672; }
  // the car list's +1924 (its base's +1920)
  uint32_t set_unknown_1924() const { return guest::LoadU32(base, address + 11676); }
  void set_set_unknown_1924(uint32_t v) const { guest::StoreU32(base, address + 11676, v); }
  uint32_t set_unknown_1924_address() const { return address + 11676; }
  // the car list's +1928 (its base's +1924)
  uint32_t set_unknown_1928() const { return guest::LoadU32(base, address + 11680); }
  void set_set_unknown_1928(uint32_t v) const { guest::StoreU32(base, address + 11680, v); }
  uint32_t set_unknown_1928_address() const { return address + 11680; }
  // the car list's +1948 (cancelled): ask the quit dialog on the next Draw
  uint32_t ask_quit() const { return guest::LoadU32(base, address + 11700); }
  void set_ask_quit(uint32_t v) const { guest::StoreU32(base, address + 11700, v); }
  uint32_t ask_quit_address() const { return address + 11700; }
  // the car list's +1964 (its base's +1960)
  uint32_t set_unknown_1964() const { return guest::LoadU32(base, address + 11716); }
  void set_set_unknown_1964(uint32_t v) const { guest::StoreU32(base, address + 11716, v); }
  uint32_t set_unknown_1964_address() const { return address + 11716; }
  // the car list's +1968 (its base's +1964)
  uint32_t set_unknown_1968() const { return guest::LoadU32(base, address + 11720); }
  void set_set_unknown_1968(uint32_t v) const { guest::StoreU32(base, address + 11720, v); }
  uint32_t set_unknown_1968_address() const { return address + 11720; }
  // the car list's +1972 (its base's +1968)
  uint32_t set_unknown_1972() const { return guest::LoadU32(base, address + 11724); }
  void set_set_unknown_1972(uint32_t v) const { guest::StoreU32(base, address + 11724, v); }
  uint32_t set_unknown_1972_address() const { return address + 11724; }
  // the GroupPair
  uint32_t pair_address() const { return address + 11728; }
};

// image; measure: PanelScreen.+0, Type6PanelScreen.+0, Type7PanelScreen.+0
struct PanelScreenVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 76;
  uint32_t deleting_destructor() const { return guest::LoadU32(base, address + 0); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t deleting_destructor_address() const { return address + 0; }
  uint32_t slot1() const { return guest::LoadU32(base, address + 4); }
  void set_slot1(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t slot1_address() const { return address + 4; }
  uint32_t is_loaded() const { return guest::LoadU32(base, address + 8); }
  void set_is_loaded(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t is_loaded_address() const { return address + 8; }
  uint32_t pick_car() const { return guest::LoadU32(base, address + 12); }
  void set_pick_car(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t pick_car_address() const { return address + 12; }
  // probably (DrawList: the car list's slot 9)
  uint32_t back() const { return guest::LoadU32(base, address + 16); }
  void set_back(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t back_address() const { return address + 16; }
  // probably (DrawList: the car list's slot 10)
  uint32_t open_power_graph() const { return guest::LoadU32(base, address + 20); }
  void set_open_power_graph(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t open_power_graph_address() const { return address + 20; }
  // the mode's car's +1972
  uint32_t car_list_tab() const { return guest::LoadU32(base, address + 24); }
  void set_car_list_tab(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t car_list_tab_address() const { return address + 24; }
  // +1976
  uint32_t car_list_entry() const { return guest::LoadU32(base, address + 28); }
  void set_car_list_entry(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t car_list_entry_address() const { return address + 28; }
  // +1980
  uint32_t car_list_slot() const { return guest::LoadU32(base, address + 32); }
  void set_car_list_slot(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t car_list_slot_address() const { return address + 32; }
  uint32_t slot9() const { return guest::LoadU32(base, address + 36); }
  void set_slot9(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t slot9_address() const { return address + 36; }
  // after DrawList's slots
  uint32_t draw_more() const { return guest::LoadU32(base, address + 40); }
  void set_draw_more(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t draw_more_address() const { return address + 40; }
  uint32_t slot11() const { return guest::LoadU32(base, address + 44); }
  void set_slot11(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t slot11_address() const { return address + 44; }
  uint32_t slot12() const { return guest::LoadU32(base, address + 48); }
  void set_slot12(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t slot12_address() const { return address + 48; }
  uint32_t open_movie() const { return guest::LoadU32(base, address + 52); }
  void set_open_movie(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t open_movie_address() const { return address + 52; }
  uint32_t slot14() const { return guest::LoadU32(base, address + 56); }
  void set_slot14(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t slot14_address() const { return address + 56; }
  uint32_t slot15() const { return guest::LoadU32(base, address + 60); }
  void set_slot15(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t slot15_address() const { return address + 60; }
  uint32_t slot16() const { return guest::LoadU32(base, address + 64); }
  void set_slot16(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t slot16_address() const { return address + 64; }
  uint32_t slot17() const { return guest::LoadU32(base, address + 68); }
  void set_slot17(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t slot17_address() const { return address + 68; }
  // deletes the movies (from Leave)
  uint32_t release() const { return guest::LoadU32(base, address + 72); }
  void set_release(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t release_address() const { return address + 72; }
};

// image; measure: PanelScreen.+9644.+0
struct PanelSetVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 76;
  uint32_t slot0() const { return guest::LoadU32(base, address + 0); }
  void set_slot0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t slot0_address() const { return address + 0; }
  // sub_8224CAD8: its lists of unlocked and garage cars
  uint32_t build_lists() const { return guest::LoadU32(base, address + 4); }
  void set_build_lists(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t build_lists_address() const { return address + 4; }
  uint32_t on_enter() const { return guest::LoadU32(base, address + 8); }
  void set_on_enter(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t on_enter_address() const { return address + 8; }
  // sub_82249730
  uint32_t set_up() const { return guest::LoadU32(base, address + 12); }
  void set_set_up(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t set_up_address() const { return address + 12; }
  // empty
  uint32_t on_left() const { return guest::LoadU32(base, address + 16); }
  void set_on_left(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t on_left_address() const { return address + 16; }
  // sub_8224CA28
  uint32_t draw() const { return guest::LoadU32(base, address + 20); }
  void set_draw(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t draw_address() const { return address + 20; }
  // sub_8224CF18
  uint32_t update() const { return guest::LoadU32(base, address + 24); }
  void set_update(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t update_address() const { return address + 24; }
  // sub_8224B880: frees its lists
  uint32_t leave() const { return guest::LoadU32(base, address + 28); }
  void set_leave(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t leave_address() const { return address + 28; }
  // a pick (DrawList: the screen's pick_car)
  uint32_t picked() const { return guest::LoadU32(base, address + 32); }
  void set_picked(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t picked_address() const { return address + 32; }
  // probably (DrawList: the screen's back)
  uint32_t backed() const { return guest::LoadU32(base, address + 36); }
  void set_backed(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t backed_address() const { return address + 36; }
  // probably (DrawList: the screen's open_power_graph)
  uint32_t power_graph_asked() const { return guest::LoadU32(base, address + 40); }
  void set_power_graph_asked(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t power_graph_asked_address() const { return address + 40; }
  uint32_t on_message24() const { return guest::LoadU32(base, address + 44); }
  void set_on_message24(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t on_message24_address() const { return address + 44; }
  uint32_t slot12() const { return guest::LoadU32(base, address + 48); }
  void set_slot12(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t slot12_address() const { return address + 48; }
  uint32_t open() const { return guest::LoadU32(base, address + 52); }
  void set_open(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t open_address() const { return address + 52; }
  uint32_t slot14() const { return guest::LoadU32(base, address + 56); }
  void set_slot14(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t slot14_address() const { return address + 56; }
  // its +1884 from the screen's car_list_tab
  uint32_t set_tab() const { return guest::LoadU32(base, address + 60); }
  void set_set_tab(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t set_tab_address() const { return address + 60; }
  // its +1888
  uint32_t set_entry() const { return guest::LoadU32(base, address + 64); }
  void set_set_entry(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t set_entry_address() const { return address + 64; }
  // its +1892
  uint32_t set_slot() const { return guest::LoadU32(base, address + 68); }
  void set_set_slot(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t set_slot_address() const { return address + 68; }
  // the selection (PickCar reads it)
  uint32_t selection() const { return guest::LoadU32(base, address + 72); }
  void set_selection(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t selection_address() const { return address + 72; }
};

// embedded; measure: MachineScreen
struct MachineScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 9656;
  static constexpr uint32_t kVtable = 0x82011168;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // a ScreenMachine (boot-mode.md)
  uint32_t machine_address() const { return address + 4; }
  // 0 at the constructor (PanelScreen's opens_itself)
  uint32_t opens_itself() const { return guest::LoadU32(base, address + 9652); }
  void set_opens_itself(uint32_t v) const { guest::StoreU32(base, address + 9652, v); }
  uint32_t opens_itself_address() const { return address + 9652; }
};

// embedded; measure: GroupPair, PanelScreen.+9648
struct GroupPair : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr uint32_t kVtable = 0x82011084;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the current group
  template <typename T = PanelGroup>
  T current() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_current(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_current(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t current_address() const { return address + 4; }
  // the screen's +9664 (UI movie 4)
  template <typename T = PanelGroup>
  T group_a() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_group_a(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_group_a(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t group_a_address() const { return address + 8; }
  // the screen's +9708 (UI movie 28)
  template <typename T = PanelGroup>
  T group_b() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_group_b(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_group_b(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t group_b_address() const { return address + 12; }
};

// image; measure: PanelScreen.+9648.+0
struct GroupPairVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  uint32_t deleting_destructor() const { return guest::LoadU32(base, address + 0); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t deleting_destructor_address() const { return address + 0; }
  uint32_t run() const { return guest::LoadU32(base, address + 4); }
  void set_run(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t run_address() const { return address + 4; }
  uint32_t wait_for_fade() const { return guest::LoadU32(base, address + 8); }
  void set_wait_for_fade(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t wait_for_fade_address() const { return address + 8; }
  uint32_t select_a() const { return guest::LoadU32(base, address + 12); }
  void set_select_a(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t select_a_address() const { return address + 12; }
  // empty
  uint32_t on_left() const { return guest::LoadU32(base, address + 16); }
  void set_on_left(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t on_left_address() const { return address + 16; }
  uint32_t reset() const { return guest::LoadU32(base, address + 20); }
  void set_reset(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t reset_address() const { return address + 20; }
  uint32_t slot6() const { return guest::LoadU32(base, address + 24); }
  void set_slot6(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t slot6_address() const { return address + 24; }
  uint32_t finish_all() const { return guest::LoadU32(base, address + 28); }
  void set_finish_all(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t finish_all_address() const { return address + 28; }
};

// embedded; measure: PanelGroup
struct PanelGroup : View {
  using View::View;
  static constexpr uint32_t kSize = 44;
  static constexpr uint32_t kVtable = 0x82010E8C;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // its UI movie's id (4 group A, 28 group B)
  uint32_t movie() const { return guest::LoadU32(base, address + 4); }
  void set_movie(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t movie_address() const { return address + 4; }
  // PanelGroupBase's
  uint32_t caption_node_set() const { return guest::LoadU32(base, address + 12); }
  void set_caption_node_set(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t caption_node_set_address() const { return address + 12; }
  uint32_t unknown_32() const { return guest::LoadU32(base, address + 32); }
  void set_unknown_32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
};

// embedded; measure: SingleGroup
struct SingleGroup : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  static constexpr uint32_t kVtable = 0x820110A4;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // its group
  template <typename T = PanelGroup>
  T group() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_group(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_group(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t group_address() const { return address + 4; }
};

// embedded; measure: ListPart, ListPartBase, ColorListScreen.+9676, TitledListScreen.+9676
struct ListPart : View {
  using View::View;
  static constexpr uint32_t kSize = 140;
  static constexpr uint32_t kVtable = 0x82011314;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the inner's yes after a pick
  uint32_t yes() const { return guest::LoadU32(base, address + 4); }
  void set_yes(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t yes_address() const { return address + 4; }
  // the inner's back after a pick
  uint32_t back() const { return guest::LoadU32(base, address + 8); }
  void set_back(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t back_address() const { return address + 8; }
  // the UI movie's id the inner is given
  uint32_t movie() const { return guest::LoadU32(base, address + 12); }
  void set_movie(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t movie_address() const { return address + 12; }
  // the inner part (+20; 0 none)
  template <typename T = ListPartInner>
  T inner() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_inner(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_inner(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t inner_address() const { return address + 16; }
  // the ListPartInner
  uint32_t inner_storage_address() const { return address + 20; }
};

// image; measure: ListPartBase.+0, ColorListScreen.+9676.+0, TitledListScreen.+9676.+0
struct ListPartVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 44;
  uint32_t deleting_destructor() const { return guest::LoadU32(base, address + 0); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t deleting_destructor_address() const { return address + 0; }
  uint32_t open() const { return guest::LoadU32(base, address + 4); }
  void set_open(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t open_address() const { return address + 4; }
  uint32_t pick() const { return guest::LoadU32(base, address + 8); }
  void set_pick(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t pick_address() const { return address + 8; }
  uint32_t input() const { return guest::LoadU32(base, address + 12); }
  void set_input(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t input_address() const { return address + 12; }
  uint32_t close() const { return guest::LoadU32(base, address + 16); }
  void set_close(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t close_address() const { return address + 16; }
  uint32_t is_idle() const { return guest::LoadU32(base, address + 20); }
  void set_is_idle(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t is_idle_address() const { return address + 20; }
  // sub_825C8390
  uint32_t slot6() const { return guest::LoadU32(base, address + 24); }
  void set_slot6(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t slot6_address() const { return address + 24; }
  // sub_8244A528
  uint32_t slot7() const { return guest::LoadU32(base, address + 28); }
  void set_slot7(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t slot7_address() const { return address + 28; }
  uint32_t can_leave() const { return guest::LoadU32(base, address + 32); }
  void set_can_leave(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t can_leave_address() const { return address + 32; }
  // empty
  uint32_t on_open() const { return guest::LoadU32(base, address + 36); }
  void set_on_open(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t on_open_address() const { return address + 36; }
  // empty
  uint32_t on_close() const { return guest::LoadU32(base, address + 40); }
  void set_on_close(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t on_close_address() const { return address + 40; }
};

// embedded; measure: ListPartInner, ListPartInnerBase, ListPartBase.+16
struct ListPartInner : View {
  using View::View;
  static constexpr uint32_t kSize = 120;
  static constexpr uint32_t kVtable = 0x820112F8;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the page Draw runs (-1 while a step runs)
  uint32_t page() const { return guest::LoadU32(base, address + 4); }
  void set_page(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t page_address() const { return address + 4; }
  // the page after the step
  uint32_t next_page() const { return guest::LoadU32(base, address + 8); }
  void set_next_page(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t next_page_address() const { return address + 8; }
  uint32_t previous_page() const { return guest::LoadU32(base, address + 12); }
  void set_previous_page(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t previous_page_address() const { return address + 12; }
  // the step Update runs (-1 none)
  uint32_t step() const { return guest::LoadU32(base, address + 16); }
  void set_step(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t step_address() const { return address + 16; }
  // the step after it (-1 none)
  uint32_t queued_step() const { return guest::LoadU32(base, address + 20); }
  void set_queued_step(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t queued_step_address() const { return address + 20; }
  // the page block's +20: 0 when a queued step starts
  uint32_t unknown_24() const { return guest::LoadU32(base, address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  // set by a step once its first call ran; 0 when the block moves on
  uint32_t step_started() const { return guest::LoadU32(base, address + 28); }
  void set_step_started(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t step_started_address() const { return address + 28; }
  // the answer: yes (copied to the part's +4)
  uint32_t yes() const { return guest::LoadU32(base, address + 32); }
  void set_yes(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t yes_address() const { return address + 32; }
  // the answer: back (copied to the part's +8)
  uint32_t back() const { return guest::LoadU32(base, address + 36); }
  void set_back(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t back_address() const { return address + 36; }
  // the two-item prompt (a MenuList, race-screens.toml)
  uint32_t menu_address() const { return address + 40; }
  // the list's +24 (move)
  uint32_t menu_move() const { return guest::LoadU32(base, address + 64); }
  void set_menu_move(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t menu_move_address() const { return address + 64; }
  // the list's +60 (player)
  uint32_t menu_player() const { return guest::LoadU32(base, address + 100); }
  void set_menu_player(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t menu_player_address() const { return address + 100; }
  // the list's +68 (enabled)
  uint32_t menu_enabled() const { return guest::LoadU32(base, address + 108); }
  void set_menu_enabled(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t menu_enabled_address() const { return address + 108; }
  // 0 opening, 1 shown, 2 closing, 3 done, 4 asked to leave
  uint32_t prompt_state() const { return guest::LoadU32(base, address + 112); }
  void set_prompt_state(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t prompt_state_address() const { return address + 112; }
  uint32_t leave_asked() const { return guest::LoadU32(base, address + 116); }
  void set_leave_asked(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t leave_asked_address() const { return address + 116; }
};

// image; measure: ListPartBase.+16.+0
struct ListPartInnerVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  uint32_t deleting_destructor() const { return guest::LoadU32(base, address + 0); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t deleting_destructor_address() const { return address + 0; }
  uint32_t open() const { return guest::LoadU32(base, address + 4); }
  void set_open(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t open_address() const { return address + 4; }
  uint32_t pick() const { return guest::LoadU32(base, address + 8); }
  void set_pick(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t pick_address() const { return address + 8; }
  uint32_t input() const { return guest::LoadU32(base, address + 12); }
  void set_input(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t input_address() const { return address + 12; }
  uint32_t is_idle() const { return guest::LoadU32(base, address + 16); }
  void set_is_idle(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t is_idle_address() const { return address + 16; }
  uint32_t close() const { return guest::LoadU32(base, address + 20); }
  void set_close(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t close_address() const { return address + 20; }
  uint32_t can_leave() const { return guest::LoadU32(base, address + 24); }
  void set_can_leave(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t can_leave_address() const { return address + 24; }
  uint32_t part_deleting_destructor() const { return guest::LoadU32(base, address + 28); }
  void set_part_deleting_destructor(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t part_deleting_destructor_address() const { return address + 28; }
};

// embedded; measure: NodeTable
struct NodeTable : View {
  using View::View;
  static constexpr uint32_t kSize = 584;
  static constexpr uint32_t kVtable = 0x82010FF0;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the UI movie's id
  uint32_t movie() const { return guest::LoadU32(base, address + 4); }
  void set_movie(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t movie_address() const { return address + 4; }
  // the sub-scene of the extra nodes (+420, +424, +264)
  uint32_t node_set_a() const { return guest::LoadU32(base, address + 8); }
  void set_node_set_a(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t node_set_a_address() const { return address + 8; }
  // the sub-scene of the items' nodes
  uint32_t node_set_b() const { return guest::LoadU32(base, address + 12); }
  void set_node_set_b(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t node_set_b_address() const { return address + 12; }
  // 27 items' named animations
  uint32_t item_anims_address() const { return address + 16; }
  // eight more (the third pair's, +444)
  uint32_t more_anims_address() const { return address + 124; }
  // 27 items' nodes
  uint32_t item_nodes_address() const { return address + 156; }
  // eight nodes (hidden at the reset)
  uint32_t nodes_address() const { return address + 264; }
  // the four children's indexes (marks and colors)
  uint32_t children_address() const { return address + 296; }
  // 27 items' masks of the four children
  uint32_t item_masks_address() const { return address + 312; }
  // the item nodes' twins on node_set_a (-1 none)
  uint32_t extra_node_base() const { return guest::LoadU32(base, address + 420); }
  void set_extra_node_base(uint32_t v) const { guest::StoreU32(base, address + 420, v); }
  uint32_t extra_node_base_address() const { return address + 420; }
  // four text nodes (strings 402, 401, 407, 408; -1 none)
  uint32_t text_node_base() const { return guest::LoadU32(base, address + 424); }
  void set_text_node_base(uint32_t v) const { guest::StoreU32(base, address + 424, v); }
  uint32_t text_node_base_address() const { return address + 424; }
  // the first pair: the item now
  uint32_t item() const { return guest::LoadU32(base, address + 428); }
  void set_item(uint32_t v) const { guest::StoreU32(base, address + 428, v); }
  uint32_t item_address() const { return address + 428; }
  // the item before
  uint32_t item_before() const { return guest::LoadU32(base, address + 432); }
  void set_item_before(uint32_t v) const { guest::StoreU32(base, address + 432, v); }
  uint32_t item_before_address() const { return address + 432; }
  // the second pair: an item now
  uint32_t second() const { return guest::LoadU32(base, address + 436); }
  void set_second(uint32_t v) const { guest::StoreU32(base, address + 436, v); }
  uint32_t second_address() const { return address + 436; }
  uint32_t second_before() const { return guest::LoadU32(base, address + 440); }
  void set_second_before(uint32_t v) const { guest::StoreU32(base, address + 440, v); }
  uint32_t second_before_address() const { return address + 440; }
  // the third pair: an index into more_anims now
  uint32_t third() const { return guest::LoadU32(base, address + 444); }
  void set_third(uint32_t v) const { guest::StoreU32(base, address + 444, v); }
  uint32_t third_address() const { return address + 444; }
  uint32_t third_before() const { return guest::LoadU32(base, address + 448); }
  void set_third_before(uint32_t v) const { guest::StoreU32(base, address + 448, v); }
  uint32_t third_before_address() const { return address + 448; }
  // the fourth pair: the paired table's mask bit; the color table's grid column
  uint32_t fourth() const { return guest::LoadU32(base, address + 452); }
  void set_fourth(uint32_t v) const { guest::StoreU32(base, address + 452, v); }
  uint32_t fourth_address() const { return address + 452; }
  uint32_t fourth_before() const { return guest::LoadU32(base, address + 456); }
  void set_fourth_before(uint32_t v) const { guest::StoreU32(base, address + 456, v); }
  uint32_t fourth_before_address() const { return address + 456; }
  // a mark bit (MarkSelected 0)
  uint32_t mark_a() const { return guest::LoadU32(base, address + 460); }
  void set_mark_a(uint32_t v) const { guest::StoreU32(base, address + 460, v); }
  uint32_t mark_a_address() const { return address + 460; }
  uint32_t mark_a_before() const { return guest::LoadU32(base, address + 464); }
  void set_mark_a_before(uint32_t v) const { guest::StoreU32(base, address + 464, v); }
  uint32_t mark_a_before_address() const { return address + 464; }
  // a mark bit (MarkSelected 1)
  uint32_t mark_b() const { return guest::LoadU32(base, address + 468); }
  void set_mark_b(uint32_t v) const { guest::StoreU32(base, address + 468, v); }
  uint32_t mark_b_address() const { return address + 468; }
  uint32_t mark_b_before() const { return guest::LoadU32(base, address + 472); }
  void set_mark_b_before(uint32_t v) const { guest::StoreU32(base, address + 472, v); }
  uint32_t mark_b_before_address() const { return address + 472; }
  // the child given color_a (-1 none)
  uint32_t color_child() const { return guest::LoadU32(base, address + 476); }
  void set_color_child(uint32_t v) const { guest::StoreU32(base, address + 476, v); }
  uint32_t color_child_address() const { return address + 476; }
  // 3 x 3 named animations by (before, now) of the fourth pair
  uint32_t grid_a_address() const { return address + 480; }
  // the same, played after grid_a's
  uint32_t grid_b_address() const { return address + 516; }
  // the color child's color (three words copied)
  uint32_t color_a_address() const { return address + 552; }
  // the other children's color (three words copied)
  uint32_t color_b_address() const { return address + 568; }
};

// embedded; measure: PairedNodeTable, sub_82247FE0.r3
struct PairedNodeTable : View {
  using View::View;
  static constexpr uint32_t kSize = 460;
  static constexpr uint32_t kVtable = 0x82010FF8;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the UI movie's id
  uint32_t movie() const { return guest::LoadU32(base, address + 4); }
  void set_movie(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t movie_address() const { return address + 4; }
  // the sub-scene of the extra nodes (+420, +424, +264)
  uint32_t node_set_a() const { return guest::LoadU32(base, address + 8); }
  void set_node_set_a(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t node_set_a_address() const { return address + 8; }
  // the sub-scene of the items' nodes
  uint32_t node_set_b() const { return guest::LoadU32(base, address + 12); }
  void set_node_set_b(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t node_set_b_address() const { return address + 12; }
  // 27 items' named animations
  uint32_t item_anims_address() const { return address + 16; }
  // eight more (the third pair's, +444)
  uint32_t more_anims_address() const { return address + 124; }
  // 27 items' nodes
  uint32_t item_nodes_address() const { return address + 156; }
  // eight nodes (hidden at the reset)
  uint32_t nodes_address() const { return address + 264; }
  // the four children's indexes (marks and colors)
  uint32_t children_address() const { return address + 296; }
  // 27 items' masks of the four children
  uint32_t item_masks_address() const { return address + 312; }
  // the item nodes' twins on node_set_a (-1 none)
  uint32_t extra_node_base() const { return guest::LoadU32(base, address + 420); }
  void set_extra_node_base(uint32_t v) const { guest::StoreU32(base, address + 420, v); }
  uint32_t extra_node_base_address() const { return address + 420; }
  // four text nodes (strings 402, 401, 407, 408; -1 none)
  uint32_t text_node_base() const { return guest::LoadU32(base, address + 424); }
  void set_text_node_base(uint32_t v) const { guest::StoreU32(base, address + 424, v); }
  uint32_t text_node_base_address() const { return address + 424; }
  // the first pair: the item now
  uint32_t item() const { return guest::LoadU32(base, address + 428); }
  void set_item(uint32_t v) const { guest::StoreU32(base, address + 428, v); }
  uint32_t item_address() const { return address + 428; }
  // the item before
  uint32_t item_before() const { return guest::LoadU32(base, address + 432); }
  void set_item_before(uint32_t v) const { guest::StoreU32(base, address + 432, v); }
  uint32_t item_before_address() const { return address + 432; }
  // the second pair: an item now
  uint32_t second() const { return guest::LoadU32(base, address + 436); }
  void set_second(uint32_t v) const { guest::StoreU32(base, address + 436, v); }
  uint32_t second_address() const { return address + 436; }
  uint32_t second_before() const { return guest::LoadU32(base, address + 440); }
  void set_second_before(uint32_t v) const { guest::StoreU32(base, address + 440, v); }
  uint32_t second_before_address() const { return address + 440; }
  // the third pair: an index into more_anims now
  uint32_t third() const { return guest::LoadU32(base, address + 444); }
  void set_third(uint32_t v) const { guest::StoreU32(base, address + 444, v); }
  uint32_t third_address() const { return address + 444; }
  uint32_t third_before() const { return guest::LoadU32(base, address + 448); }
  void set_third_before(uint32_t v) const { guest::StoreU32(base, address + 448, v); }
  uint32_t third_before_address() const { return address + 448; }
  // the fourth pair: the paired table's mask bit; the color table's grid column
  uint32_t fourth() const { return guest::LoadU32(base, address + 452); }
  void set_fourth(uint32_t v) const { guest::StoreU32(base, address + 452, v); }
  uint32_t fourth_address() const { return address + 452; }
  uint32_t fourth_before() const { return guest::LoadU32(base, address + 456); }
  void set_fourth_before(uint32_t v) const { guest::StoreU32(base, address + 456, v); }
  uint32_t fourth_before_address() const { return address + 456; }
};

// embedded; measure: ColorNodeTable, ColorListScreen.+10432, NodeTable::SetMarks.ret
struct ColorNodeTable : View {
  using View::View;
  static constexpr uint32_t kSize = 584;
  static constexpr uint32_t kVtable = 0x82011000;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the UI movie's id
  uint32_t movie() const { return guest::LoadU32(base, address + 4); }
  void set_movie(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t movie_address() const { return address + 4; }
  // the sub-scene of the extra nodes (+420, +424, +264)
  uint32_t node_set_a() const { return guest::LoadU32(base, address + 8); }
  void set_node_set_a(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t node_set_a_address() const { return address + 8; }
  // the sub-scene of the items' nodes
  uint32_t node_set_b() const { return guest::LoadU32(base, address + 12); }
  void set_node_set_b(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t node_set_b_address() const { return address + 12; }
  // 27 items' named animations
  uint32_t item_anims_address() const { return address + 16; }
  // eight more (the third pair's, +444)
  uint32_t more_anims_address() const { return address + 124; }
  // 27 items' nodes
  uint32_t item_nodes_address() const { return address + 156; }
  // eight nodes (hidden at the reset)
  uint32_t nodes_address() const { return address + 264; }
  // the four children's indexes (marks and colors)
  uint32_t children_address() const { return address + 296; }
  // 27 items' masks of the four children
  uint32_t item_masks_address() const { return address + 312; }
  // the item nodes' twins on node_set_a (-1 none)
  uint32_t extra_node_base() const { return guest::LoadU32(base, address + 420); }
  void set_extra_node_base(uint32_t v) const { guest::StoreU32(base, address + 420, v); }
  uint32_t extra_node_base_address() const { return address + 420; }
  // four text nodes (strings 402, 401, 407, 408; -1 none)
  uint32_t text_node_base() const { return guest::LoadU32(base, address + 424); }
  void set_text_node_base(uint32_t v) const { guest::StoreU32(base, address + 424, v); }
  uint32_t text_node_base_address() const { return address + 424; }
  // the first pair: the item now
  uint32_t item() const { return guest::LoadU32(base, address + 428); }
  void set_item(uint32_t v) const { guest::StoreU32(base, address + 428, v); }
  uint32_t item_address() const { return address + 428; }
  // the item before
  uint32_t item_before() const { return guest::LoadU32(base, address + 432); }
  void set_item_before(uint32_t v) const { guest::StoreU32(base, address + 432, v); }
  uint32_t item_before_address() const { return address + 432; }
  // the second pair: an item now
  uint32_t second() const { return guest::LoadU32(base, address + 436); }
  void set_second(uint32_t v) const { guest::StoreU32(base, address + 436, v); }
  uint32_t second_address() const { return address + 436; }
  uint32_t second_before() const { return guest::LoadU32(base, address + 440); }
  void set_second_before(uint32_t v) const { guest::StoreU32(base, address + 440, v); }
  uint32_t second_before_address() const { return address + 440; }
  // the third pair: an index into more_anims now
  uint32_t third() const { return guest::LoadU32(base, address + 444); }
  void set_third(uint32_t v) const { guest::StoreU32(base, address + 444, v); }
  uint32_t third_address() const { return address + 444; }
  uint32_t third_before() const { return guest::LoadU32(base, address + 448); }
  void set_third_before(uint32_t v) const { guest::StoreU32(base, address + 448, v); }
  uint32_t third_before_address() const { return address + 448; }
  // the fourth pair: the paired table's mask bit; the color table's grid column
  uint32_t fourth() const { return guest::LoadU32(base, address + 452); }
  void set_fourth(uint32_t v) const { guest::StoreU32(base, address + 452, v); }
  uint32_t fourth_address() const { return address + 452; }
  uint32_t fourth_before() const { return guest::LoadU32(base, address + 456); }
  void set_fourth_before(uint32_t v) const { guest::StoreU32(base, address + 456, v); }
  uint32_t fourth_before_address() const { return address + 456; }
  // a mark bit (MarkSelected 0)
  uint32_t mark_a() const { return guest::LoadU32(base, address + 460); }
  void set_mark_a(uint32_t v) const { guest::StoreU32(base, address + 460, v); }
  uint32_t mark_a_address() const { return address + 460; }
  uint32_t mark_a_before() const { return guest::LoadU32(base, address + 464); }
  void set_mark_a_before(uint32_t v) const { guest::StoreU32(base, address + 464, v); }
  uint32_t mark_a_before_address() const { return address + 464; }
  // a mark bit (MarkSelected 1)
  uint32_t mark_b() const { return guest::LoadU32(base, address + 468); }
  void set_mark_b(uint32_t v) const { guest::StoreU32(base, address + 468, v); }
  uint32_t mark_b_address() const { return address + 468; }
  uint32_t mark_b_before() const { return guest::LoadU32(base, address + 472); }
  void set_mark_b_before(uint32_t v) const { guest::StoreU32(base, address + 472, v); }
  uint32_t mark_b_before_address() const { return address + 472; }
  // the child given color_a (-1 none)
  uint32_t color_child() const { return guest::LoadU32(base, address + 476); }
  void set_color_child(uint32_t v) const { guest::StoreU32(base, address + 476, v); }
  uint32_t color_child_address() const { return address + 476; }
  // 3 x 3 named animations by (before, now) of the fourth pair
  uint32_t grid_a_address() const { return address + 480; }
  // the same, played after grid_a's
  uint32_t grid_b_address() const { return address + 516; }
  // the color child's color (three words copied)
  uint32_t color_a_address() const { return address + 552; }
  // the other children's color (three words copied)
  uint32_t color_b_address() const { return address + 568; }
};

// embedded; measure: NodeTable::SetPairs.r4, NodeScreen::ItemInfo.r5, Type6ListScreen::ItemInfo.r5, NodeListScreen::ItemInfo.r5
struct Type6ItemInfo : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // the node table's third (+444)
  uint32_t third() const { return guest::LoadU32(base, address + 0); }
  void set_third(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t third_address() const { return address + 0; }
  // the node table's item (+428)
  uint32_t item() const { return guest::LoadU32(base, address + 4); }
  void set_item(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t item_address() const { return address + 4; }
  // the node table's second (+436)
  uint32_t second() const { return guest::LoadU32(base, address + 8); }
  void set_second(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t second_address() const { return address + 8; }
  // the node table's fourth (+452); bits 0 and 1 the screen's item_bit_0 and _1
  uint32_t fourth() const { return guest::LoadU32(base, address + 12); }
  void set_fourth(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t fourth_address() const { return address + 12; }
};

// embedded; measure: ColorNodeTable::Set.r4
struct Type7ItemInfo : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // the table's item (+428)
  uint32_t item() const { return guest::LoadU32(base, address + 0); }
  void set_item(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t item_address() const { return address + 0; }
  // the table's fourth (+452)
  uint32_t grid_column() const { return guest::LoadU32(base, address + 4); }
  void set_grid_column(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t grid_column_address() const { return address + 4; }
  // the color list's count; 1: the table's third -1
  uint32_t color_count() const { return guest::LoadU32(base, address + 8); }
  void set_color_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t color_count_address() const { return address + 8; }
  // the table's second (+436); an index into the detail texts at 0x82637078
  uint32_t detail_a() const { return guest::LoadU32(base, address + 12); }
  void set_detail_a(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t detail_a_address() const { return address + 12; }
  // the table's third (+444) unless color_count is 1
  uint32_t detail_b() const { return guest::LoadU32(base, address + 16); }
  void set_detail_b(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t detail_b_address() const { return address + 16; }
  // the table's mark_a (+460); the color child for color 0
  uint32_t bit_a() const { return guest::LoadU32(base, address + 20); }
  void set_bit_a(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t bit_a_address() const { return address + 20; }
  // the table's mark_b (+468); for color 1
  uint32_t bit_b() const { return guest::LoadU32(base, address + 24); }
  void set_bit_b(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t bit_b_address() const { return address + 24; }
};

// allocated; measure: Type6Screens
struct Type6Screens : View {
  using View::View;
  static constexpr uint32_t kSize = 91272;
  static constexpr uint32_t kVtable = 0x82010A78;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the machine at +4's +9612 (1 at Init)
  uint32_t screen0_open_at_start() const { return guest::LoadU32(base, address + 9616); }
  void set_screen0_open_at_start(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t screen0_open_at_start_address() const { return address + 9616; }
  // the machine at +4's +9616 (0 at Init)
  uint32_t screen0_unknown_9616() const { return guest::LoadU32(base, address + 9620); }
  void set_screen0_unknown_9616(uint32_t v) const { guest::StoreU32(base, address + 9620, v); }
  uint32_t screen0_unknown_9616_address() const { return address + 9620; }
  // the machine at +9672's +9612 (1 at Init)
  uint32_t machine_9672_open_at_start() const { return guest::LoadU32(base, address + 19284); }
  void set_machine_9672_open_at_start(uint32_t v) const { guest::StoreU32(base, address + 19284, v); }
  uint32_t machine_9672_open_at_start_address() const { return address + 19284; }
  // the machine at +9672's +9616 (0 at Init)
  uint32_t machine_9672_unknown_9616() const { return guest::LoadU32(base, address + 19288); }
  void set_machine_9672_unknown_9616(uint32_t v) const { guest::StoreU32(base, address + 19288, v); }
  uint32_t machine_9672_unknown_9616_address() const { return address + 19288; }
  // the Type6ListScreen
  uint32_t list_screen_address() const { return address + 19432; }
  // the node screen
  uint32_t node_screen_address() const { return address + 30396; }
  // the Type67PanelScreen
  uint32_t panel_screen_address() const { return address + 40948; }
  // the power graph screen's vtable (0x82010A68; 0x82010A2C its base's)
  uint32_t power_graph_vtable() const { return guest::LoadU32(base, address + 52692); }
  void set_power_graph_vtable(uint32_t v) const { guest::StoreU32(base, address + 52692, v); }
  uint32_t power_graph_vtable_address() const { return address + 52692; }
  // the machine at +62336's +9612 (1 at Init)
  uint32_t machine_62336_open_at_start() const { return guest::LoadU32(base, address + 71948); }
  void set_machine_62336_open_at_start(uint32_t v) const { guest::StoreU32(base, address + 71948, v); }
  uint32_t machine_62336_open_at_start_address() const { return address + 71948; }
  // the machine at +62336's +9616 (0 at Init)
  uint32_t machine_62336_unknown_9616() const { return guest::LoadU32(base, address + 71952); }
  void set_machine_62336_unknown_9616(uint32_t v) const { guest::StoreU32(base, address + 71952, v); }
  uint32_t machine_62336_unknown_9616_address() const { return address + 71952; }
  // the machine at +71976's +9612 (1 at Init)
  uint32_t machine_71976_open_at_start() const { return guest::LoadU32(base, address + 81588); }
  void set_machine_71976_open_at_start(uint32_t v) const { guest::StoreU32(base, address + 81588, v); }
  uint32_t machine_71976_open_at_start_address() const { return address + 81588; }
  // the machine at +71976's +9616 (0 at Init)
  uint32_t machine_71976_unknown_9616() const { return guest::LoadU32(base, address + 81592); }
  void set_machine_71976_unknown_9616(uint32_t v) const { guest::StoreU32(base, address + 81592, v); }
  uint32_t machine_71976_unknown_9616_address() const { return address + 81592; }
  // the machine at +81616's +9612 (1 at Init)
  uint32_t machine_81616_open_at_start() const { return guest::LoadU32(base, address + 91228); }
  void set_machine_81616_open_at_start(uint32_t v) const { guest::StoreU32(base, address + 91228, v); }
  uint32_t machine_81616_open_at_start_address() const { return address + 91228; }
  // the machine at +81616's +9616 (0 at Init)
  uint32_t machine_81616_unknown_9616() const { return guest::LoadU32(base, address + 91232); }
  void set_machine_81616_unknown_9616(uint32_t v) const { guest::StoreU32(base, address + 91232, v); }
  uint32_t machine_81616_unknown_9616_address() const { return address + 91232; }
  // the block's 8-byte answer: its first byte
  uint8_t answer() const { return guest::LoadU8(base, address + 91256); }
  void set_answer(uint8_t v) const { guest::StoreU8(base, address + 91256, v); }
  uint32_t answer_address() const { return address + 91256; }
  // the answer's word at +4
  uint32_t answer_word() const { return guest::LoadU32(base, address + 91260); }
  void set_answer_word(uint32_t v) const { guest::StoreU32(base, address + 91260, v); }
  uint32_t answer_word_address() const { return address + 91260; }
  // 4 leaving, 5 torn down
  uint32_t phase() const { return guest::LoadU32(base, address + 91264); }
  void set_phase(uint32_t v) const { guest::StoreU32(base, address + 91264, v); }
  uint32_t phase_address() const { return address + 91264; }
};

// allocated; measure: Type7Screens
struct Type7Screens : View {
  using View::View;
  static constexpr uint32_t kSize = 71292;
  static constexpr uint32_t kVtable = 0x82010FE0;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the machine at +9664's +9612 (1 at Init)
  uint32_t machine_9664_open_at_start() const { return guest::LoadU32(base, address + 19276); }
  void set_machine_9664_open_at_start(uint32_t v) const { guest::StoreU32(base, address + 19276, v); }
  uint32_t machine_9664_open_at_start_address() const { return address + 19276; }
  // the machine at +9664's +9616 (0 at Init)
  uint32_t machine_9664_unknown_9616() const { return guest::LoadU32(base, address + 19280); }
  void set_machine_9664_unknown_9616(uint32_t v) const { guest::StoreU32(base, address + 19280, v); }
  uint32_t machine_9664_unknown_9616_address() const { return address + 19280; }
  // the Type7ListScreen
  uint32_t list_screen_address() const { return address + 19428; }
  // the Type67PanelScreen
  uint32_t panel_screen_address() const { return address + 30608; }
  // the power graph screen's vtable (0x82010FD0; 0x82010A2C its base's)
  uint32_t power_graph_vtable() const { return guest::LoadU32(base, address + 42352); }
  void set_power_graph_vtable(uint32_t v) const { guest::StoreU32(base, address + 42352, v); }
  uint32_t power_graph_vtable_address() const { return address + 42352; }
  // the machine at +51996's +9612 (1 at Init)
  uint32_t machine_51996_open_at_start() const { return guest::LoadU32(base, address + 61608); }
  void set_machine_51996_open_at_start(uint32_t v) const { guest::StoreU32(base, address + 61608, v); }
  uint32_t machine_51996_open_at_start_address() const { return address + 61608; }
  // the machine at +51996's +9616 (0 at Init)
  uint32_t machine_51996_unknown_9616() const { return guest::LoadU32(base, address + 61612); }
  void set_machine_51996_unknown_9616(uint32_t v) const { guest::StoreU32(base, address + 61612, v); }
  uint32_t machine_51996_unknown_9616_address() const { return address + 61612; }
  // the machine at +61636's +9612 (1 at Init)
  uint32_t machine_61636_open_at_start() const { return guest::LoadU32(base, address + 71248); }
  void set_machine_61636_open_at_start(uint32_t v) const { guest::StoreU32(base, address + 71248, v); }
  uint32_t machine_61636_open_at_start_address() const { return address + 71248; }
  // the machine at +61636's +9616 (0 at Init)
  uint32_t machine_61636_unknown_9616() const { return guest::LoadU32(base, address + 71252); }
  void set_machine_61636_unknown_9616(uint32_t v) const { guest::StoreU32(base, address + 71252, v); }
  uint32_t machine_61636_unknown_9616_address() const { return address + 71252; }
  // the block's 8-byte answer: its first byte
  uint8_t answer() const { return guest::LoadU8(base, address + 71276); }
  void set_answer(uint8_t v) const { guest::StoreU8(base, address + 71276, v); }
  uint32_t answer_address() const { return address + 71276; }
  // the answer's word at +4
  uint32_t answer_word() const { return guest::LoadU32(base, address + 71280); }
  void set_answer_word(uint32_t v) const { guest::StoreU32(base, address + 71280, v); }
  uint32_t answer_word_address() const { return address + 71280; }
  // 4 leaving, 5 torn down
  uint32_t phase() const { return guest::LoadU32(base, address + 71284); }
  void set_phase(uint32_t v) const { guest::StoreU32(base, address + 71284, v); }
  uint32_t phase_address() const { return address + 71284; }
};

// The image words docs/functions/type67-screens.md's code uses.
struct Type67ScreensGlobals {
  // the screen fades' fade-out count (0 once done)
  static uint32_t fade_out_count(uint8_t* base) { return guest::LoadU32(base, 0x826A50E4u); }
  static void set_fade_out_count(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50E4u, v); }
  static constexpr uint32_t fade_out_count_address() { return 0x826A50E4u; }
  // the screen fades' fade-in count (0 once done)
  static uint32_t fade_in_count(uint8_t* base) { return guest::LoadU32(base, 0x826A50DCu); }
  static void set_fade_in_count(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50DCu, v); }
  static constexpr uint32_t fade_in_count_address() { return 0x826A50DCu; }
  // the passes word ScreenFades::FadeIn is given
  static uint32_t fade_in_passes(uint8_t* base) { return guest::LoadU32(base, 0x82001BECu); }
  static void set_fade_in_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BECu, v); }
  static constexpr uint32_t fade_in_passes_address() { return 0x82001BECu; }
  // the passes word ScreenFades::FadeOut is given
  static uint32_t fade_out_passes(uint8_t* base) { return guest::LoadU32(base, 0x82001BF0u); }
  static void set_fade_out_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BF0u, v); }
  static constexpr uint32_t fade_out_passes_address() { return 0x82001BF0u; }
  // set by PanelFade::In, cleared by GroupPair::WaitForFade
  static uint32_t fade_wait(uint8_t* base) { return guest::LoadU32(base, 0x82815EB0u); }
  static void set_fade_wait(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82815EB0u, v); }
  static constexpr uint32_t fade_wait_address() { return 0x82815EB0u; }
  // the titled list's page table (9 member function pointers)
  static constexpr uint32_t titled_pages_address() { return 0x82815F88u; }
  // bit 0 once the table is built
  static uint32_t titled_page_guard(uint8_t* base) { return guest::LoadU32(base, 0x82815FD0u); }
  static void set_titled_page_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82815FD0u, v); }
  static constexpr uint32_t titled_page_guard_address() { return 0x82815FD0u; }
  // the titled list's step table (3 entries)
  static constexpr uint32_t titled_steps_address() { return 0x82815FD4u; }
  // bit 0 once the table is built
  static uint32_t titled_step_guard(uint8_t* base) { return guest::LoadU32(base, 0x82815FECu); }
  static void set_titled_step_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82815FECu, v); }
  static constexpr uint32_t titled_step_guard_address() { return 0x82815FECu; }
  // the color list's page table (11 entries)
  static constexpr uint32_t color_pages_address() { return 0x82815EC8u; }
  // bit 0 once the table is built
  static uint32_t color_page_guard(uint8_t* base) { return guest::LoadU32(base, 0x82815F20u); }
  static void set_color_page_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82815F20u, v); }
  static constexpr uint32_t color_page_guard_address() { return 0x82815F20u; }
  // the color list's step table (11 entries)
  static constexpr uint32_t color_steps_address() { return 0x82815F28u; }
  // bit 0 once the table is built
  static uint32_t color_step_guard(uint8_t* base) { return guest::LoadU32(base, 0x82815F80u); }
  static void set_color_step_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82815F80u, v); }
  static constexpr uint32_t color_step_guard_address() { return 0x82815F80u; }
  // the node screen's page table (6 entries)
  static constexpr uint32_t node_pages_address() { return 0x82816078u; }
  // bit 0 once the table is built
  static uint32_t node_page_guard(uint8_t* base) { return guest::LoadU32(base, 0x828160A8u); }
  static void set_node_page_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828160A8u, v); }
  static constexpr uint32_t node_page_guard_address() { return 0x828160A8u; }
  // the node screen's step table (3 entries)
  static constexpr uint32_t node_steps_address() { return 0x828160ACu; }
  // bit 0 once the table is built
  static uint32_t node_step_guard(uint8_t* base) { return guest::LoadU32(base, 0x828160C4u); }
  static void set_node_step_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828160C4u, v); }
  static constexpr uint32_t node_step_guard_address() { return 0x828160C4u; }
  // the list part inner's page table (Page0 to Page2)
  static constexpr uint32_t list_part_pages_address() { return 0x82637A20u; }
  // its step table (Step0 to Step2)
  static constexpr uint32_t list_part_steps_address() { return 0x82637A2Cu; }
  // the named animations of its two items
  static constexpr uint32_t list_part_named_address() { return 0x820112F0u; }
  // the type 6 list's and node screen's item string ids
  static constexpr uint32_t type6_item_texts_address() { return 0x826366FCu; }
  // 8 Type6ItemInfos
  static constexpr uint32_t type6_item_infos_address() { return 0x82636720u; }
  // the type 7 list's item string ids
  static constexpr uint32_t type7_item_texts_address() { return 0x82637054u; }
  // the type 7 list's detail string ids
  static constexpr uint32_t type7_detail_texts_address() { return 0x82637078u; }
  // 9 Type7ItemInfos
  static constexpr uint32_t type7_item_infos_address() { return 0x82637098u; }
  // the type 7 list's title string ids
  static constexpr uint32_t type7_title_texts_address() { return 0x82637194u; }
  // the type 6 mode's car (type6-type7-modes.toml)
  static constexpr uint32_t type6_car_address() { return 0x826367E8u; }
  // the type 7 mode's car: PickCar fills it
  static constexpr uint32_t type7_car_address() { return 0x826371D0u; }
  // the type 7 car's +1972 (the list picked)
  static uint32_t type7_car_list(uint8_t* base) { return guest::LoadU32(base, 0x82637984u); }
  static void set_type7_car_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82637984u, v); }
  static constexpr uint32_t type7_car_list_address() { return 0x82637984u; }
  // its +1976 (the entry)
  static uint32_t type7_car_entry(uint8_t* base) { return guest::LoadU32(base, 0x82637988u); }
  static void set_type7_car_entry(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82637988u, v); }
  static constexpr uint32_t type7_car_entry_address() { return 0x82637988u; }
  // its +1980 (the slot)
  static uint32_t type7_car_slot(uint8_t* base) { return guest::LoadU32(base, 0x8263798Cu); }
  static void set_type7_car_slot(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8263798Cu, v); }
  static constexpr uint32_t type7_car_slot_address() { return 0x8263798Cu; }
  // the type 6 car's +1972
  static uint32_t type6_car_list(uint8_t* base) { return guest::LoadU32(base, 0x82636F9Cu); }
  static void set_type6_car_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82636F9Cu, v); }
  static constexpr uint32_t type6_car_list_address() { return 0x82636F9Cu; }
  // its +1976
  static uint32_t type6_car_entry(uint8_t* base) { return guest::LoadU32(base, 0x82636FA0u); }
  static void set_type6_car_entry(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82636FA0u, v); }
  static constexpr uint32_t type6_car_entry_address() { return 0x82636FA0u; }
  // its +1980
  static uint32_t type6_car_slot(uint8_t* base) { return guest::LoadU32(base, 0x82636FA4u); }
  static void set_type6_car_slot(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82636FA4u, v); }
  static constexpr uint32_t type6_car_slot_address() { return 0x82636FA4u; }
};

}  // namespace schema
