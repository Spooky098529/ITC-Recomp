// The views of docs/functions/car-list.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/car-list.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: PanelSetBase, PanelSetBase.+1820, PanelSetBase::PanelSetBase.r3, PanelSetBase::Build.r3, PanelSetBase::ToTabs.r3, PanelSetBase::Update.r3, PanelSetBase::HandleInput.r3
struct PanelSetBase : View {
  using View::View;
  static constexpr uint32_t kSize = 1972;
  static constexpr uint32_t kVtable = 0x820110C4;
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the lists built (Build: nothing again while it and the two-player word share a bit)
  uint32_t built() const { return guest::LoadU32(address + 4); }
  void set_built(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t built_address() const { return address + 4; }
  // a car picked (slot 8)
  uint32_t picked() const { return guest::LoadU32(address + 8); }
  void set_picked(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t picked_address() const { return address + 8; }
  // the list done (picked or backed out)
  uint32_t done() const { return guest::LoadU32(address + 12); }
  void set_done(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t done_address() const { return address + 12; }
  // the power graph asked (slot 10)
  uint32_t power_graph_asked() const { return guest::LoadU32(address + 16); }
  void set_power_graph_asked(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t power_graph_asked_address() const { return address + 16; }
  // show the stand's car
  uint32_t show_stand_car() const { return guest::LoadU32(address + 20); }
  void set_show_stand_car(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t show_stand_car_address() const { return address + 20; }
  // its model loaded (GarageStage::ReloadModel answered 1)
  uint32_t stand_model_loaded() const { return guest::LoadU32(address + 24); }
  void set_stand_model_loaded(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t stand_model_loaded_address() const { return address + 24; }
  // group B's busy animation playing
  uint32_t busy_playing() const { return guest::LoadU32(address + 28); }
  void set_busy_playing(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t busy_playing_address() const { return address + 28; }
  // 0 tabs, 1 rows, 2 the car's actions, 3 colors, 4 the two players' confirm, 5 picked, 6 the power graph, 7 and 8 back; -1 while a step runs
  uint32_t page() const { return guest::LoadU32(address + 32); }
  void set_page(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t page_address() const { return address + 32; }
  // the page after the step
  uint32_t next_page() const { return guest::LoadU32(address + 36); }
  void set_next_page(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t next_page_address() const { return address + 36; }
  // the page before
  uint32_t previous_page() const { return guest::LoadU32(address + 40); }
  void set_previous_page(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t previous_page_address() const { return address + 40; }
  // 0 to 26, -1 none
  uint32_t step() const { return guest::LoadU32(address + 44); }
  void set_step(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t step_address() const { return address + 44; }
  uint32_t queued_step() const { return guest::LoadU32(address + 48); }
  void set_queued_step(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t queued_step_address() const { return address + 48; }
  // cleared on each step
  uint32_t step_cleared() const { return guest::LoadU32(address + 52); }
  void set_step_cleared(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t step_cleared_address() const { return address + 52; }
  // the step's first-pass word (each step's r4 points here)
  uint32_t first_pass() const { return guest::LoadU32(address + 56); }
  void set_first_pass(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t first_pass_address() const { return address + 56; }
  // the page's menu (a MenuList)
  uint32_t page_menu_address() const { return address + 60; }
  // the page menu's +0, the item count
  uint32_t page_menu_count() const { return guest::LoadU32(address + 60); }
  void set_page_menu_count(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t page_menu_count_address() const { return address + 60; }
  // the page menu's +4
  uint32_t page_menu_scroll() const { return guest::LoadU32(address + 64); }
  void set_page_menu_scroll(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t page_menu_scroll_address() const { return address + 64; }
  // the page menu's +8
  uint32_t page_menu_scroll_before() const { return guest::LoadU32(address + 68); }
  void set_page_menu_scroll_before(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t page_menu_scroll_before_address() const { return address + 68; }
  // the page menu's +24 (1, -1, 0)
  uint32_t page_menu_last_move() const { return guest::LoadU32(address + 84); }
  void set_page_menu_last_move(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t page_menu_last_move_address() const { return address + 84; }
  // the page menu's +32
  uint32_t page_menu_wrapped() const { return guest::LoadU32(address + 92); }
  void set_page_menu_wrapped(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t page_menu_wrapped_address() const { return address + 92; }
  // the page menu's +60
  uint32_t page_menu_player() const { return guest::LoadU32(address + 120); }
  void set_page_menu_player(uint32_t v) const { guest::StoreU32(address + 120, v); }
  uint32_t page_menu_player_address() const { return address + 120; }
  // the page menu's +68 (a word an item)
  uint32_t page_menu_items() const { return guest::LoadU32(address + 128); }
  void set_page_menu_items(uint32_t v) const { guest::StoreU32(address + 128, v); }
  uint32_t page_menu_items_address() const { return address + 128; }
  // the rows' menu (a MenuList)
  uint32_t rows_menu_address() const { return address + 132; }
  // the rows menu's +4
  uint32_t rows_menu_scroll() const { return guest::LoadU32(address + 136); }
  void set_rows_menu_scroll(uint32_t v) const { guest::StoreU32(address + 136, v); }
  uint32_t rows_menu_scroll_address() const { return address + 136; }
  // the rows menu's +8
  uint32_t rows_menu_scroll_before() const { return guest::LoadU32(address + 140); }
  void set_rows_menu_scroll_before(uint32_t v) const { guest::StoreU32(address + 140, v); }
  uint32_t rows_menu_scroll_before_address() const { return address + 140; }
  // the rows menu's +24
  uint32_t rows_menu_last_move() const { return guest::LoadU32(address + 156); }
  void set_rows_menu_last_move(uint32_t v) const { guest::StoreU32(address + 156, v); }
  uint32_t rows_menu_last_move_address() const { return address + 156; }
  // the rows menu's +60
  uint32_t rows_menu_player() const { return guest::LoadU32(address + 192); }
  void set_rows_menu_player(uint32_t v) const { guest::StoreU32(address + 192, v); }
  uint32_t rows_menu_player_address() const { return address + 192; }
  // the rows menu's +68
  uint32_t rows_menu_items() const { return guest::LoadU32(address + 200); }
  void set_rows_menu_items(uint32_t v) const { guest::StoreU32(address + 200, v); }
  uint32_t rows_menu_items_address() const { return address + 200; }
  // the record slot 18 answers starts here (its +28 to +36 are tab, row and scroll); 0 in the constructor
  uint32_t answer_record() const { return guest::LoadU32(address + 204); }
  void set_answer_record(uint32_t v) const { guest::StoreU32(address + 204, v); }
  uint32_t answer_record_address() const { return address + 204; }
  // set by Build (20); not read
  uint32_t unknown_208() const { return guest::LoadU32(address + 208); }
  void set_unknown_208(uint32_t v) const { guest::StoreU32(address + 208, v); }
  uint32_t unknown_208_address() const { return address + 208; }
  // 0 in the constructor
  uint32_t unknown_212() const { return guest::LoadU32(address + 212); }
  void set_unknown_212(uint32_t v) const { guest::StoreU32(address + 212, v); }
  uint32_t unknown_212_address() const { return address + 212; }
  // 0 in the constructor
  uint32_t unknown_216() const { return guest::LoadU32(address + 216); }
  void set_unknown_216(uint32_t v) const { guest::StoreU32(address + 216, v); }
  uint32_t unknown_216_address() const { return address + 216; }
  // 0 in the constructor
  uint32_t unknown_220() const { return guest::LoadU32(address + 220); }
  void set_unknown_220(uint32_t v) const { guest::StoreU32(address + 220, v); }
  uint32_t unknown_220_address() const { return address + 220; }
  // -1 in the constructor
  uint32_t unknown_224() const { return guest::LoadU32(address + 224); }
  void set_unknown_224(uint32_t v) const { guest::StoreU32(address + 224, v); }
  uint32_t unknown_224_address() const { return address + 224; }
  uint32_t tab_shown() const { return guest::LoadU32(address + 228); }
  void set_tab_shown(uint32_t v) const { guest::StoreU32(address + 228, v); }
  uint32_t tab_shown_address() const { return address + 228; }
  // 0 and 1 the unlocked cars by their spec's byte +771, 2 the garage
  uint32_t tab() const { return guest::LoadU32(address + 232); }
  void set_tab(uint32_t v) const { guest::StoreU32(address + 232, v); }
  uint32_t tab_address() const { return address + 232; }
  uint32_t row() const { return guest::LoadU32(address + 236); }
  void set_row(uint32_t v) const { guest::StoreU32(address + 236, v); }
  uint32_t row_address() const { return address + 236; }
  uint32_t scroll() const { return guest::LoadU32(address + 240); }
  void set_scroll(uint32_t v) const { guest::StoreU32(address + 240, v); }
  uint32_t scroll_address() const { return address + 240; }
  uint32_t color() const { return guest::LoadU32(address + 244); }
  void set_color(uint32_t v) const { guest::StoreU32(address + 244, v); }
  uint32_t color_address() const { return address + 244; }
  uint32_t color_scroll() const { return guest::LoadU32(address + 248); }
  void set_color_scroll(uint32_t v) const { guest::StoreU32(address + 248, v); }
  uint32_t color_scroll_address() const { return address + 248; }
  // set by Build (0, then 6,000); not read
  uint32_t unknown_252() const { return guest::LoadU32(address + 252); }
  void set_unknown_252(uint32_t v) const { guest::StoreU32(address + 252, v); }
  uint32_t unknown_252_address() const { return address + 252; }
  // set by Build (1); not read
  uint32_t unknown_256() const { return guest::LoadU32(address + 256); }
  void set_unknown_256(uint32_t v) const { guest::StoreU32(address + 256, v); }
  uint32_t unknown_256_address() const { return address + 256; }
  // set by Build (20); not read
  uint32_t unknown_260() const { return guest::LoadU32(address + 260); }
  void set_unknown_260(uint32_t v) const { guest::StoreU32(address + 260, v); }
  uint32_t unknown_260_address() const { return address + 260; }
  // set by Build (10); not read
  uint32_t unknown_264() const { return guest::LoadU32(address + 264); }
  void set_unknown_264(uint32_t v) const { guest::StoreU32(address + 264, v); }
  uint32_t unknown_264_address() const { return address + 264; }
  // set by Build; not read
  uint32_t unknown_268() const { return guest::LoadU32(address + 268); }
  void set_unknown_268(uint32_t v) const { guest::StoreU32(address + 268, v); }
  uint32_t unknown_268_address() const { return address + 268; }
  // a CarListList: the unlocked cars whose spec byte +771 is 0
  uint32_t list0_address() const { return address + 272; }
  uint32_t list0_count() const { return guest::LoadU32(address + 784); }
  void set_list0_count(uint32_t v) const { guest::StoreU32(address + 784, v); }
  uint32_t list0_count_address() const { return address + 784; }
  // a CarListList: the unlocked cars whose spec byte +771 is 1
  uint32_t list1_address() const { return address + 788; }
  uint32_t list1_count() const { return guest::LoadU32(address + 1300); }
  void set_list1_count(uint32_t v) const { guest::StoreU32(address + 1300, v); }
  uint32_t list1_count_address() const { return address + 1300; }
  // a CarListList: the garage's cars
  uint32_t list2_address() const { return address + 1304; }
  uint32_t list2_count() const { return guest::LoadU32(address + 1816); }
  void set_list2_count(uint32_t v) const { guest::StoreU32(address + 1816, v); }
  uint32_t list2_count_address() const { return address + 1816; }
  // list0, list1 or list2
  template <typename T = CarListList>
  T current_list() const { return T(guest::LoadU32(address + 1820)); }
  void set_current_list(const View& v) const { guest::StoreU32(address + 1820, v.address); }
  void set_current_list(uint32_t v) const { guest::StoreU32(address + 1820, v); }
  uint32_t current_list_address() const { return address + 1820; }
  // a CarListColorList (up to 13 CarListColor records, count at +1876)
  uint32_t colors_address() const { return address + 1824; }
  uint32_t color_count() const { return guest::LoadU32(address + 1876); }
  void set_color_count(uint32_t v) const { guest::StoreU32(address + 1876, v); }
  uint32_t color_count_address() const { return address + 1876; }
  // the tab picked before (slot 15)
  uint32_t previous_tab() const { return guest::LoadU32(address + 1880); }
  void set_previous_tab(uint32_t v) const { guest::StoreU32(address + 1880, v); }
  uint32_t previous_tab_address() const { return address + 1880; }
  // the entry picked before (slot 16)
  uint32_t previous_entry() const { return guest::LoadU32(address + 1884); }
  void set_previous_entry(uint32_t v) const { guest::StoreU32(address + 1884, v); }
  uint32_t previous_entry_address() const { return address + 1884; }
  // its color (slot 17)
  uint32_t previous_color() const { return guest::LoadU32(address + 1888); }
  void set_previous_color(uint32_t v) const { guest::StoreU32(address + 1888, v); }
  uint32_t previous_color_address() const { return address + 1888; }
  // 0 in both constructors, no writer seen; when set, pages 7 and 8 update it and backing out of the rows goes through step 26
  template <typename T = CarListBackObject>
  T back_object() const { return T(guest::LoadU32(address + 1892)); }
  void set_back_object(const View& v) const { guest::StoreU32(address + 1892, v.address); }
  void set_back_object(uint32_t v) const { guest::StoreU32(address + 1892, v); }
  uint32_t back_object_address() const { return address + 1892; }
  // F
  template <typename T = TabPanel>
  T tabs() const { return T(guest::LoadU32(address + 1896)); }
  void set_tabs(const View& v) const { guest::StoreU32(address + 1896, v.address); }
  void set_tabs(uint32_t v) const { guest::StoreU32(address + 1896, v); }
  uint32_t tabs_address() const { return address + 1896; }
  // E
  template <typename T = RowPanel>
  T rows() const { return T(guest::LoadU32(address + 1900)); }
  void set_rows(const View& v) const { guest::StoreU32(address + 1900, v.address); }
  void set_rows(uint32_t v) const { guest::StoreU32(address + 1900, v); }
  uint32_t rows_address() const { return address + 1900; }
  // D
  template <typename T = ColorPanel>
  T color_panel() const { return T(guest::LoadU32(address + 1904)); }
  void set_color_panel(const View& v) const { guest::StoreU32(address + 1904, v.address); }
  void set_color_panel(uint32_t v) const { guest::StoreU32(address + 1904, v); }
  uint32_t color_panel_address() const { return address + 1904; }
  // C
  template <typename T = CarPanel>
  T car_panel() const { return T(guest::LoadU32(address + 1908)); }
  void set_car_panel(const View& v) const { guest::StoreU32(address + 1908, v.address); }
  void set_car_panel(uint32_t v) const { guest::StoreU32(address + 1908, v); }
  uint32_t car_panel_address() const { return address + 1908; }
  // the two players' confirm panel
  template <typename T = ConfirmPanel>
  T confirm_panel() const { return T(guest::LoadU32(address + 1912)); }
  void set_confirm_panel(const View& v) const { guest::StoreU32(address + 1912, v.address); }
  void set_confirm_panel(uint32_t v) const { guest::StoreU32(address + 1912, v); }
  uint32_t confirm_panel_address() const { return address + 1912; }
  uint32_t rows_shown() const { return guest::LoadU32(address + 1916); }
  void set_rows_shown(uint32_t v) const { guest::StoreU32(address + 1916, v); }
  uint32_t rows_shown_address() const { return address + 1916; }
  // 1 in the constructor
  uint32_t unknown_1920() const { return guest::LoadU32(address + 1920); }
  void set_unknown_1920(uint32_t v) const { guest::StoreU32(address + 1920, v); }
  uint32_t unknown_1920_address() const { return address + 1920; }
  uint32_t colors_shown() const { return guest::LoadU32(address + 1924); }
  void set_colors_shown(uint32_t v) const { guest::StoreU32(address + 1924, v); }
  uint32_t colors_shown_address() const { return address + 1924; }
  // slot 13: 1 opens on the tabs at the list picked before, else on list 0's rows
  uint32_t open_kind() const { return guest::LoadU32(address + 1928); }
  void set_open_kind(uint32_t v) const { guest::StoreU32(address + 1928, v); }
  uint32_t open_kind_address() const { return address + 1928; }
  // the page HandleInput ran last
  uint32_t page_drawn() const { return guest::LoadU32(address + 1932); }
  void set_page_drawn(uint32_t v) const { guest::StoreU32(address + 1932, v); }
  uint32_t page_drawn_address() const { return address + 1932; }
  uint32_t page_changed() const { return guest::LoadU32(address + 1936); }
  void set_page_changed(uint32_t v) const { guest::StoreU32(address + 1936, v); }
  uint32_t page_changed_address() const { return address + 1936; }
  // may leave on cancel
  uint32_t may_leave() const { return guest::LoadU32(address + 1940); }
  void set_may_leave(uint32_t v) const { guest::StoreU32(address + 1940, v); }
  uint32_t may_leave_address() const { return address + 1940; }
  // cancelled this frame
  uint32_t cancelled() const { return guest::LoadU32(address + 1944); }
  void set_cancelled(uint32_t v) const { guest::StoreU32(address + 1944, v); }
  uint32_t cancelled_address() const { return address + 1944; }
  uint32_t fade_with_steps() const { return guest::LoadU32(address + 1948); }
  void set_fade_with_steps(uint32_t v) const { guest::StoreU32(address + 1948, v); }
  uint32_t fade_with_steps_address() const { return address + 1948; }
  // -1, or 0 and 1 in PanelSetPair
  uint32_t player() const { return guest::LoadU32(address + 1952); }
  void set_player(uint32_t v) const { guest::StoreU32(address + 1952, v); }
  uint32_t player_address() const { return address + 1952; }
  // the screen's panel group B
  View group_b() const { return View(guest::LoadU32(address + 1956)); }
  void set_group_b(uint32_t v) const { guest::StoreU32(address + 1956, v); }
  uint32_t group_b_address() const { return address + 1956; }
  // group B's busy animations (-1 none): looped
  uint32_t busy_anim_a() const { return guest::LoadU32(address + 1960); }
  void set_busy_anim_a(uint32_t v) const { guest::StoreU32(address + 1960, v); }
  uint32_t busy_anim_a_address() const { return address + 1960; }
  // looped
  uint32_t busy_anim_b() const { return guest::LoadU32(address + 1964); }
  void set_busy_anim_b(uint32_t v) const { guest::StoreU32(address + 1964, v); }
  uint32_t busy_anim_b_address() const { return address + 1964; }
  // played once not busy
  uint32_t busy_anim_done() const { return guest::LoadU32(address + 1968); }
  void set_busy_anim_done(uint32_t v) const { guest::StoreU32(address + 1968, v); }
  uint32_t busy_anim_done_address() const { return address + 1968; }
};

// embedded; measure: PanelSet, PanelScreen.+9644, PanelSet::PanelSet.r3
struct PanelSet : View {
  using View::View;
  static constexpr uint32_t kSize = 1976;
  static constexpr uint32_t kVtable = 0x820110C8;
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the base's (PanelSetBase at +4)
  uint32_t base_vtable() const { return guest::LoadU32(address + 4); }
  void set_base_vtable(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t base_vtable_address() const { return address + 4; }
  uint32_t picked() const { return guest::LoadU32(address + 12); }
  void set_picked(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t picked_address() const { return address + 12; }
  uint32_t done() const { return guest::LoadU32(address + 16); }
  void set_done(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t done_address() const { return address + 16; }
  uint32_t power_graph_asked() const { return guest::LoadU32(address + 20); }
  void set_power_graph_asked(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t power_graph_asked_address() const { return address + 20; }
  uint32_t show_stand_car() const { return guest::LoadU32(address + 24); }
  void set_show_stand_car(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t show_stand_car_address() const { return address + 24; }
  uint32_t stand_model_loaded() const { return guest::LoadU32(address + 28); }
  void set_stand_model_loaded(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t stand_model_loaded_address() const { return address + 28; }
  uint32_t page() const { return guest::LoadU32(address + 36); }
  void set_page(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t page_address() const { return address + 36; }
  uint32_t next_page() const { return guest::LoadU32(address + 40); }
  void set_next_page(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t next_page_address() const { return address + 40; }
  uint32_t previous_page() const { return guest::LoadU32(address + 44); }
  void set_previous_page(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t previous_page_address() const { return address + 44; }
  uint32_t step() const { return guest::LoadU32(address + 48); }
  void set_step(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t step_address() const { return address + 48; }
  uint32_t step_cleared() const { return guest::LoadU32(address + 56); }
  void set_step_cleared(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t step_cleared_address() const { return address + 56; }
  uint32_t first_pass() const { return guest::LoadU32(address + 60); }
  void set_first_pass(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t first_pass_address() const { return address + 60; }
  uint32_t tab_shown() const { return guest::LoadU32(address + 232); }
  void set_tab_shown(uint32_t v) const { guest::StoreU32(address + 232, v); }
  uint32_t tab_shown_address() const { return address + 232; }
  uint32_t tab() const { return guest::LoadU32(address + 236); }
  void set_tab(uint32_t v) const { guest::StoreU32(address + 236, v); }
  uint32_t tab_address() const { return address + 236; }
  uint32_t row() const { return guest::LoadU32(address + 240); }
  void set_row(uint32_t v) const { guest::StoreU32(address + 240, v); }
  uint32_t row_address() const { return address + 240; }
  uint32_t scroll() const { return guest::LoadU32(address + 244); }
  void set_scroll(uint32_t v) const { guest::StoreU32(address + 244, v); }
  uint32_t scroll_address() const { return address + 244; }
  uint32_t color() const { return guest::LoadU32(address + 248); }
  void set_color(uint32_t v) const { guest::StoreU32(address + 248, v); }
  uint32_t color_address() const { return address + 248; }
  uint32_t color_scroll() const { return guest::LoadU32(address + 252); }
  void set_color_scroll(uint32_t v) const { guest::StoreU32(address + 252, v); }
  uint32_t color_scroll_address() const { return address + 252; }
  template <typename T = CarListList>
  T current_list() const { return T(guest::LoadU32(address + 1824)); }
  void set_current_list(const View& v) const { guest::StoreU32(address + 1824, v.address); }
  void set_current_list(uint32_t v) const { guest::StoreU32(address + 1824, v); }
  uint32_t current_list_address() const { return address + 1824; }
  uint32_t previous_tab() const { return guest::LoadU32(address + 1884); }
  void set_previous_tab(uint32_t v) const { guest::StoreU32(address + 1884, v); }
  uint32_t previous_tab_address() const { return address + 1884; }
  uint32_t previous_entry() const { return guest::LoadU32(address + 1888); }
  void set_previous_entry(uint32_t v) const { guest::StoreU32(address + 1888, v); }
  uint32_t previous_entry_address() const { return address + 1888; }
  uint32_t previous_color() const { return guest::LoadU32(address + 1892); }
  void set_previous_color(uint32_t v) const { guest::StoreU32(address + 1892, v); }
  uint32_t previous_color_address() const { return address + 1892; }
  template <typename T = TabPanel>
  T tabs() const { return T(guest::LoadU32(address + 1900)); }
  void set_tabs(const View& v) const { guest::StoreU32(address + 1900, v.address); }
  void set_tabs(uint32_t v) const { guest::StoreU32(address + 1900, v); }
  uint32_t tabs_address() const { return address + 1900; }
  template <typename T = RowPanel>
  T rows() const { return T(guest::LoadU32(address + 1904)); }
  void set_rows(const View& v) const { guest::StoreU32(address + 1904, v.address); }
  void set_rows(uint32_t v) const { guest::StoreU32(address + 1904, v); }
  uint32_t rows_address() const { return address + 1904; }
  template <typename T = ColorPanel>
  T color_panel() const { return T(guest::LoadU32(address + 1908)); }
  void set_color_panel(const View& v) const { guest::StoreU32(address + 1908, v.address); }
  void set_color_panel(uint32_t v) const { guest::StoreU32(address + 1908, v); }
  uint32_t color_panel_address() const { return address + 1908; }
  template <typename T = CarPanel>
  T car_panel() const { return T(guest::LoadU32(address + 1912)); }
  void set_car_panel(const View& v) const { guest::StoreU32(address + 1912, v.address); }
  void set_car_panel(uint32_t v) const { guest::StoreU32(address + 1912, v); }
  uint32_t car_panel_address() const { return address + 1912; }
  uint32_t open_kind() const { return guest::LoadU32(address + 1932); }
  void set_open_kind(uint32_t v) const { guest::StoreU32(address + 1932, v); }
  uint32_t open_kind_address() const { return address + 1932; }
  uint32_t may_leave() const { return guest::LoadU32(address + 1944); }
  void set_may_leave(uint32_t v) const { guest::StoreU32(address + 1944, v); }
  uint32_t may_leave_address() const { return address + 1944; }
  uint32_t cancelled() const { return guest::LoadU32(address + 1948); }
  void set_cancelled(uint32_t v) const { guest::StoreU32(address + 1948, v); }
  uint32_t cancelled_address() const { return address + 1948; }
  uint32_t fade_with_steps() const { return guest::LoadU32(address + 1952); }
  void set_fade_with_steps(uint32_t v) const { guest::StoreU32(address + 1952, v); }
  uint32_t fade_with_steps_address() const { return address + 1952; }
  uint32_t player() const { return guest::LoadU32(address + 1956); }
  void set_player(uint32_t v) const { guest::StoreU32(address + 1956, v); }
  uint32_t player_address() const { return address + 1956; }
  View group_b() const { return View(guest::LoadU32(address + 1960)); }
  void set_group_b(uint32_t v) const { guest::StoreU32(address + 1960, v); }
  uint32_t group_b_address() const { return address + 1960; }
};

// embedded; measure: PanelSetPair
struct PanelSetPair : View {
  using View::View;
  static constexpr uint32_t kSize = 3948;
  static constexpr uint32_t kVtable = 0x82011118;
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the first base's vtable (PanelSetBase at +4)
  uint32_t first_base() const { return guest::LoadU32(address + 4); }
  void set_first_base(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t first_base_address() const { return address + 4; }
  uint32_t first_picked() const { return guest::LoadU32(address + 12); }
  void set_first_picked(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t first_picked_address() const { return address + 12; }
  uint32_t first_done() const { return guest::LoadU32(address + 16); }
  void set_first_done(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t first_done_address() const { return address + 16; }
  uint32_t first_power_graph_asked() const { return guest::LoadU32(address + 20); }
  void set_first_power_graph_asked(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t first_power_graph_asked_address() const { return address + 20; }
  uint32_t first_show_stand_car() const { return guest::LoadU32(address + 24); }
  void set_first_show_stand_car(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t first_show_stand_car_address() const { return address + 24; }
  uint32_t first_stand_model_loaded() const { return guest::LoadU32(address + 28); }
  void set_first_stand_model_loaded(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t first_stand_model_loaded_address() const { return address + 28; }
  uint32_t first_page() const { return guest::LoadU32(address + 36); }
  void set_first_page(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t first_page_address() const { return address + 36; }
  uint32_t first_next_page() const { return guest::LoadU32(address + 40); }
  void set_first_next_page(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t first_next_page_address() const { return address + 40; }
  uint32_t first_previous_page() const { return guest::LoadU32(address + 44); }
  void set_first_previous_page(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t first_previous_page_address() const { return address + 44; }
  uint32_t first_step() const { return guest::LoadU32(address + 48); }
  void set_first_step(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t first_step_address() const { return address + 48; }
  uint32_t first_step_cleared() const { return guest::LoadU32(address + 56); }
  void set_first_step_cleared(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t first_step_cleared_address() const { return address + 56; }
  uint32_t first_first_pass() const { return guest::LoadU32(address + 60); }
  void set_first_first_pass(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t first_first_pass_address() const { return address + 60; }
  uint32_t first_tab_shown() const { return guest::LoadU32(address + 232); }
  void set_first_tab_shown(uint32_t v) const { guest::StoreU32(address + 232, v); }
  uint32_t first_tab_shown_address() const { return address + 232; }
  uint32_t first_tab() const { return guest::LoadU32(address + 236); }
  void set_first_tab(uint32_t v) const { guest::StoreU32(address + 236, v); }
  uint32_t first_tab_address() const { return address + 236; }
  uint32_t first_row() const { return guest::LoadU32(address + 240); }
  void set_first_row(uint32_t v) const { guest::StoreU32(address + 240, v); }
  uint32_t first_row_address() const { return address + 240; }
  uint32_t first_scroll() const { return guest::LoadU32(address + 244); }
  void set_first_scroll(uint32_t v) const { guest::StoreU32(address + 244, v); }
  uint32_t first_scroll_address() const { return address + 244; }
  uint32_t first_color() const { return guest::LoadU32(address + 248); }
  void set_first_color(uint32_t v) const { guest::StoreU32(address + 248, v); }
  uint32_t first_color_address() const { return address + 248; }
  uint32_t first_color_scroll() const { return guest::LoadU32(address + 252); }
  void set_first_color_scroll(uint32_t v) const { guest::StoreU32(address + 252, v); }
  uint32_t first_color_scroll_address() const { return address + 252; }
  template <typename T = CarListList>
  T first_current_list() const { return T(guest::LoadU32(address + 1824)); }
  void set_first_current_list(const View& v) const { guest::StoreU32(address + 1824, v.address); }
  void set_first_current_list(uint32_t v) const { guest::StoreU32(address + 1824, v); }
  uint32_t first_current_list_address() const { return address + 1824; }
  template <typename T = TabPanel>
  T first_tabs() const { return T(guest::LoadU32(address + 1900)); }
  void set_first_tabs(const View& v) const { guest::StoreU32(address + 1900, v.address); }
  void set_first_tabs(uint32_t v) const { guest::StoreU32(address + 1900, v); }
  uint32_t first_tabs_address() const { return address + 1900; }
  template <typename T = RowPanel>
  T first_rows() const { return T(guest::LoadU32(address + 1904)); }
  void set_first_rows(const View& v) const { guest::StoreU32(address + 1904, v.address); }
  void set_first_rows(uint32_t v) const { guest::StoreU32(address + 1904, v); }
  uint32_t first_rows_address() const { return address + 1904; }
  template <typename T = ColorPanel>
  T first_color_panel() const { return T(guest::LoadU32(address + 1908)); }
  void set_first_color_panel(const View& v) const { guest::StoreU32(address + 1908, v.address); }
  void set_first_color_panel(uint32_t v) const { guest::StoreU32(address + 1908, v); }
  uint32_t first_color_panel_address() const { return address + 1908; }
  template <typename T = CarPanel>
  T first_car_panel() const { return T(guest::LoadU32(address + 1912)); }
  void set_first_car_panel(const View& v) const { guest::StoreU32(address + 1912, v.address); }
  void set_first_car_panel(uint32_t v) const { guest::StoreU32(address + 1912, v); }
  uint32_t first_car_panel_address() const { return address + 1912; }
  template <typename T = ConfirmPanel>
  T first_confirm_panel() const { return T(guest::LoadU32(address + 1916)); }
  void set_first_confirm_panel(const View& v) const { guest::StoreU32(address + 1916, v.address); }
  void set_first_confirm_panel(uint32_t v) const { guest::StoreU32(address + 1916, v); }
  uint32_t first_confirm_panel_address() const { return address + 1916; }
  uint32_t first_may_leave() const { return guest::LoadU32(address + 1944); }
  void set_first_may_leave(uint32_t v) const { guest::StoreU32(address + 1944, v); }
  uint32_t first_may_leave_address() const { return address + 1944; }
  uint32_t first_cancelled() const { return guest::LoadU32(address + 1948); }
  void set_first_cancelled(uint32_t v) const { guest::StoreU32(address + 1948, v); }
  uint32_t first_cancelled_address() const { return address + 1948; }
  uint32_t first_fade_with_steps() const { return guest::LoadU32(address + 1952); }
  void set_first_fade_with_steps(uint32_t v) const { guest::StoreU32(address + 1952, v); }
  uint32_t first_fade_with_steps_address() const { return address + 1952; }
  uint32_t first_player() const { return guest::LoadU32(address + 1956); }
  void set_first_player(uint32_t v) const { guest::StoreU32(address + 1956, v); }
  uint32_t first_player_address() const { return address + 1956; }
  View first_group_b() const { return View(guest::LoadU32(address + 1960)); }
  void set_first_group_b(uint32_t v) const { guest::StoreU32(address + 1960, v); }
  uint32_t first_group_b_address() const { return address + 1960; }
  // the second base's vtable (PanelSetBase at +1976)
  uint32_t second_base() const { return guest::LoadU32(address + 1976); }
  void set_second_base(uint32_t v) const { guest::StoreU32(address + 1976, v); }
  uint32_t second_base_address() const { return address + 1976; }
  uint32_t second_picked() const { return guest::LoadU32(address + 1984); }
  void set_second_picked(uint32_t v) const { guest::StoreU32(address + 1984, v); }
  uint32_t second_picked_address() const { return address + 1984; }
  uint32_t second_done() const { return guest::LoadU32(address + 1988); }
  void set_second_done(uint32_t v) const { guest::StoreU32(address + 1988, v); }
  uint32_t second_done_address() const { return address + 1988; }
  template <typename T = TabPanel>
  T second_tabs() const { return T(guest::LoadU32(address + 3872)); }
  void set_second_tabs(const View& v) const { guest::StoreU32(address + 3872, v.address); }
  void set_second_tabs(uint32_t v) const { guest::StoreU32(address + 3872, v); }
  uint32_t second_tabs_address() const { return address + 3872; }
  template <typename T = RowPanel>
  T second_rows() const { return T(guest::LoadU32(address + 3876)); }
  void set_second_rows(const View& v) const { guest::StoreU32(address + 3876, v.address); }
  void set_second_rows(uint32_t v) const { guest::StoreU32(address + 3876, v); }
  uint32_t second_rows_address() const { return address + 3876; }
  template <typename T = ColorPanel>
  T second_color_panel() const { return T(guest::LoadU32(address + 3880)); }
  void set_second_color_panel(const View& v) const { guest::StoreU32(address + 3880, v.address); }
  void set_second_color_panel(uint32_t v) const { guest::StoreU32(address + 3880, v); }
  uint32_t second_color_panel_address() const { return address + 3880; }
  template <typename T = CarPanel>
  T second_car_panel() const { return T(guest::LoadU32(address + 3884)); }
  void set_second_car_panel(const View& v) const { guest::StoreU32(address + 3884, v.address); }
  void set_second_car_panel(uint32_t v) const { guest::StoreU32(address + 3884, v); }
  uint32_t second_car_panel_address() const { return address + 3884; }
  template <typename T = ConfirmPanel>
  T second_confirm_panel() const { return T(guest::LoadU32(address + 3888)); }
  void set_second_confirm_panel(const View& v) const { guest::StoreU32(address + 3888, v.address); }
  void set_second_confirm_panel(uint32_t v) const { guest::StoreU32(address + 3888, v); }
  uint32_t second_confirm_panel_address() const { return address + 3888; }
  uint32_t second_may_leave() const { return guest::LoadU32(address + 3916); }
  void set_second_may_leave(uint32_t v) const { guest::StoreU32(address + 3916, v); }
  uint32_t second_may_leave_address() const { return address + 3916; }
  uint32_t second_cancelled() const { return guest::LoadU32(address + 3920); }
  void set_second_cancelled(uint32_t v) const { guest::StoreU32(address + 3920, v); }
  uint32_t second_cancelled_address() const { return address + 3920; }
  uint32_t second_fade_with_steps() const { return guest::LoadU32(address + 3924); }
  void set_second_fade_with_steps(uint32_t v) const { guest::StoreU32(address + 3924, v); }
  uint32_t second_fade_with_steps_address() const { return address + 3924; }
  uint32_t second_player() const { return guest::LoadU32(address + 3928); }
  void set_second_player(uint32_t v) const { guest::StoreU32(address + 3928, v); }
  uint32_t second_player_address() const { return address + 3928; }
  View second_group_b() const { return View(guest::LoadU32(address + 3932)); }
  void set_second_group_b(uint32_t v) const { guest::StoreU32(address + 3932, v); }
  uint32_t second_group_b_address() const { return address + 3932; }
};

// embedded; measure: PanelSetInterface
struct PanelSetInterface : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr uint32_t kVtable = 0x82011018;
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// embedded; measure: -
struct CarListList : View {
  using View::View;
  static constexpr uint32_t kSize = 516;
  // 128 pointers to CarListEntry
  uint32_t entries_address() const { return address + 0; }
  uint32_t count() const { return guest::LoadU32(address + 512); }
  void set_count(uint32_t v) const { guest::StoreU32(address + 512, v); }
  uint32_t count_address() const { return address + 512; }
};

// allocated; measure: new<8448>@PanelSetBase::Build, new<8448>@PanelSetBase::Build#2, CarPanel::SetName.r4, CarPanel::ShowStats.r4, RowPanel::SetRow.r5
#pragma pack(push, 1)
struct CarListEntryData {
  uint8_t pad_0[648] = {};
  uint32_t displacement = 0;  // +648
  float unknown_652 = 0;  // +652
  float unknown_656 = 0;  // +656
  float unknown_660 = 0;  // +660
  float unknown_664 = 0;  // +664
  uint8_t pad_668[40] = {};
  uint8_t unknown_708 = 0;  // +708
  uint8_t turbo = 0;  // +709
  uint8_t pad_710[2] = {};
  uint32_t unknown_712 = 0;  // +712
  uint32_t unknown_716 = 0;  // +716
  uint32_t unknown_720 = 0;  // +720
  uint32_t unknown_724 = 0;  // +724
  uint8_t pad_728[16] = {};
  uint32_t price = 0;  // +744
  uint8_t pad_748[21] = {};
  uint8_t maker = 0;  // +769
  uint8_t unknown_770 = 0;  // +770
  uint8_t list_kind = 0;  // +771
  uint8_t pad_772[7344] = {};
  uint8_t color_records[260] = {};  // +8116, guest order
  uint32_t color_count = 0;  // +8376
  uint8_t pad_8380[52] = {};
  uint32_t car_id = 0;  // +8432
  uint32_t color = 0;  // +8436
  uint8_t pad_8440[8] = {};
};
#pragma pack(pop)
static_assert(sizeof(CarListEntryData) == 8448);
inline constexpr memory::LayoutField kCarListEntryFields[] = {{648, 4}, {652, 4}, {656, 4}, {660, 4}, {664, 4}, {712, 4}, {716, 4}, {720, 4}, {724, 4}, {744, 4}, {8376, 4}, {8432, 4}, {8436, 4}};
inline constexpr memory::Layout kCarListEntryLayout = {kCarListEntryFields, 13};

struct CarListEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 8448;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CarListEntryData* data() const { return static_cast<CarListEntryData*>(memory::HandleObject(address)); }
  static CarListEntry New() {
    return CarListEntry(memory::MakeHandle(new CarListEntryData(), sizeof(CarListEntryData), "CarListEntry", &kCarListEntryLayout));
  }
  template <uint32_t Stride = kSize>
  static CarListEntry NewArray(uint32_t count) {
    static_assert(sizeof(CarListEntryData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarListEntryData*>(::operator new(sizeof(CarListEntryData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarListEntryData();
    return CarListEntry(memory::MakeHandle(p, sizeof(CarListEntryData) * count, "CarListEntry", &kCarListEntryLayout, sizeof(CarListEntryData)));
  }
  CarListEntry at(uint32_t i) const { return CarListEntry(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarListEntryData* direct() const { return static_cast<CarListEntryData*>(memory::HandleOwn(address, &kCarListEntryLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarListEntryLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarListEntryLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // the spec's block 2 +8 (cc)
  uint32_t displacement_address() const { return address + 648; }
  uint32_t displacement_bits() const { if (auto* d = direct()) return d->displacement; return uint32_t((memory::IsHandle(address + 648) ? uint32_t(memory::OwnerLoad(address + 648, 4)) : guest::LoadU32(address + 648))); }
  void set_displacement_bits(uint32_t v) const { if (auto* d = direct()) { d->displacement = v; return; } if (memory::IsHandle(address + 648)) memory::OwnerStore(address + 648, 4, v); else guest::StoreU32(address + 648, v); }
  uint32_t displacement() const { if (auto* d = direct()) return d->displacement; return uint32_t((memory::IsHandle(address + 648) ? uint32_t(memory::OwnerLoad(address + 648, 4)) : guest::LoadU32(address + 648))); }
  void set_displacement(uint32_t v) const { if (auto* d = direct()) { d->displacement = v; return; } if (memory::IsHandle(address + 648)) memory::OwnerStore(address + 648, 4, v); else guest::StoreU32(address + 648, v); }
  // one of power, torque and their rpm (block 2 +12 to +24)
  uint32_t unknown_652_address() const { return address + 652; }
  uint32_t unknown_652_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_652); return uint32_t((memory::IsHandle(address + 652) ? uint32_t(memory::OwnerLoad(address + 652, 4)) : guest::LoadU32(address + 652))); }
  void set_unknown_652_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_652 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 652)) memory::OwnerStore(address + 652, 4, v); else guest::StoreU32(address + 652, v); }
  float unknown_652() const { return std::bit_cast<float>(unknown_652_bits()); }
  void set_unknown_652(float v) const { set_unknown_652_bits(std::bit_cast<uint32_t>(v)); }
  // one of power, torque and their rpm
  uint32_t unknown_656_address() const { return address + 656; }
  uint32_t unknown_656_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_656); return uint32_t((memory::IsHandle(address + 656) ? uint32_t(memory::OwnerLoad(address + 656, 4)) : guest::LoadU32(address + 656))); }
  void set_unknown_656_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_656 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 656)) memory::OwnerStore(address + 656, 4, v); else guest::StoreU32(address + 656, v); }
  float unknown_656() const { return std::bit_cast<float>(unknown_656_bits()); }
  void set_unknown_656(float v) const { set_unknown_656_bits(std::bit_cast<uint32_t>(v)); }
  // one of power, torque and their rpm
  uint32_t unknown_660_address() const { return address + 660; }
  uint32_t unknown_660_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_660); return uint32_t((memory::IsHandle(address + 660) ? uint32_t(memory::OwnerLoad(address + 660, 4)) : guest::LoadU32(address + 660))); }
  void set_unknown_660_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_660 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 660)) memory::OwnerStore(address + 660, 4, v); else guest::StoreU32(address + 660, v); }
  float unknown_660() const { return std::bit_cast<float>(unknown_660_bits()); }
  void set_unknown_660(float v) const { set_unknown_660_bits(std::bit_cast<uint32_t>(v)); }
  // one of power, torque and their rpm
  uint32_t unknown_664_address() const { return address + 664; }
  uint32_t unknown_664_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_664); return uint32_t((memory::IsHandle(address + 664) ? uint32_t(memory::OwnerLoad(address + 664, 4)) : guest::LoadU32(address + 664))); }
  void set_unknown_664_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_664 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 664)) memory::OwnerStore(address + 664, 4, v); else guest::StoreU32(address + 664, v); }
  float unknown_664() const { return std::bit_cast<float>(unknown_664_bits()); }
  void set_unknown_664(float v) const { set_unknown_664_bits(std::bit_cast<uint32_t>(v)); }
  // block 2 +68, the engine type (PowerGraph::SetEntry's class switch)
  uint32_t unknown_708_address() const { return address + 708; }
  uint8_t unknown_708_bits() const { if (auto* d = direct()) return d->unknown_708; return uint8_t((memory::IsHandle(address + 708) ? uint8_t(memory::OwnerLoad(address + 708, 1)) : guest::LoadU8(address + 708))); }
  void set_unknown_708_bits(uint8_t v) const { if (auto* d = direct()) { d->unknown_708 = v; return; } if (memory::IsHandle(address + 708)) memory::OwnerStore(address + 708, 1, v); else guest::StoreU8(address + 708, v); }
  uint8_t unknown_708() const { if (auto* d = direct()) return d->unknown_708; return uint8_t((memory::IsHandle(address + 708) ? uint8_t(memory::OwnerLoad(address + 708, 1)) : guest::LoadU8(address + 708))); }
  void set_unknown_708(uint8_t v) const { if (auto* d = direct()) { d->unknown_708 = v; return; } if (memory::IsHandle(address + 708)) memory::OwnerStore(address + 708, 1, v); else guest::StoreU8(address + 708, v); }
  // block 2 +69: 1 for turbo
  uint32_t turbo_address() const { return address + 709; }
  uint8_t turbo_bits() const { if (auto* d = direct()) return d->turbo; return uint8_t((memory::IsHandle(address + 709) ? uint8_t(memory::OwnerLoad(address + 709, 1)) : guest::LoadU8(address + 709))); }
  void set_turbo_bits(uint8_t v) const { if (auto* d = direct()) { d->turbo = v; return; } if (memory::IsHandle(address + 709)) memory::OwnerStore(address + 709, 1, v); else guest::StoreU8(address + 709, v); }
  uint8_t turbo() const { if (auto* d = direct()) return d->turbo; return uint8_t((memory::IsHandle(address + 709) ? uint8_t(memory::OwnerLoad(address + 709, 1)) : guest::LoadU8(address + 709))); }
  void set_turbo(uint8_t v) const { if (auto* d = direct()) { d->turbo = v; return; } if (memory::IsHandle(address + 709)) memory::OwnerStore(address + 709, 1, v); else guest::StoreU8(address + 709, v); }
  // shown by CarPanel::SetStats (scaled); the power graph record's +4
  uint32_t unknown_712_address() const { return address + 712; }
  uint32_t unknown_712_bits() const { if (auto* d = direct()) return d->unknown_712; return uint32_t((memory::IsHandle(address + 712) ? uint32_t(memory::OwnerLoad(address + 712, 4)) : guest::LoadU32(address + 712))); }
  void set_unknown_712_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_712 = v; return; } if (memory::IsHandle(address + 712)) memory::OwnerStore(address + 712, 4, v); else guest::StoreU32(address + 712, v); }
  uint32_t unknown_712() const { if (auto* d = direct()) return d->unknown_712; return uint32_t((memory::IsHandle(address + 712) ? uint32_t(memory::OwnerLoad(address + 712, 4)) : guest::LoadU32(address + 712))); }
  void set_unknown_712(uint32_t v) const { if (auto* d = direct()) { d->unknown_712 = v; return; } if (memory::IsHandle(address + 712)) memory::OwnerStore(address + 712, 4, v); else guest::StoreU32(address + 712, v); }
  // the power graph record's +8 (the dealer: at most 4700)
  uint32_t unknown_716_address() const { return address + 716; }
  uint32_t unknown_716_bits() const { if (auto* d = direct()) return d->unknown_716; return uint32_t((memory::IsHandle(address + 716) ? uint32_t(memory::OwnerLoad(address + 716, 4)) : guest::LoadU32(address + 716))); }
  void set_unknown_716_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_716 = v; return; } if (memory::IsHandle(address + 716)) memory::OwnerStore(address + 716, 4, v); else guest::StoreU32(address + 716, v); }
  uint32_t unknown_716() const { if (auto* d = direct()) return d->unknown_716; return uint32_t((memory::IsHandle(address + 716) ? uint32_t(memory::OwnerLoad(address + 716, 4)) : guest::LoadU32(address + 716))); }
  void set_unknown_716(uint32_t v) const { if (auto* d = direct()) { d->unknown_716 = v; return; } if (memory::IsHandle(address + 716)) memory::OwnerStore(address + 716, 4, v); else guest::StoreU32(address + 716, v); }
  // the power graph record's +16 (the dealer: at most 2000)
  uint32_t unknown_720_address() const { return address + 720; }
  uint32_t unknown_720_bits() const { if (auto* d = direct()) return d->unknown_720; return uint32_t((memory::IsHandle(address + 720) ? uint32_t(memory::OwnerLoad(address + 720, 4)) : guest::LoadU32(address + 720))); }
  void set_unknown_720_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_720 = v; return; } if (memory::IsHandle(address + 720)) memory::OwnerStore(address + 720, 4, v); else guest::StoreU32(address + 720, v); }
  uint32_t unknown_720() const { if (auto* d = direct()) return d->unknown_720; return uint32_t((memory::IsHandle(address + 720) ? uint32_t(memory::OwnerLoad(address + 720, 4)) : guest::LoadU32(address + 720))); }
  void set_unknown_720(uint32_t v) const { if (auto* d = direct()) { d->unknown_720 = v; return; } if (memory::IsHandle(address + 720)) memory::OwnerStore(address + 720, 4, v); else guest::StoreU32(address + 720, v); }
  // the power graph record's +12 (the dealer: at most 1700)
  uint32_t unknown_724_address() const { return address + 724; }
  uint32_t unknown_724_bits() const { if (auto* d = direct()) return d->unknown_724; return uint32_t((memory::IsHandle(address + 724) ? uint32_t(memory::OwnerLoad(address + 724, 4)) : guest::LoadU32(address + 724))); }
  void set_unknown_724_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_724 = v; return; } if (memory::IsHandle(address + 724)) memory::OwnerStore(address + 724, 4, v); else guest::StoreU32(address + 724, v); }
  uint32_t unknown_724() const { if (auto* d = direct()) return d->unknown_724; return uint32_t((memory::IsHandle(address + 724) ? uint32_t(memory::OwnerLoad(address + 724, 4)) : guest::LoadU32(address + 724))); }
  void set_unknown_724(uint32_t v) const { if (auto* d = direct()) { d->unknown_724 = v; return; } if (memory::IsHandle(address + 724)) memory::OwnerStore(address + 724, 4, v); else guest::StoreU32(address + 724, v); }
  uint32_t price_address() const { return address + 744; }
  uint32_t price_bits() const { if (auto* d = direct()) return d->price; return uint32_t((memory::IsHandle(address + 744) ? uint32_t(memory::OwnerLoad(address + 744, 4)) : guest::LoadU32(address + 744))); }
  void set_price_bits(uint32_t v) const { if (auto* d = direct()) { d->price = v; return; } if (memory::IsHandle(address + 744)) memory::OwnerStore(address + 744, 4, v); else guest::StoreU32(address + 744, v); }
  uint32_t price() const { if (auto* d = direct()) return d->price; return uint32_t((memory::IsHandle(address + 744) ? uint32_t(memory::OwnerLoad(address + 744, 4)) : guest::LoadU32(address + 744))); }
  void set_price(uint32_t v) const { if (auto* d = direct()) { d->price = v; return; } if (memory::IsHandle(address + 744)) memory::OwnerStore(address + 744, 4, v); else guest::StoreU32(address + 744, v); }
  // string 1934 + it (the maker's name)
  uint32_t maker_address() const { return address + 769; }
  uint8_t maker_bits() const { if (auto* d = direct()) return d->maker; return uint8_t((memory::IsHandle(address + 769) ? uint8_t(memory::OwnerLoad(address + 769, 1)) : guest::LoadU8(address + 769))); }
  void set_maker_bits(uint8_t v) const { if (auto* d = direct()) { d->maker = v; return; } if (memory::IsHandle(address + 769)) memory::OwnerStore(address + 769, 1, v); else guest::StoreU8(address + 769, v); }
  uint8_t maker() const { if (auto* d = direct()) return d->maker; return uint8_t((memory::IsHandle(address + 769) ? uint8_t(memory::OwnerLoad(address + 769, 1)) : guest::LoadU8(address + 769))); }
  void set_maker(uint8_t v) const { if (auto* d = direct()) { d->maker = v; return; } if (memory::IsHandle(address + 769)) memory::OwnerStore(address + 769, 1, v); else guest::StoreU8(address + 769, v); }
  // the stats record's +0 (string 421 + it)
  uint32_t unknown_770_address() const { return address + 770; }
  uint8_t unknown_770_bits() const { if (auto* d = direct()) return d->unknown_770; return uint8_t((memory::IsHandle(address + 770) ? uint8_t(memory::OwnerLoad(address + 770, 1)) : guest::LoadU8(address + 770))); }
  void set_unknown_770_bits(uint8_t v) const { if (auto* d = direct()) { d->unknown_770 = v; return; } if (memory::IsHandle(address + 770)) memory::OwnerStore(address + 770, 1, v); else guest::StoreU8(address + 770, v); }
  uint8_t unknown_770() const { if (auto* d = direct()) return d->unknown_770; return uint8_t((memory::IsHandle(address + 770) ? uint8_t(memory::OwnerLoad(address + 770, 1)) : guest::LoadU8(address + 770))); }
  void set_unknown_770(uint8_t v) const { if (auto* d = direct()) { d->unknown_770 = v; return; } if (memory::IsHandle(address + 770)) memory::OwnerStore(address + 770, 1, v); else guest::StoreU8(address + 770, v); }
  // 0 list 0, 1 list 1, others not listed
  uint32_t list_kind_address() const { return address + 771; }
  uint8_t list_kind_bits() const { if (auto* d = direct()) return d->list_kind; return uint8_t((memory::IsHandle(address + 771) ? uint8_t(memory::OwnerLoad(address + 771, 1)) : guest::LoadU8(address + 771))); }
  void set_list_kind_bits(uint8_t v) const { if (auto* d = direct()) { d->list_kind = v; return; } if (memory::IsHandle(address + 771)) memory::OwnerStore(address + 771, 1, v); else guest::StoreU8(address + 771, v); }
  uint8_t list_kind() const { if (auto* d = direct()) return d->list_kind; return uint8_t((memory::IsHandle(address + 771) ? uint8_t(memory::OwnerLoad(address + 771, 1)) : guest::LoadU8(address + 771))); }
  void set_list_kind(uint8_t v) const { if (auto* d = direct()) { d->list_kind = v; return; } if (memory::IsHandle(address + 771)) memory::OwnerStore(address + 771, 1, v); else guest::StoreU8(address + 771, v); }
  // 13 records of 20 bytes: +0 raised (u16), +4 the hue (u16), +6 the saturation, +7 the value (bytes)
  uint32_t color_records_address() const { return address + 8116; }
  uint8_t* color_records() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 8116 : guest::Pointer(address + 8116); }
  uint32_t color_count_address() const { return address + 8376; }
  uint32_t color_count_bits() const { if (auto* d = direct()) return d->color_count; return uint32_t((memory::IsHandle(address + 8376) ? uint32_t(memory::OwnerLoad(address + 8376, 4)) : guest::LoadU32(address + 8376))); }
  void set_color_count_bits(uint32_t v) const { if (auto* d = direct()) { d->color_count = v; return; } if (memory::IsHandle(address + 8376)) memory::OwnerStore(address + 8376, 4, v); else guest::StoreU32(address + 8376, v); }
  uint32_t color_count() const { if (auto* d = direct()) return d->color_count; return uint32_t((memory::IsHandle(address + 8376) ? uint32_t(memory::OwnerLoad(address + 8376, 4)) : guest::LoadU32(address + 8376))); }
  void set_color_count(uint32_t v) const { if (auto* d = direct()) { d->color_count = v; return; } if (memory::IsHandle(address + 8376)) memory::OwnerStore(address + 8376, 4, v); else guest::StoreU32(address + 8376, v); }
  uint32_t car_id_address() const { return address + 8432; }
  uint32_t car_id_bits() const { if (auto* d = direct()) return d->car_id; return uint32_t((memory::IsHandle(address + 8432) ? uint32_t(memory::OwnerLoad(address + 8432, 4)) : guest::LoadU32(address + 8432))); }
  void set_car_id_bits(uint32_t v) const { if (auto* d = direct()) { d->car_id = v; return; } if (memory::IsHandle(address + 8432)) memory::OwnerStore(address + 8432, 4, v); else guest::StoreU32(address + 8432, v); }
  uint32_t car_id() const { if (auto* d = direct()) return d->car_id; return uint32_t((memory::IsHandle(address + 8432) ? uint32_t(memory::OwnerLoad(address + 8432, 4)) : guest::LoadU32(address + 8432))); }
  void set_car_id(uint32_t v) const { if (auto* d = direct()) { d->car_id = v; return; } if (memory::IsHandle(address + 8432)) memory::OwnerStore(address + 8432, 4, v); else guest::StoreU32(address + 8432, v); }
  uint32_t color_address() const { return address + 8436; }
  uint32_t color_bits() const { if (auto* d = direct()) return d->color; return uint32_t((memory::IsHandle(address + 8436) ? uint32_t(memory::OwnerLoad(address + 8436, 4)) : guest::LoadU32(address + 8436))); }
  void set_color_bits(uint32_t v) const { if (auto* d = direct()) { d->color = v; return; } if (memory::IsHandle(address + 8436)) memory::OwnerStore(address + 8436, 4, v); else guest::StoreU32(address + 8436, v); }
  uint32_t color() const { if (auto* d = direct()) return d->color; return uint32_t((memory::IsHandle(address + 8436) ? uint32_t(memory::OwnerLoad(address + 8436, 4)) : guest::LoadU32(address + 8436))); }
  void set_color(uint32_t v) const { if (auto* d = direct()) { d->color = v; return; } if (memory::IsHandle(address + 8436)) memory::OwnerStore(address + 8436, 4, v); else guest::StoreU32(address + 8436, v); }
};

// embedded; measure: ColorPanel::SetSwatches.r4
struct CarListColorList : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  // 13 pointers to CarListColor
  uint32_t records_address() const { return address + 0; }
  uint32_t count() const { return guest::LoadU32(address + 52); }
  void set_count(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t count_address() const { return address + 52; }
};

// allocated; measure: ColorPanel::SetSwatch.r5, new<32>@PanelSetBase::SetUpColors
#pragma pack(push, 1)
struct CarListColorData {
  uint32_t red = 0;  // +0
  uint32_t green = 0;  // +4
  uint32_t blue = 0;  // +8
  uint8_t pad_12[4] = {};
  float hue = 0;  // +16
  float saturation = 0;  // +20
  float value = 0;  // +24
  uint8_t pad_28[4] = {};
};
#pragma pack(pop)
static_assert(sizeof(CarListColorData) == 32);
inline constexpr memory::LayoutField kCarListColorFields[] = {{0, 4}, {4, 4}, {8, 4}, {16, 4}, {20, 4}, {24, 4}};
inline constexpr memory::Layout kCarListColorLayout = {kCarListColorFields, 6};

struct CarListColor : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CarListColorData* data() const { return static_cast<CarListColorData*>(memory::HandleObject(address)); }
  static CarListColor New() {
    return CarListColor(memory::MakeHandle(new CarListColorData(), sizeof(CarListColorData), "CarListColor", &kCarListColorLayout));
  }
  template <uint32_t Stride = kSize>
  static CarListColor NewArray(uint32_t count) {
    static_assert(sizeof(CarListColorData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarListColorData*>(::operator new(sizeof(CarListColorData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarListColorData();
    return CarListColor(memory::MakeHandle(p, sizeof(CarListColorData) * count, "CarListColor", &kCarListColorLayout, sizeof(CarListColorData)));
  }
  CarListColor at(uint32_t i) const { return CarListColor(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarListColorData* direct() const { return static_cast<CarListColorData*>(memory::HandleOwn(address, &kCarListColorLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarListColorLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarListColorLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t red_address() const { return address + 0; }
  uint32_t red_bits() const { if (auto* d = direct()) return d->red; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_red_bits(uint32_t v) const { if (auto* d = direct()) { d->red = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t red() const { if (auto* d = direct()) return d->red; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_red(uint32_t v) const { if (auto* d = direct()) { d->red = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t green_address() const { return address + 4; }
  uint32_t green_bits() const { if (auto* d = direct()) return d->green; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_green_bits(uint32_t v) const { if (auto* d = direct()) { d->green = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t green() const { if (auto* d = direct()) return d->green; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_green(uint32_t v) const { if (auto* d = direct()) { d->green = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t blue_address() const { return address + 8; }
  uint32_t blue_bits() const { if (auto* d = direct()) return d->blue; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_blue_bits(uint32_t v) const { if (auto* d = direct()) { d->blue = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t blue() const { if (auto* d = direct()) return d->blue; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_blue(uint32_t v) const { if (auto* d = direct()) { d->blue = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t hue_address() const { return address + 16; }
  uint32_t hue_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->hue); return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_hue_bits(uint32_t v) const { if (auto* d = direct()) { d->hue = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  float hue() const { return std::bit_cast<float>(hue_bits()); }
  void set_hue(float v) const { set_hue_bits(std::bit_cast<uint32_t>(v)); }
  // 0 to 1
  uint32_t saturation_address() const { return address + 20; }
  uint32_t saturation_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->saturation); return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_saturation_bits(uint32_t v) const { if (auto* d = direct()) { d->saturation = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  float saturation() const { return std::bit_cast<float>(saturation_bits()); }
  void set_saturation(float v) const { set_saturation_bits(std::bit_cast<uint32_t>(v)); }
  // 0 to 1
  uint32_t value_address() const { return address + 24; }
  uint32_t value_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->value); return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_value_bits(uint32_t v) const { if (auto* d = direct()) { d->value = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  float value() const { return std::bit_cast<float>(value_bits()); }
  void set_value(float v) const { set_value_bits(std::bit_cast<uint32_t>(v)); }
  // red and green read as one doubleword (Color::HsvToRgb's copy)
  uint32_t red_green_address() const { return address + 0; }
  uint64_t red_green_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kCarListColorLayout, 0, 8)); return uint64_t((memory::IsHandle(address + 0) ? uint64_t(memory::OwnerLoad(address + 0, 8)) : guest::LoadU64(address + 0))); }
  void set_red_green_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarListColorLayout, 0, 8, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 8, v); else guest::StoreU64(address + 0, v); }
  uint64_t red_green() const { return uint64_t(red_green_bits()); }
  void set_red_green(uint64_t v) const { set_red_green_bits(uint64_t(v)); }
  // blue and +12 read as one doubleword (Color::HsvToRgb's copy)
  uint32_t blue_unknown_12_address() const { return address + 8; }
  uint64_t blue_unknown_12_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kCarListColorLayout, 8, 8)); return uint64_t((memory::IsHandle(address + 8) ? uint64_t(memory::OwnerLoad(address + 8, 8)) : guest::LoadU64(address + 8))); }
  void set_blue_unknown_12_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarListColorLayout, 8, 8, v); return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 8, v); else guest::StoreU64(address + 8, v); }
  uint64_t blue_unknown_12() const { return uint64_t(blue_unknown_12_bits()); }
  void set_blue_unknown_12(uint64_t v) const { set_blue_unknown_12_bits(uint64_t(v)); }
};

// embedded; measure: PanelSetBase.+1892
struct CarListBackObject : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t value() const { return guest::LoadU32(address + 4); }
  void set_value(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t value_address() const { return address + 4; }
  // copied to +4 by BackPage7
  uint32_t page7_value() const { return guest::LoadU32(address + 8); }
  void set_page7_value(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t page7_value_address() const { return address + 8; }
  // copied to +4 by BackPage8
  uint32_t page8_value() const { return guest::LoadU32(address + 12); }
  void set_page8_value(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t page8_value_address() const { return address + 12; }
};

// embedded; measure: CarPanel::SetStats.r4, Panel6C::SetStats.r4
struct CarListStats : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // the spec's +770 (string 421 + it)
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // the spec's +709 set (string 426 + it)
  uint32_t turbo() const { return guest::LoadU32(address + 4); }
  void set_turbo(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t turbo_address() const { return address + 4; }
  // the spec's +712, shown scaled
  uint32_t unknown_8() const { return guest::LoadU32(address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  // the spec's +648
  uint32_t displacement() const { return guest::LoadU32(address + 12); }
  void set_displacement(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t displacement_address() const { return address + 12; }
  // the spec's +652 as an integer
  uint32_t unknown_16() const { return guest::LoadU32(address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  // the spec's +656 as an integer
  uint32_t unknown_20() const { return guest::LoadU32(address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  // the spec's +660, shown scaled
  float unknown_24() const { return guest::LoadFloat(address + 24); }
  void set_unknown_24(float v) const { guest::StoreFloat(address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  // the spec's +664 as an integer
  uint32_t unknown_28() const { return guest::LoadU32(address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
};

// embedded; measure: PanelGroupBase, Type6Screens@+40948@+9708, Type7Screens@+30608@+9708
struct PanelGroupBase : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  static constexpr uint32_t kVtable = 0x82010974;
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // NodeBase's (0); a panel group's movie (PanelGroup::PanelGroup: 4)
  uint32_t node_word() const { return guest::LoadU32(address + 4); }
  void set_node_word(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t node_word_address() const { return address + 4; }
  // -1 in the constructor
  uint32_t unknown_8() const { return guest::LoadU32(address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  // the caption's node set (PanelGroup::SetCaption)
  uint32_t text_node_set() const { return guest::LoadU32(address + 12); }
  void set_text_node_set(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t text_node_set_address() const { return address + 12; }
  // -1 in the constructor
  uint32_t unknown_16() const { return guest::LoadU32(address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  // -1 in the constructor
  uint32_t unknown_20() const { return guest::LoadU32(address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  // -1 in the constructor
  uint32_t unknown_24() const { return guest::LoadU32(address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  // -1 in the constructor
  uint32_t unknown_28() const { return guest::LoadU32(address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  // the caption's node (-1 none)
  uint32_t caption_node() const { return guest::LoadU32(address + 32); }
  void set_caption_node(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t caption_node_address() const { return address + 32; }
};

// embedded; measure: PanelPart
#pragma pack(push, 1)
struct PanelPartData {
  uint8_t pad_0[4] = {};
  uint32_t group = 0;  // +4
};
#pragma pack(pop)
static_assert(sizeof(PanelPartData) == 8);
inline constexpr memory::LayoutField kPanelPartFields[] = {{4, 4}};
inline constexpr memory::Layout kPanelPartLayout = {kPanelPartFields, 1};

struct PanelPart : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  PanelPartData* data() const { return static_cast<PanelPartData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static PanelPart New() {
    return PanelPart(memory::MakeHandle(new PanelPartData(), sizeof(PanelPartData), "PanelPart", &kPanelPartLayout));
  }
  template <uint32_t Stride = kSize>
  static PanelPart NewArray(uint32_t count) {
    static_assert(sizeof(PanelPartData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<PanelPartData*>(::operator new(sizeof(PanelPartData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) PanelPartData();
    return PanelPart(memory::MakeHandle(p, sizeof(PanelPartData) * count, "PanelPart", &kPanelPartLayout, sizeof(PanelPartData)));
  }
  PanelPart at(uint32_t i) const { return PanelPart(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  PanelPartData* direct() const { return static_cast<PanelPartData*>(memory::HandleOwn(address, &kPanelPartLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kPanelPartLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kPanelPartLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t group_address() const { return address + 4; }
  uint32_t group_bits() const { if (auto* d = direct()) return d->group; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_group_bits(uint32_t v) const { if (auto* d = direct()) { d->group = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  View group() const { return View(group_bits()); }
  void set_group(uint32_t v) const { set_group_bits(v); }
};

// embedded; measure: TabPanel, PanelFBase, Panel6F, Panel7F
#pragma pack(push, 1)
struct TabPanelData {
  uint32_t vtable = 0;  // +0
  uint32_t group = 0;  // +4
  uint32_t tab = 0;  // +8
  uint32_t tab0_enabled = 0;  // +12
  uint32_t tab1_enabled = 0;  // +16
  uint32_t tab2_enabled = 0;  // +20
  uint32_t node_set = 0;  // +24
  uint32_t texts_node = 0;  // +28
  uint32_t tabs_node = 0;  // +32
  uint32_t buttons_node = 0;  // +36
  uint32_t cursor = 0;  // +40
  uint32_t tab0_text = 0;  // +44
  uint32_t tab1_text = 0;  // +48
  uint32_t tab2_text = 0;  // +52
  uint8_t pad_56[28] = {};
  uint32_t buttons = 0;  // +84
  uint32_t tab_word_a = 0;  // +88
  uint32_t tab_word_b = 0;  // +92
  uint32_t tab_anims = 0;  // +96
  uint32_t scroll_down = 0;  // +100
  uint32_t scroll_up = 0;  // +104
  uint32_t unknown_108 = 0;  // +108
  uint32_t unknown_112 = 0;  // +112
  uint32_t show_anim = 0;  // +116
  uint32_t move_anim = 0;  // +120
  uint32_t loop_a = 0;  // +124
  uint32_t loop_b = 0;  // +128
  uint32_t loop_c = 0;  // +132
  uint32_t loop_d = 0;  // +136
  uint32_t two_players = 0;  // +140
};
#pragma pack(pop)
static_assert(sizeof(TabPanelData) == 144);
inline constexpr memory::LayoutField kTabPanelFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {84, 4}, {88, 4}, {92, 4}, {96, 4}, {100, 4}, {104, 4}, {108, 4}, {112, 4}, {116, 4}, {120, 4}, {124, 4}, {128, 4}, {132, 4}, {136, 4}, {140, 4}};
inline constexpr memory::Layout kTabPanelLayout = {kTabPanelFields, 29};

struct TabPanel : View {
  using View::View;
  static constexpr uint32_t kSize = 144;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  TabPanelData* data() const { return static_cast<TabPanelData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static TabPanel New() {
    return TabPanel(memory::MakeHandle(new TabPanelData(), sizeof(TabPanelData), "TabPanel", &kTabPanelLayout));
  }
  template <uint32_t Stride = kSize>
  static TabPanel NewArray(uint32_t count) {
    static_assert(sizeof(TabPanelData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<TabPanelData*>(::operator new(sizeof(TabPanelData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) TabPanelData();
    return TabPanel(memory::MakeHandle(p, sizeof(TabPanelData) * count, "TabPanel", &kTabPanelLayout, sizeof(TabPanelData)));
  }
  TabPanel at(uint32_t i) const { return TabPanel(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  TabPanelData* direct() const { return static_cast<TabPanelData*>(memory::HandleOwn(address, &kTabPanelLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kTabPanelLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTabPanelLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0x82010B5C (NodeBase's) from PanelFBase, 0x82010974 from Panel6F and Panel7F
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // its panel group
  uint32_t group_address() const { return address + 4; }
  uint32_t group_bits() const { if (auto* d = direct()) return d->group; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_group_bits(uint32_t v) const { if (auto* d = direct()) { d->group = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  View group() const { return View(group_bits()); }
  void set_group(uint32_t v) const { set_group_bits(v); }
  uint32_t tab_address() const { return address + 8; }
  uint32_t tab_bits() const { if (auto* d = direct()) return d->tab; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_tab_bits(uint32_t v) const { if (auto* d = direct()) { d->tab = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t tab() const { if (auto* d = direct()) return d->tab; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_tab(uint32_t v) const { if (auto* d = direct()) { d->tab = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  // 1 the tab's animation plays; set by step 0 from the lists' counts
  uint32_t tab0_enabled_address() const { return address + 12; }
  uint32_t tab0_enabled_bits() const { if (auto* d = direct()) return d->tab0_enabled; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_tab0_enabled_bits(uint32_t v) const { if (auto* d = direct()) { d->tab0_enabled = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t tab0_enabled() const { if (auto* d = direct()) return d->tab0_enabled; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_tab0_enabled(uint32_t v) const { if (auto* d = direct()) { d->tab0_enabled = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t tab1_enabled_address() const { return address + 16; }
  uint32_t tab1_enabled_bits() const { if (auto* d = direct()) return d->tab1_enabled; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_tab1_enabled_bits(uint32_t v) const { if (auto* d = direct()) { d->tab1_enabled = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t tab1_enabled() const { if (auto* d = direct()) return d->tab1_enabled; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_tab1_enabled(uint32_t v) const { if (auto* d = direct()) { d->tab1_enabled = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t tab2_enabled_address() const { return address + 20; }
  uint32_t tab2_enabled_bits() const { if (auto* d = direct()) return d->tab2_enabled; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_tab2_enabled_bits(uint32_t v) const { if (auto* d = direct()) { d->tab2_enabled = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t tab2_enabled() const { if (auto* d = direct()) return d->tab2_enabled; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_tab2_enabled(uint32_t v) const { if (auto* d = direct()) { d->tab2_enabled = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t node_set_address() const { return address + 24; }
  uint32_t node_set_bits() const { if (auto* d = direct()) return d->node_set; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_node_set_bits(uint32_t v) const { if (auto* d = direct()) { d->node_set = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  uint32_t node_set() const { if (auto* d = direct()) return d->node_set; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_node_set(uint32_t v) const { if (auto* d = direct()) { d->node_set = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  uint32_t texts_node_address() const { return address + 28; }
  uint32_t texts_node_bits() const { if (auto* d = direct()) return d->texts_node; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_texts_node_bits(uint32_t v) const { if (auto* d = direct()) { d->texts_node = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  uint32_t texts_node() const { if (auto* d = direct()) return d->texts_node; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_texts_node(uint32_t v) const { if (auto* d = direct()) { d->texts_node = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  uint32_t tabs_node_address() const { return address + 32; }
  uint32_t tabs_node_bits() const { if (auto* d = direct()) return d->tabs_node; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_tabs_node_bits(uint32_t v) const { if (auto* d = direct()) { d->tabs_node = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  uint32_t tabs_node() const { if (auto* d = direct()) return d->tabs_node; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_tabs_node(uint32_t v) const { if (auto* d = direct()) { d->tabs_node = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  uint32_t buttons_node_address() const { return address + 36; }
  uint32_t buttons_node_bits() const { if (auto* d = direct()) return d->buttons_node; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_buttons_node_bits(uint32_t v) const { if (auto* d = direct()) { d->buttons_node = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  uint32_t buttons_node() const { if (auto* d = direct()) return d->buttons_node; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_buttons_node(uint32_t v) const { if (auto* d = direct()) { d->buttons_node = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  // the cursor's child of (node_set, texts_node)
  uint32_t cursor_address() const { return address + 40; }
  uint32_t cursor_bits() const { if (auto* d = direct()) return d->cursor; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_cursor_bits(uint32_t v) const { if (auto* d = direct()) { d->cursor = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  uint32_t cursor() const { if (auto* d = direct()) return d->cursor; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_cursor(uint32_t v) const { if (auto* d = direct()) { d->cursor = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  // the tabs' text children
  uint32_t tab0_text_address() const { return address + 44; }
  uint32_t tab0_text_bits() const { if (auto* d = direct()) return d->tab0_text; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_tab0_text_bits(uint32_t v) const { if (auto* d = direct()) { d->tab0_text = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  uint32_t tab0_text() const { if (auto* d = direct()) return d->tab0_text; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_tab0_text(uint32_t v) const { if (auto* d = direct()) { d->tab0_text = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  uint32_t tab1_text_address() const { return address + 48; }
  uint32_t tab1_text_bits() const { if (auto* d = direct()) return d->tab1_text; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_tab1_text_bits(uint32_t v) const { if (auto* d = direct()) { d->tab1_text = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  uint32_t tab1_text() const { if (auto* d = direct()) return d->tab1_text; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_tab1_text(uint32_t v) const { if (auto* d = direct()) { d->tab1_text = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  uint32_t tab2_text_address() const { return address + 52; }
  uint32_t tab2_text_bits() const { if (auto* d = direct()) return d->tab2_text; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_tab2_text_bits(uint32_t v) const { if (auto* d = direct()) { d->tab2_text = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t tab2_text() const { if (auto* d = direct()) return d->tab2_text; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_tab2_text(uint32_t v) const { if (auto* d = direct()) { d->tab2_text = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  // the first of six button children
  uint32_t buttons_address() const { return address + 84; }
  uint32_t buttons_bits() const { if (auto* d = direct()) return d->buttons; return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(address + 84))); }
  void set_buttons_bits(uint32_t v) const { if (auto* d = direct()) { d->buttons = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(address + 84, v); }
  uint32_t buttons() const { if (auto* d = direct()) return d->buttons; return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(address + 84))); }
  void set_buttons(uint32_t v) const { if (auto* d = direct()) { d->buttons = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(address + 84, v); }
  // a child of tabs_node whose +108 is the tab
  uint32_t tab_word_a_address() const { return address + 88; }
  uint32_t tab_word_a_bits() const { if (auto* d = direct()) return d->tab_word_a; return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(address + 88))); }
  void set_tab_word_a_bits(uint32_t v) const { if (auto* d = direct()) { d->tab_word_a = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(address + 88, v); }
  uint32_t tab_word_a() const { if (auto* d = direct()) return d->tab_word_a; return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(address + 88))); }
  void set_tab_word_a(uint32_t v) const { if (auto* d = direct()) { d->tab_word_a = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(address + 88, v); }
  // another
  uint32_t tab_word_b_address() const { return address + 92; }
  uint32_t tab_word_b_bits() const { if (auto* d = direct()) return d->tab_word_b; return uint32_t((memory::IsHandle(address + 92) ? uint32_t(memory::OwnerLoad(address + 92, 4)) : guest::LoadU32(address + 92))); }
  void set_tab_word_b_bits(uint32_t v) const { if (auto* d = direct()) { d->tab_word_b = v; return; } if (memory::IsHandle(address + 92)) memory::OwnerStore(address + 92, 4, v); else guest::StoreU32(address + 92, v); }
  uint32_t tab_word_b() const { if (auto* d = direct()) return d->tab_word_b; return uint32_t((memory::IsHandle(address + 92) ? uint32_t(memory::OwnerLoad(address + 92, 4)) : guest::LoadU32(address + 92))); }
  void set_tab_word_b(uint32_t v) const { if (auto* d = direct()) { d->tab_word_b = v; return; } if (memory::IsHandle(address + 92)) memory::OwnerStore(address + 92, 4, v); else guest::StoreU32(address + 92, v); }
  // the first tab's animation (+ the tab)
  uint32_t tab_anims_address() const { return address + 96; }
  uint32_t tab_anims_bits() const { if (auto* d = direct()) return d->tab_anims; return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(address + 96))); }
  void set_tab_anims_bits(uint32_t v) const { if (auto* d = direct()) { d->tab_anims = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(address + 96, v); }
  uint32_t tab_anims() const { if (auto* d = direct()) return d->tab_anims; return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(address + 96))); }
  void set_tab_anims(uint32_t v) const { if (auto* d = direct()) { d->tab_anims = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(address + 96, v); }
  // played for a move of -1
  uint32_t scroll_down_address() const { return address + 100; }
  uint32_t scroll_down_bits() const { if (auto* d = direct()) return d->scroll_down; return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(address + 100))); }
  void set_scroll_down_bits(uint32_t v) const { if (auto* d = direct()) { d->scroll_down = v; return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(address + 100, v); }
  uint32_t scroll_down() const { if (auto* d = direct()) return d->scroll_down; return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(address + 100))); }
  void set_scroll_down(uint32_t v) const { if (auto* d = direct()) { d->scroll_down = v; return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(address + 100, v); }
  // played for a move of 1
  uint32_t scroll_up_address() const { return address + 104; }
  uint32_t scroll_up_bits() const { if (auto* d = direct()) return d->scroll_up; return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(address + 104))); }
  void set_scroll_up_bits(uint32_t v) const { if (auto* d = direct()) { d->scroll_up = v; return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(address + 104, v); }
  uint32_t scroll_up() const { if (auto* d = direct()) return d->scroll_up; return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(address + 104))); }
  void set_scroll_up(uint32_t v) const { if (auto* d = direct()) { d->scroll_up = v; return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(address + 104, v); }
  // played and finished by Move for a tab not enabled
  uint32_t unknown_108_address() const { return address + 108; }
  uint32_t unknown_108_bits() const { if (auto* d = direct()) return d->unknown_108; return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(address + 108))); }
  void set_unknown_108_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_108 = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(address + 108, v); }
  uint32_t unknown_108() const { if (auto* d = direct()) return d->unknown_108; return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(address + 108))); }
  void set_unknown_108(uint32_t v) const { if (auto* d = direct()) { d->unknown_108 = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(address + 108, v); }
  // played reversed and finished by Move for a tab not enabled
  uint32_t unknown_112_address() const { return address + 112; }
  uint32_t unknown_112_bits() const { if (auto* d = direct()) return d->unknown_112; return uint32_t((memory::IsHandle(address + 112) ? uint32_t(memory::OwnerLoad(address + 112, 4)) : guest::LoadU32(address + 112))); }
  void set_unknown_112_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_112 = v; return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 4, v); else guest::StoreU32(address + 112, v); }
  uint32_t unknown_112() const { if (auto* d = direct()) return d->unknown_112; return uint32_t((memory::IsHandle(address + 112) ? uint32_t(memory::OwnerLoad(address + 112, 4)) : guest::LoadU32(address + 112))); }
  void set_unknown_112(uint32_t v) const { if (auto* d = direct()) { d->unknown_112 = v; return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 4, v); else guest::StoreU32(address + 112, v); }
  uint32_t show_anim_address() const { return address + 116; }
  uint32_t show_anim_bits() const { if (auto* d = direct()) return d->show_anim; return uint32_t((memory::IsHandle(address + 116) ? uint32_t(memory::OwnerLoad(address + 116, 4)) : guest::LoadU32(address + 116))); }
  void set_show_anim_bits(uint32_t v) const { if (auto* d = direct()) { d->show_anim = v; return; } if (memory::IsHandle(address + 116)) memory::OwnerStore(address + 116, 4, v); else guest::StoreU32(address + 116, v); }
  uint32_t show_anim() const { if (auto* d = direct()) return d->show_anim; return uint32_t((memory::IsHandle(address + 116) ? uint32_t(memory::OwnerLoad(address + 116, 4)) : guest::LoadU32(address + 116))); }
  void set_show_anim(uint32_t v) const { if (auto* d = direct()) { d->show_anim = v; return; } if (memory::IsHandle(address + 116)) memory::OwnerStore(address + 116, 4, v); else guest::StoreU32(address + 116, v); }
  uint32_t move_anim_address() const { return address + 120; }
  uint32_t move_anim_bits() const { if (auto* d = direct()) return d->move_anim; return uint32_t((memory::IsHandle(address + 120) ? uint32_t(memory::OwnerLoad(address + 120, 4)) : guest::LoadU32(address + 120))); }
  void set_move_anim_bits(uint32_t v) const { if (auto* d = direct()) { d->move_anim = v; return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 4, v); else guest::StoreU32(address + 120, v); }
  uint32_t move_anim() const { if (auto* d = direct()) return d->move_anim; return uint32_t((memory::IsHandle(address + 120) ? uint32_t(memory::OwnerLoad(address + 120, 4)) : guest::LoadU32(address + 120))); }
  void set_move_anim(uint32_t v) const { if (auto* d = direct()) { d->move_anim = v; return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 4, v); else guest::StoreU32(address + 120, v); }
  // looped by Show
  uint32_t loop_a_address() const { return address + 124; }
  uint32_t loop_a_bits() const { if (auto* d = direct()) return d->loop_a; return uint32_t((memory::IsHandle(address + 124) ? uint32_t(memory::OwnerLoad(address + 124, 4)) : guest::LoadU32(address + 124))); }
  void set_loop_a_bits(uint32_t v) const { if (auto* d = direct()) { d->loop_a = v; return; } if (memory::IsHandle(address + 124)) memory::OwnerStore(address + 124, 4, v); else guest::StoreU32(address + 124, v); }
  uint32_t loop_a() const { if (auto* d = direct()) return d->loop_a; return uint32_t((memory::IsHandle(address + 124) ? uint32_t(memory::OwnerLoad(address + 124, 4)) : guest::LoadU32(address + 124))); }
  void set_loop_a(uint32_t v) const { if (auto* d = direct()) { d->loop_a = v; return; } if (memory::IsHandle(address + 124)) memory::OwnerStore(address + 124, 4, v); else guest::StoreU32(address + 124, v); }
  // looped by Select
  uint32_t loop_b_address() const { return address + 128; }
  uint32_t loop_b_bits() const { if (auto* d = direct()) return d->loop_b; return uint32_t((memory::IsHandle(address + 128) ? uint32_t(memory::OwnerLoad(address + 128, 4)) : guest::LoadU32(address + 128))); }
  void set_loop_b_bits(uint32_t v) const { if (auto* d = direct()) { d->loop_b = v; return; } if (memory::IsHandle(address + 128)) memory::OwnerStore(address + 128, 4, v); else guest::StoreU32(address + 128, v); }
  uint32_t loop_b() const { if (auto* d = direct()) return d->loop_b; return uint32_t((memory::IsHandle(address + 128) ? uint32_t(memory::OwnerLoad(address + 128, 4)) : guest::LoadU32(address + 128))); }
  void set_loop_b(uint32_t v) const { if (auto* d = direct()) { d->loop_b = v; return; } if (memory::IsHandle(address + 128)) memory::OwnerStore(address + 128, 4, v); else guest::StoreU32(address + 128, v); }
  // looped by Show and Select
  uint32_t loop_c_address() const { return address + 132; }
  uint32_t loop_c_bits() const { if (auto* d = direct()) return d->loop_c; return uint32_t((memory::IsHandle(address + 132) ? uint32_t(memory::OwnerLoad(address + 132, 4)) : guest::LoadU32(address + 132))); }
  void set_loop_c_bits(uint32_t v) const { if (auto* d = direct()) { d->loop_c = v; return; } if (memory::IsHandle(address + 132)) memory::OwnerStore(address + 132, 4, v); else guest::StoreU32(address + 132, v); }
  uint32_t loop_c() const { if (auto* d = direct()) return d->loop_c; return uint32_t((memory::IsHandle(address + 132) ? uint32_t(memory::OwnerLoad(address + 132, 4)) : guest::LoadU32(address + 132))); }
  void set_loop_c(uint32_t v) const { if (auto* d = direct()) { d->loop_c = v; return; } if (memory::IsHandle(address + 132)) memory::OwnerStore(address + 132, 4, v); else guest::StoreU32(address + 132, v); }
  // looped by Show
  uint32_t loop_d_address() const { return address + 136; }
  uint32_t loop_d_bits() const { if (auto* d = direct()) return d->loop_d; return uint32_t((memory::IsHandle(address + 136) ? uint32_t(memory::OwnerLoad(address + 136, 4)) : guest::LoadU32(address + 136))); }
  void set_loop_d_bits(uint32_t v) const { if (auto* d = direct()) { d->loop_d = v; return; } if (memory::IsHandle(address + 136)) memory::OwnerStore(address + 136, 4, v); else guest::StoreU32(address + 136, v); }
  uint32_t loop_d() const { if (auto* d = direct()) return d->loop_d; return uint32_t((memory::IsHandle(address + 136) ? uint32_t(memory::OwnerLoad(address + 136, 4)) : guest::LoadU32(address + 136))); }
  void set_loop_d(uint32_t v) const { if (auto* d = direct()) { d->loop_d = v; return; } if (memory::IsHandle(address + 136)) memory::OwnerStore(address + 136, 4, v); else guest::StoreU32(address + 136, v); }
  // the two-player word: no caption
  uint32_t two_players_address() const { return address + 140; }
  uint32_t two_players_bits() const { if (auto* d = direct()) return d->two_players; return uint32_t((memory::IsHandle(address + 140) ? uint32_t(memory::OwnerLoad(address + 140, 4)) : guest::LoadU32(address + 140))); }
  void set_two_players_bits(uint32_t v) const { if (auto* d = direct()) { d->two_players = v; return; } if (memory::IsHandle(address + 140)) memory::OwnerStore(address + 140, 4, v); else guest::StoreU32(address + 140, v); }
  uint32_t two_players() const { if (auto* d = direct()) return d->two_players; return uint32_t((memory::IsHandle(address + 140) ? uint32_t(memory::OwnerLoad(address + 140, 4)) : guest::LoadU32(address + 140))); }
  void set_two_players(uint32_t v) const { if (auto* d = direct()) { d->two_players = v; return; } if (memory::IsHandle(address + 140)) memory::OwnerStore(address + 140, 4, v); else guest::StoreU32(address + 140, v); }
};

// embedded; measure: RowPanel, MenuList.+1900, PanelE, PanelEBase, PanelSetBase::ToTabs.r3.+1900
#pragma pack(push, 1)
struct RowPanelData {
  uint32_t group = 0;  // +0
  uint32_t row_count = 0;  // +4
  uint32_t row = 0;  // +8
  uint32_t node_set = 0;  // +12
  uint32_t node = 0;  // +16
  uint8_t row_nodes[40] = {};  // +20, guest order
  uint32_t spare_row_node = 0;  // +60
  uint32_t unknown_64 = 0;  // +64
  uint32_t unknown_68 = 0;  // +68
  uint32_t unknown_72 = 0;  // +72
  uint32_t show_anim = 0;  // +76
  uint32_t show_anim_alt = 0;  // +80
  uint32_t show_first_anim = 0;  // +84
  uint32_t hide_anim = 0;  // +88
  uint32_t row_anims = 0;  // +92
  uint32_t scroll_up = 0;  // +96
  uint32_t scroll_down = 0;  // +100
  uint32_t arrow_up = 0;  // +104
  uint32_t arrow_down = 0;  // +108
  uint32_t loop_a = 0;  // +112
  uint32_t loop_b = 0;  // +116
  uint32_t logo = 0;  // +120
  uint32_t text_maker = 0;  // +124
  uint32_t text_name = 0;  // +128
  uint32_t text_third = 0;  // +132
  uint32_t unknown_136 = 0;  // +136
  uint32_t unknown_140 = 0;  // +140
  uint32_t cursor = 0;  // +144
  uint32_t arrows = 0;  // +148
  uint32_t show_alt = 0;  // +152
  uint32_t two_players = 0;  // +156
};
#pragma pack(pop)
static_assert(sizeof(RowPanelData) == 160);
inline constexpr memory::LayoutField kRowPanelFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {60, 4}, {64, 4}, {68, 4}, {72, 4}, {76, 4}, {80, 4}, {84, 4}, {88, 4}, {92, 4}, {96, 4}, {100, 4}, {104, 4}, {108, 4}, {112, 4}, {116, 4}, {120, 4}, {124, 4}, {128, 4}, {132, 4}, {136, 4}, {140, 4}, {144, 4}, {148, 4}, {152, 4}, {156, 4}};
inline constexpr memory::Layout kRowPanelLayout = {kRowPanelFields, 30};

struct RowPanel : View {
  using View::View;
  static constexpr uint32_t kSize = 160;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  RowPanelData* data() const { return static_cast<RowPanelData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static RowPanel New() {
    return RowPanel(memory::MakeHandle(new RowPanelData(), sizeof(RowPanelData), "RowPanel", &kRowPanelLayout));
  }
  template <uint32_t Stride = kSize>
  static RowPanel NewArray(uint32_t count) {
    static_assert(sizeof(RowPanelData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<RowPanelData*>(::operator new(sizeof(RowPanelData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) RowPanelData();
    return RowPanel(memory::MakeHandle(p, sizeof(RowPanelData) * count, "RowPanel", &kRowPanelLayout, sizeof(RowPanelData)));
  }
  RowPanel at(uint32_t i) const { return RowPanel(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  RowPanelData* direct() const { return static_cast<RowPanelData*>(memory::HandleOwn(address, &kRowPanelLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kRowPanelLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kRowPanelLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // its panel group
  uint32_t group_address() const { return address + 0; }
  uint32_t group_bits() const { if (auto* d = direct()) return d->group; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_group_bits(uint32_t v) const { if (auto* d = direct()) { d->group = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  View group() const { return View(group_bits()); }
  void set_group(uint32_t v) const { set_group_bits(v); }
  // the rows shown
  uint32_t row_count_address() const { return address + 4; }
  uint32_t row_count_bits() const { if (auto* d = direct()) return d->row_count; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_row_count_bits(uint32_t v) const { if (auto* d = direct()) { d->row_count = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t row_count() const { if (auto* d = direct()) return d->row_count; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_row_count(uint32_t v) const { if (auto* d = direct()) { d->row_count = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  // the highlighted row (-1 none)
  uint32_t row_address() const { return address + 8; }
  uint32_t row_bits() const { if (auto* d = direct()) return d->row; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_row_bits(uint32_t v) const { if (auto* d = direct()) { d->row = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t row() const { if (auto* d = direct()) return d->row; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_row(uint32_t v) const { if (auto* d = direct()) { d->row = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t node_set_address() const { return address + 12; }
  uint32_t node_set_bits() const { if (auto* d = direct()) return d->node_set; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_node_set_bits(uint32_t v) const { if (auto* d = direct()) { d->node_set = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t node_set() const { if (auto* d = direct()) return d->node_set; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_node_set(uint32_t v) const { if (auto* d = direct()) { d->node_set = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t node_address() const { return address + 16; }
  uint32_t node_bits() const { if (auto* d = direct()) return d->node; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_node_bits(uint32_t v) const { if (auto* d = direct()) { d->node = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t node() const { if (auto* d = direct()) return d->node; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_node(uint32_t v) const { if (auto* d = direct()) { d->node = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  // the rows' child indices
  uint32_t row_nodes_address() const { return address + 20; }
  uint8_t* row_nodes() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 20 : guest::Pointer(address + 20); }
  // the row SetRow fills for row -1 (scrolled in)
  uint32_t spare_row_node_address() const { return address + 60; }
  uint32_t spare_row_node_bits() const { if (auto* d = direct()) return d->spare_row_node; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_spare_row_node_bits(uint32_t v) const { if (auto* d = direct()) { d->spare_row_node = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  uint32_t spare_row_node() const { if (auto* d = direct()) return d->spare_row_node; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_spare_row_node(uint32_t v) const { if (auto* d = direct()) { d->spare_row_node = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  // 5 in PanelE::PanelE
  uint32_t unknown_64_address() const { return address + 64; }
  uint32_t unknown_64_bits() const { if (auto* d = direct()) return d->unknown_64; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_unknown_64_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_64 = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  uint32_t unknown_64() const { if (auto* d = direct()) return d->unknown_64; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_unknown_64(uint32_t v) const { if (auto* d = direct()) { d->unknown_64 = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  // 15 in PanelE::PanelE
  uint32_t unknown_68_address() const { return address + 68; }
  uint32_t unknown_68_bits() const { if (auto* d = direct()) return d->unknown_68; return uint32_t((memory::IsHandle(address + 68) ? uint32_t(memory::OwnerLoad(address + 68, 4)) : guest::LoadU32(address + 68))); }
  void set_unknown_68_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_68 = v; return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 4, v); else guest::StoreU32(address + 68, v); }
  uint32_t unknown_68() const { if (auto* d = direct()) return d->unknown_68; return uint32_t((memory::IsHandle(address + 68) ? uint32_t(memory::OwnerLoad(address + 68, 4)) : guest::LoadU32(address + 68))); }
  void set_unknown_68(uint32_t v) const { if (auto* d = direct()) { d->unknown_68 = v; return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 4, v); else guest::StoreU32(address + 68, v); }
  // 16 in PanelE::PanelE
  uint32_t unknown_72_address() const { return address + 72; }
  uint32_t unknown_72_bits() const { if (auto* d = direct()) return d->unknown_72; return uint32_t((memory::IsHandle(address + 72) ? uint32_t(memory::OwnerLoad(address + 72, 4)) : guest::LoadU32(address + 72))); }
  void set_unknown_72_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_72 = v; return; } if (memory::IsHandle(address + 72)) memory::OwnerStore(address + 72, 4, v); else guest::StoreU32(address + 72, v); }
  uint32_t unknown_72() const { if (auto* d = direct()) return d->unknown_72; return uint32_t((memory::IsHandle(address + 72) ? uint32_t(memory::OwnerLoad(address + 72, 4)) : guest::LoadU32(address + 72))); }
  void set_unknown_72(uint32_t v) const { if (auto* d = direct()) { d->unknown_72 = v; return; } if (memory::IsHandle(address + 72)) memory::OwnerStore(address + 72, 4, v); else guest::StoreU32(address + 72, v); }
  uint32_t show_anim_address() const { return address + 76; }
  uint32_t show_anim_bits() const { if (auto* d = direct()) return d->show_anim; return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(address + 76))); }
  void set_show_anim_bits(uint32_t v) const { if (auto* d = direct()) { d->show_anim = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(address + 76, v); }
  uint32_t show_anim() const { if (auto* d = direct()) return d->show_anim; return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(address + 76))); }
  void set_show_anim(uint32_t v) const { if (auto* d = direct()) { d->show_anim = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(address + 76, v); }
  // played instead with show_alt
  uint32_t show_anim_alt_address() const { return address + 80; }
  uint32_t show_anim_alt_bits() const { if (auto* d = direct()) return d->show_anim_alt; return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(address + 80))); }
  void set_show_anim_alt_bits(uint32_t v) const { if (auto* d = direct()) { d->show_anim_alt = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(address + 80, v); }
  uint32_t show_anim_alt() const { if (auto* d = direct()) return d->show_anim_alt; return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(address + 80))); }
  void set_show_anim_alt(uint32_t v) const { if (auto* d = direct()) { d->show_anim_alt = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(address + 80, v); }
  uint32_t show_first_anim_address() const { return address + 84; }
  uint32_t show_first_anim_bits() const { if (auto* d = direct()) return d->show_first_anim; return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(address + 84))); }
  void set_show_first_anim_bits(uint32_t v) const { if (auto* d = direct()) { d->show_first_anim = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(address + 84, v); }
  uint32_t show_first_anim() const { if (auto* d = direct()) return d->show_first_anim; return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(address + 84))); }
  void set_show_first_anim(uint32_t v) const { if (auto* d = direct()) { d->show_first_anim = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(address + 84, v); }
  uint32_t hide_anim_address() const { return address + 88; }
  uint32_t hide_anim_bits() const { if (auto* d = direct()) return d->hide_anim; return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(address + 88))); }
  void set_hide_anim_bits(uint32_t v) const { if (auto* d = direct()) { d->hide_anim = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(address + 88, v); }
  uint32_t hide_anim() const { if (auto* d = direct()) return d->hide_anim; return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(address + 88))); }
  void set_hide_anim(uint32_t v) const { if (auto* d = direct()) { d->hide_anim = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(address + 88, v); }
  // the first row's animation (+ the row)
  uint32_t row_anims_address() const { return address + 92; }
  uint32_t row_anims_bits() const { if (auto* d = direct()) return d->row_anims; return uint32_t((memory::IsHandle(address + 92) ? uint32_t(memory::OwnerLoad(address + 92, 4)) : guest::LoadU32(address + 92))); }
  void set_row_anims_bits(uint32_t v) const { if (auto* d = direct()) { d->row_anims = v; return; } if (memory::IsHandle(address + 92)) memory::OwnerStore(address + 92, 4, v); else guest::StoreU32(address + 92, v); }
  uint32_t row_anims() const { if (auto* d = direct()) return d->row_anims; return uint32_t((memory::IsHandle(address + 92) ? uint32_t(memory::OwnerLoad(address + 92, 4)) : guest::LoadU32(address + 92))); }
  void set_row_anims(uint32_t v) const { if (auto* d = direct()) { d->row_anims = v; return; } if (memory::IsHandle(address + 92)) memory::OwnerStore(address + 92, 4, v); else guest::StoreU32(address + 92, v); }
  uint32_t scroll_up_address() const { return address + 96; }
  uint32_t scroll_up_bits() const { if (auto* d = direct()) return d->scroll_up; return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(address + 96))); }
  void set_scroll_up_bits(uint32_t v) const { if (auto* d = direct()) { d->scroll_up = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(address + 96, v); }
  uint32_t scroll_up() const { if (auto* d = direct()) return d->scroll_up; return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(address + 96))); }
  void set_scroll_up(uint32_t v) const { if (auto* d = direct()) { d->scroll_up = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(address + 96, v); }
  uint32_t scroll_down_address() const { return address + 100; }
  uint32_t scroll_down_bits() const { if (auto* d = direct()) return d->scroll_down; return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(address + 100))); }
  void set_scroll_down_bits(uint32_t v) const { if (auto* d = direct()) { d->scroll_down = v; return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(address + 100, v); }
  uint32_t scroll_down() const { if (auto* d = direct()) return d->scroll_down; return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(address + 100))); }
  void set_scroll_down(uint32_t v) const { if (auto* d = direct()) { d->scroll_down = v; return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(address + 100, v); }
  uint32_t arrow_up_address() const { return address + 104; }
  uint32_t arrow_up_bits() const { if (auto* d = direct()) return d->arrow_up; return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(address + 104))); }
  void set_arrow_up_bits(uint32_t v) const { if (auto* d = direct()) { d->arrow_up = v; return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(address + 104, v); }
  uint32_t arrow_up() const { if (auto* d = direct()) return d->arrow_up; return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(address + 104))); }
  void set_arrow_up(uint32_t v) const { if (auto* d = direct()) { d->arrow_up = v; return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(address + 104, v); }
  uint32_t arrow_down_address() const { return address + 108; }
  uint32_t arrow_down_bits() const { if (auto* d = direct()) return d->arrow_down; return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(address + 108))); }
  void set_arrow_down_bits(uint32_t v) const { if (auto* d = direct()) { d->arrow_down = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(address + 108, v); }
  uint32_t arrow_down() const { if (auto* d = direct()) return d->arrow_down; return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(address + 108))); }
  void set_arrow_down(uint32_t v) const { if (auto* d = direct()) { d->arrow_down = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(address + 108, v); }
  uint32_t loop_a_address() const { return address + 112; }
  uint32_t loop_a_bits() const { if (auto* d = direct()) return d->loop_a; return uint32_t((memory::IsHandle(address + 112) ? uint32_t(memory::OwnerLoad(address + 112, 4)) : guest::LoadU32(address + 112))); }
  void set_loop_a_bits(uint32_t v) const { if (auto* d = direct()) { d->loop_a = v; return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 4, v); else guest::StoreU32(address + 112, v); }
  uint32_t loop_a() const { if (auto* d = direct()) return d->loop_a; return uint32_t((memory::IsHandle(address + 112) ? uint32_t(memory::OwnerLoad(address + 112, 4)) : guest::LoadU32(address + 112))); }
  void set_loop_a(uint32_t v) const { if (auto* d = direct()) { d->loop_a = v; return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 4, v); else guest::StoreU32(address + 112, v); }
  uint32_t loop_b_address() const { return address + 116; }
  uint32_t loop_b_bits() const { if (auto* d = direct()) return d->loop_b; return uint32_t((memory::IsHandle(address + 116) ? uint32_t(memory::OwnerLoad(address + 116, 4)) : guest::LoadU32(address + 116))); }
  void set_loop_b_bits(uint32_t v) const { if (auto* d = direct()) { d->loop_b = v; return; } if (memory::IsHandle(address + 116)) memory::OwnerStore(address + 116, 4, v); else guest::StoreU32(address + 116, v); }
  uint32_t loop_b() const { if (auto* d = direct()) return d->loop_b; return uint32_t((memory::IsHandle(address + 116) ? uint32_t(memory::OwnerLoad(address + 116, 4)) : guest::LoadU32(address + 116))); }
  void set_loop_b(uint32_t v) const { if (auto* d = direct()) { d->loop_b = v; return; } if (memory::IsHandle(address + 116)) memory::OwnerStore(address + 116, 4, v); else guest::StoreU32(address + 116, v); }
  // a row's logo child (its +108 the frame)
  uint32_t logo_address() const { return address + 120; }
  uint32_t logo_bits() const { if (auto* d = direct()) return d->logo; return uint32_t((memory::IsHandle(address + 120) ? uint32_t(memory::OwnerLoad(address + 120, 4)) : guest::LoadU32(address + 120))); }
  void set_logo_bits(uint32_t v) const { if (auto* d = direct()) { d->logo = v; return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 4, v); else guest::StoreU32(address + 120, v); }
  uint32_t logo() const { if (auto* d = direct()) return d->logo; return uint32_t((memory::IsHandle(address + 120) ? uint32_t(memory::OwnerLoad(address + 120, 4)) : guest::LoadU32(address + 120))); }
  void set_logo(uint32_t v) const { if (auto* d = direct()) { d->logo = v; return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 4, v); else guest::StoreU32(address + 120, v); }
  // a row's text child: string 1934 + the maker
  uint32_t text_maker_address() const { return address + 124; }
  uint32_t text_maker_bits() const { if (auto* d = direct()) return d->text_maker; return uint32_t((memory::IsHandle(address + 124) ? uint32_t(memory::OwnerLoad(address + 124, 4)) : guest::LoadU32(address + 124))); }
  void set_text_maker_bits(uint32_t v) const { if (auto* d = direct()) { d->text_maker = v; return; } if (memory::IsHandle(address + 124)) memory::OwnerStore(address + 124, 4, v); else guest::StoreU32(address + 124, v); }
  uint32_t text_maker() const { if (auto* d = direct()) return d->text_maker; return uint32_t((memory::IsHandle(address + 124) ? uint32_t(memory::OwnerLoad(address + 124, 4)) : guest::LoadU32(address + 124))); }
  void set_text_maker(uint32_t v) const { if (auto* d = direct()) { d->text_maker = v; return; } if (memory::IsHandle(address + 124)) memory::OwnerStore(address + 124, 4, v); else guest::StoreU32(address + 124, v); }
  // string 1646 + the car's id
  uint32_t text_name_address() const { return address + 128; }
  uint32_t text_name_bits() const { if (auto* d = direct()) return d->text_name; return uint32_t((memory::IsHandle(address + 128) ? uint32_t(memory::OwnerLoad(address + 128, 4)) : guest::LoadU32(address + 128))); }
  void set_text_name_bits(uint32_t v) const { if (auto* d = direct()) { d->text_name = v; return; } if (memory::IsHandle(address + 128)) memory::OwnerStore(address + 128, 4, v); else guest::StoreU32(address + 128, v); }
  uint32_t text_name() const { if (auto* d = direct()) return d->text_name; return uint32_t((memory::IsHandle(address + 128) ? uint32_t(memory::OwnerLoad(address + 128, 4)) : guest::LoadU32(address + 128))); }
  void set_text_name(uint32_t v) const { if (auto* d = direct()) { d->text_name = v; return; } if (memory::IsHandle(address + 128)) memory::OwnerStore(address + 128, 4, v); else guest::StoreU32(address + 128, v); }
  // string 1718 + the car's id
  uint32_t text_third_address() const { return address + 132; }
  uint32_t text_third_bits() const { if (auto* d = direct()) return d->text_third; return uint32_t((memory::IsHandle(address + 132) ? uint32_t(memory::OwnerLoad(address + 132, 4)) : guest::LoadU32(address + 132))); }
  void set_text_third_bits(uint32_t v) const { if (auto* d = direct()) { d->text_third = v; return; } if (memory::IsHandle(address + 132)) memory::OwnerStore(address + 132, 4, v); else guest::StoreU32(address + 132, v); }
  uint32_t text_third() const { if (auto* d = direct()) return d->text_third; return uint32_t((memory::IsHandle(address + 132) ? uint32_t(memory::OwnerLoad(address + 132, 4)) : guest::LoadU32(address + 132))); }
  void set_text_third(uint32_t v) const { if (auto* d = direct()) { d->text_third = v; return; } if (memory::IsHandle(address + 132)) memory::OwnerStore(address + 132, 4, v); else guest::StoreU32(address + 132, v); }
  // 8 in PanelE::PanelE (after the texts' 5 to 7)
  uint32_t unknown_136_address() const { return address + 136; }
  uint32_t unknown_136_bits() const { if (auto* d = direct()) return d->unknown_136; return uint32_t((memory::IsHandle(address + 136) ? uint32_t(memory::OwnerLoad(address + 136, 4)) : guest::LoadU32(address + 136))); }
  void set_unknown_136_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_136 = v; return; } if (memory::IsHandle(address + 136)) memory::OwnerStore(address + 136, 4, v); else guest::StoreU32(address + 136, v); }
  uint32_t unknown_136() const { if (auto* d = direct()) return d->unknown_136; return uint32_t((memory::IsHandle(address + 136) ? uint32_t(memory::OwnerLoad(address + 136, 4)) : guest::LoadU32(address + 136))); }
  void set_unknown_136(uint32_t v) const { if (auto* d = direct()) { d->unknown_136 = v; return; } if (memory::IsHandle(address + 136)) memory::OwnerStore(address + 136, 4, v); else guest::StoreU32(address + 136, v); }
  // -1 in PanelE::PanelE
  uint32_t unknown_140_address() const { return address + 140; }
  uint32_t unknown_140_bits() const { if (auto* d = direct()) return d->unknown_140; return uint32_t((memory::IsHandle(address + 140) ? uint32_t(memory::OwnerLoad(address + 140, 4)) : guest::LoadU32(address + 140))); }
  void set_unknown_140_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_140 = v; return; } if (memory::IsHandle(address + 140)) memory::OwnerStore(address + 140, 4, v); else guest::StoreU32(address + 140, v); }
  uint32_t unknown_140() const { if (auto* d = direct()) return d->unknown_140; return uint32_t((memory::IsHandle(address + 140) ? uint32_t(memory::OwnerLoad(address + 140, 4)) : guest::LoadU32(address + 140))); }
  void set_unknown_140(uint32_t v) const { if (auto* d = direct()) { d->unknown_140 = v; return; } if (memory::IsHandle(address + 140)) memory::OwnerStore(address + 140, 4, v); else guest::StoreU32(address + 140, v); }
  // the more-rows marker child
  uint32_t cursor_address() const { return address + 144; }
  uint32_t cursor_bits() const { if (auto* d = direct()) return d->cursor; return uint32_t((memory::IsHandle(address + 144) ? uint32_t(memory::OwnerLoad(address + 144, 4)) : guest::LoadU32(address + 144))); }
  void set_cursor_bits(uint32_t v) const { if (auto* d = direct()) { d->cursor = v; return; } if (memory::IsHandle(address + 144)) memory::OwnerStore(address + 144, 4, v); else guest::StoreU32(address + 144, v); }
  uint32_t cursor() const { if (auto* d = direct()) return d->cursor; return uint32_t((memory::IsHandle(address + 144) ? uint32_t(memory::OwnerLoad(address + 144, 4)) : guest::LoadU32(address + 144))); }
  void set_cursor(uint32_t v) const { if (auto* d = direct()) { d->cursor = v; return; } if (memory::IsHandle(address + 144)) memory::OwnerStore(address + 144, 4, v); else guest::StoreU32(address + 144, v); }
  // the up arrow's child, the down's after it
  uint32_t arrows_address() const { return address + 148; }
  uint32_t arrows_bits() const { if (auto* d = direct()) return d->arrows; return uint32_t((memory::IsHandle(address + 148) ? uint32_t(memory::OwnerLoad(address + 148, 4)) : guest::LoadU32(address + 148))); }
  void set_arrows_bits(uint32_t v) const { if (auto* d = direct()) { d->arrows = v; return; } if (memory::IsHandle(address + 148)) memory::OwnerStore(address + 148, 4, v); else guest::StoreU32(address + 148, v); }
  uint32_t arrows() const { if (auto* d = direct()) return d->arrows; return uint32_t((memory::IsHandle(address + 148) ? uint32_t(memory::OwnerLoad(address + 148, 4)) : guest::LoadU32(address + 148))); }
  void set_arrows(uint32_t v) const { if (auto* d = direct()) { d->arrows = v; return; } if (memory::IsHandle(address + 148)) memory::OwnerStore(address + 148, 4, v); else guest::StoreU32(address + 148, v); }
  // show with show_anim_alt (ToTabs and PanelSet::Open set it)
  uint32_t show_alt_address() const { return address + 152; }
  uint32_t show_alt_bits() const { if (auto* d = direct()) return d->show_alt; return uint32_t((memory::IsHandle(address + 152) ? uint32_t(memory::OwnerLoad(address + 152, 4)) : guest::LoadU32(address + 152))); }
  void set_show_alt_bits(uint32_t v) const { if (auto* d = direct()) { d->show_alt = v; return; } if (memory::IsHandle(address + 152)) memory::OwnerStore(address + 152, 4, v); else guest::StoreU32(address + 152, v); }
  uint32_t show_alt() const { if (auto* d = direct()) return d->show_alt; return uint32_t((memory::IsHandle(address + 152) ? uint32_t(memory::OwnerLoad(address + 152, 4)) : guest::LoadU32(address + 152))); }
  void set_show_alt(uint32_t v) const { if (auto* d = direct()) { d->show_alt = v; return; } if (memory::IsHandle(address + 152)) memory::OwnerStore(address + 152, 4, v); else guest::StoreU32(address + 152, v); }
  // the two-player word: no caption
  uint32_t two_players_address() const { return address + 156; }
  uint32_t two_players_bits() const { if (auto* d = direct()) return d->two_players; return uint32_t((memory::IsHandle(address + 156) ? uint32_t(memory::OwnerLoad(address + 156, 4)) : guest::LoadU32(address + 156))); }
  void set_two_players_bits(uint32_t v) const { if (auto* d = direct()) { d->two_players = v; return; } if (memory::IsHandle(address + 156)) memory::OwnerStore(address + 156, 4, v); else guest::StoreU32(address + 156, v); }
  uint32_t two_players() const { if (auto* d = direct()) return d->two_players; return uint32_t((memory::IsHandle(address + 156) ? uint32_t(memory::OwnerLoad(address + 156, 4)) : guest::LoadU32(address + 156))); }
  void set_two_players(uint32_t v) const { if (auto* d = direct()) { d->two_players = v; return; } if (memory::IsHandle(address + 156)) memory::OwnerStore(address + 156, 4, v); else guest::StoreU32(address + 156, v); }
};

// embedded; measure: ColorPanel, PanelDBase, Panel6D, Panel7D
#pragma pack(push, 1)
struct ColorPanelData {
  uint32_t vtable = 0;  // +0
  uint32_t group = 0;  // +4
  uint32_t node_set = 0;  // +8
  uint32_t node = 0;  // +12
  uint32_t show_anim = 0;  // +16
  uint32_t hide_anim = 0;  // +20
  uint32_t item_anims = 0;  // +24
  uint32_t scroll_back = 0;  // +28
  uint32_t scroll_on = 0;  // +32
  uint32_t swatches_on = 0;  // +36
  uint32_t swatches_back = 0;  // +40
  uint32_t unknown_44 = 0;  // +44
  uint32_t unknown_48 = 0;  // +48
  uint32_t arrow_a = 0;  // +52
  uint32_t arrow_b = 0;  // +56
  uint32_t arrows_node = 0;  // +60
  uint32_t cursor = 0;  // +64
  uint32_t swatches = 0;  // +68
  uint32_t arrow_up_node = 0;  // +72
  uint32_t arrow_down_node = 0;  // +76
  uint32_t swatch_color = 0;  // +80
  uint8_t pad_84[4] = {};
  uint32_t two_players = 0;  // +88
};
#pragma pack(pop)
static_assert(sizeof(ColorPanelData) == 92);
inline constexpr memory::LayoutField kColorPanelFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}, {68, 4}, {72, 4}, {76, 4}, {80, 4}, {88, 4}};
inline constexpr memory::Layout kColorPanelLayout = {kColorPanelFields, 22};

struct ColorPanel : View {
  using View::View;
  static constexpr uint32_t kSize = 92;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  ColorPanelData* data() const { return static_cast<ColorPanelData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static ColorPanel New() {
    return ColorPanel(memory::MakeHandle(new ColorPanelData(), sizeof(ColorPanelData), "ColorPanel", &kColorPanelLayout));
  }
  template <uint32_t Stride = kSize>
  static ColorPanel NewArray(uint32_t count) {
    static_assert(sizeof(ColorPanelData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ColorPanelData*>(::operator new(sizeof(ColorPanelData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ColorPanelData();
    return ColorPanel(memory::MakeHandle(p, sizeof(ColorPanelData) * count, "ColorPanel", &kColorPanelLayout, sizeof(ColorPanelData)));
  }
  ColorPanel at(uint32_t i) const { return ColorPanel(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  ColorPanelData* direct() const { return static_cast<ColorPanelData*>(memory::HandleOwn(address, &kColorPanelLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kColorPanelLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kColorPanelLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0x82010B5C (NodeBase's) from PanelDBase, 0x82010974 from Panel6D and Panel7D
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // its panel group
  uint32_t group_address() const { return address + 4; }
  uint32_t group_bits() const { if (auto* d = direct()) return d->group; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_group_bits(uint32_t v) const { if (auto* d = direct()) { d->group = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  View group() const { return View(group_bits()); }
  void set_group(uint32_t v) const { set_group_bits(v); }
  uint32_t node_set_address() const { return address + 8; }
  uint32_t node_set_bits() const { if (auto* d = direct()) return d->node_set; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_node_set_bits(uint32_t v) const { if (auto* d = direct()) { d->node_set = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t node_set() const { if (auto* d = direct()) return d->node_set; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_node_set(uint32_t v) const { if (auto* d = direct()) { d->node_set = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t node_address() const { return address + 12; }
  uint32_t node_bits() const { if (auto* d = direct()) return d->node; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_node_bits(uint32_t v) const { if (auto* d = direct()) { d->node = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t node() const { if (auto* d = direct()) return d->node; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_node(uint32_t v) const { if (auto* d = direct()) { d->node = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t show_anim_address() const { return address + 16; }
  uint32_t show_anim_bits() const { if (auto* d = direct()) return d->show_anim; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_show_anim_bits(uint32_t v) const { if (auto* d = direct()) { d->show_anim = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t show_anim() const { if (auto* d = direct()) return d->show_anim; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_show_anim(uint32_t v) const { if (auto* d = direct()) { d->show_anim = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t hide_anim_address() const { return address + 20; }
  uint32_t hide_anim_bits() const { if (auto* d = direct()) return d->hide_anim; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_hide_anim_bits(uint32_t v) const { if (auto* d = direct()) { d->hide_anim = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t hide_anim() const { if (auto* d = direct()) return d->hide_anim; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_hide_anim(uint32_t v) const { if (auto* d = direct()) { d->hide_anim = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  // the first item's animation (+ the item)
  uint32_t item_anims_address() const { return address + 24; }
  uint32_t item_anims_bits() const { if (auto* d = direct()) return d->item_anims; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_item_anims_bits(uint32_t v) const { if (auto* d = direct()) { d->item_anims = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  uint32_t item_anims() const { if (auto* d = direct()) return d->item_anims; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_item_anims(uint32_t v) const { if (auto* d = direct()) { d->item_anims = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  // played for a step below 0
  uint32_t scroll_back_address() const { return address + 28; }
  uint32_t scroll_back_bits() const { if (auto* d = direct()) return d->scroll_back; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_scroll_back_bits(uint32_t v) const { if (auto* d = direct()) { d->scroll_back = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  uint32_t scroll_back() const { if (auto* d = direct()) return d->scroll_back; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_scroll_back(uint32_t v) const { if (auto* d = direct()) { d->scroll_back = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  // played for a step above 0
  uint32_t scroll_on_address() const { return address + 32; }
  uint32_t scroll_on_bits() const { if (auto* d = direct()) return d->scroll_on; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_scroll_on_bits(uint32_t v) const { if (auto* d = direct()) { d->scroll_on = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  uint32_t scroll_on() const { if (auto* d = direct()) return d->scroll_on; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_scroll_on(uint32_t v) const { if (auto* d = direct()) { d->scroll_on = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  // played for moves above 0
  uint32_t swatches_on_address() const { return address + 36; }
  uint32_t swatches_on_bits() const { if (auto* d = direct()) return d->swatches_on; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_swatches_on_bits(uint32_t v) const { if (auto* d = direct()) { d->swatches_on = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  uint32_t swatches_on() const { if (auto* d = direct()) return d->swatches_on; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_swatches_on(uint32_t v) const { if (auto* d = direct()) { d->swatches_on = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  // played for moves below 0
  uint32_t swatches_back_address() const { return address + 40; }
  uint32_t swatches_back_bits() const { if (auto* d = direct()) return d->swatches_back; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_swatches_back_bits(uint32_t v) const { if (auto* d = direct()) { d->swatches_back = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  uint32_t swatches_back() const { if (auto* d = direct()) return d->swatches_back; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_swatches_back(uint32_t v) const { if (auto* d = direct()) { d->swatches_back = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  // finished by Move
  uint32_t unknown_44_address() const { return address + 44; }
  uint32_t unknown_44_bits() const { if (auto* d = direct()) return d->unknown_44; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_unknown_44_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_44 = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  uint32_t unknown_44() const { if (auto* d = direct()) return d->unknown_44; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_unknown_44(uint32_t v) const { if (auto* d = direct()) { d->unknown_44 = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  // finished by Move
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_48_bits() const { if (auto* d = direct()) return d->unknown_48; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_unknown_48_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_48 = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  uint32_t unknown_48() const { if (auto* d = direct()) return d->unknown_48; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_unknown_48(uint32_t v) const { if (auto* d = direct()) { d->unknown_48 = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  uint32_t arrow_a_address() const { return address + 52; }
  uint32_t arrow_a_bits() const { if (auto* d = direct()) return d->arrow_a; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_arrow_a_bits(uint32_t v) const { if (auto* d = direct()) { d->arrow_a = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t arrow_a() const { if (auto* d = direct()) return d->arrow_a; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_arrow_a(uint32_t v) const { if (auto* d = direct()) { d->arrow_a = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t arrow_b_address() const { return address + 56; }
  uint32_t arrow_b_bits() const { if (auto* d = direct()) return d->arrow_b; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_arrow_b_bits(uint32_t v) const { if (auto* d = direct()) { d->arrow_b = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  uint32_t arrow_b() const { if (auto* d = direct()) return d->arrow_b; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_arrow_b(uint32_t v) const { if (auto* d = direct()) { d->arrow_b = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  uint32_t arrows_node_address() const { return address + 60; }
  uint32_t arrows_node_bits() const { if (auto* d = direct()) return d->arrows_node; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_arrows_node_bits(uint32_t v) const { if (auto* d = direct()) { d->arrows_node = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  uint32_t arrows_node() const { if (auto* d = direct()) return d->arrows_node; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_arrows_node(uint32_t v) const { if (auto* d = direct()) { d->arrows_node = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  uint32_t cursor_address() const { return address + 64; }
  uint32_t cursor_bits() const { if (auto* d = direct()) return d->cursor; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_cursor_bits(uint32_t v) const { if (auto* d = direct()) { d->cursor = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  uint32_t cursor() const { if (auto* d = direct()) return d->cursor; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_cursor(uint32_t v) const { if (auto* d = direct()) { d->cursor = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  // the swatches' child (its +100 the first swatch, +104 their count)
  uint32_t swatches_address() const { return address + 68; }
  uint32_t swatches_bits() const { if (auto* d = direct()) return d->swatches; return uint32_t((memory::IsHandle(address + 68) ? uint32_t(memory::OwnerLoad(address + 68, 4)) : guest::LoadU32(address + 68))); }
  void set_swatches_bits(uint32_t v) const { if (auto* d = direct()) { d->swatches = v; return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 4, v); else guest::StoreU32(address + 68, v); }
  uint32_t swatches() const { if (auto* d = direct()) return d->swatches; return uint32_t((memory::IsHandle(address + 68) ? uint32_t(memory::OwnerLoad(address + 68, 4)) : guest::LoadU32(address + 68))); }
  void set_swatches(uint32_t v) const { if (auto* d = direct()) { d->swatches = v; return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 4, v); else guest::StoreU32(address + 68, v); }
  uint32_t arrow_up_node_address() const { return address + 72; }
  uint32_t arrow_up_node_bits() const { if (auto* d = direct()) return d->arrow_up_node; return uint32_t((memory::IsHandle(address + 72) ? uint32_t(memory::OwnerLoad(address + 72, 4)) : guest::LoadU32(address + 72))); }
  void set_arrow_up_node_bits(uint32_t v) const { if (auto* d = direct()) { d->arrow_up_node = v; return; } if (memory::IsHandle(address + 72)) memory::OwnerStore(address + 72, 4, v); else guest::StoreU32(address + 72, v); }
  uint32_t arrow_up_node() const { if (auto* d = direct()) return d->arrow_up_node; return uint32_t((memory::IsHandle(address + 72) ? uint32_t(memory::OwnerLoad(address + 72, 4)) : guest::LoadU32(address + 72))); }
  void set_arrow_up_node(uint32_t v) const { if (auto* d = direct()) { d->arrow_up_node = v; return; } if (memory::IsHandle(address + 72)) memory::OwnerStore(address + 72, 4, v); else guest::StoreU32(address + 72, v); }
  uint32_t arrow_down_node_address() const { return address + 76; }
  uint32_t arrow_down_node_bits() const { if (auto* d = direct()) return d->arrow_down_node; return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(address + 76))); }
  void set_arrow_down_node_bits(uint32_t v) const { if (auto* d = direct()) { d->arrow_down_node = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(address + 76, v); }
  uint32_t arrow_down_node() const { if (auto* d = direct()) return d->arrow_down_node; return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(address + 76))); }
  void set_arrow_down_node(uint32_t v) const { if (auto* d = direct()) { d->arrow_down_node = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(address + 76, v); }
  // a swatch's color child (+56 to +64 the RGB)
  uint32_t swatch_color_address() const { return address + 80; }
  uint32_t swatch_color_bits() const { if (auto* d = direct()) return d->swatch_color; return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(address + 80))); }
  void set_swatch_color_bits(uint32_t v) const { if (auto* d = direct()) { d->swatch_color = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(address + 80, v); }
  uint32_t swatch_color() const { if (auto* d = direct()) return d->swatch_color; return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(address + 80))); }
  void set_swatch_color(uint32_t v) const { if (auto* d = direct()) { d->swatch_color = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(address + 80, v); }
  // the two-player word: no caption
  uint32_t two_players_address() const { return address + 88; }
  uint32_t two_players_bits() const { if (auto* d = direct()) return d->two_players; return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(address + 88))); }
  void set_two_players_bits(uint32_t v) const { if (auto* d = direct()) { d->two_players = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(address + 88, v); }
  uint32_t two_players() const { if (auto* d = direct()) return d->two_players; return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(address + 88))); }
  void set_two_players(uint32_t v) const { if (auto* d = direct()) { d->two_players = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(address + 88, v); }
};

// allocated; measure: CarPanel, CarPanelBase, Panel6C, Panel7C, PanelCBase
#pragma pack(push, 1)
struct CarPanelData {
  uint32_t vtable = 0;  // +0
  uint32_t shows_price = 0;  // +4
  uint32_t action = 0;  // +8
  uint32_t group = 0;  // +12
  uint32_t node_set = 0;  // +16
  uint32_t node = 0;  // +20
  uint32_t stats_node = 0;  // +24
  uint32_t price_node = 0;  // +28
  uint32_t buttons_node = 0;  // +32
  uint32_t name_node = 0;  // +36
  uint32_t show_anim_a = 0;  // +40
  uint32_t show_anim_b = 0;  // +44
  uint32_t hide_anim_a = 0;  // +48
  uint32_t hide_anim_b = 0;  // +52
  uint32_t show_again_anim = 0;  // +56
  uint32_t arrow_a = 0;  // +60
  uint32_t arrow_b = 0;  // +64
  uint8_t action_anims[40] = {};  // +68, guest order
  uint32_t scroll_back = 0;  // +108
  uint32_t scroll_on = 0;  // +112
  uint32_t stats_in = 0;  // +116
  uint32_t stats_out = 0;  // +120
  uint32_t loop_a = 0;  // +124
  uint32_t loop_b = 0;  // +128
  uint32_t text_maker = 0;  // +132
  uint32_t text_name = 0;  // +136
  uint32_t text_third = 0;  // +140
  uint32_t texts = 0;  // +144
  uint32_t unknown_148 = 0;  // +148
  uint32_t cursor = 0;  // +152
  uint32_t unknown_156 = 0;  // +156
  uint8_t stats_children[88] = {};  // +160, guest order
  uint8_t pad_248[12] = {};
  uint32_t price_child = 0;  // +260
  uint32_t price_text_a = 0;  // +264
  uint32_t price_text_b = 0;  // +268
  uint32_t buttons = 0;  // +272
  uint32_t text_a = 0;  // +276
  uint32_t text_b = 0;  // +280
  uint8_t pad_284[32] = {};
  uint32_t two_players = 0;  // +316
};
#pragma pack(pop)
static_assert(sizeof(CarPanelData) == 320);
inline constexpr memory::LayoutField kCarPanelFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}, {108, 4}, {112, 4}, {116, 4}, {120, 4}, {124, 4}, {128, 4}, {132, 4}, {136, 4}, {140, 4}, {144, 4}, {148, 4}, {152, 4}, {156, 4}, {260, 4}, {264, 4}, {268, 4}, {272, 4}, {276, 4}, {280, 4}, {316, 4}};
inline constexpr memory::Layout kCarPanelLayout = {kCarPanelFields, 37};

struct CarPanel : View {
  using View::View;
  static constexpr uint32_t kSize = 320;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  CarPanelData* data() const { return static_cast<CarPanelData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static CarPanel New() {
    return CarPanel(memory::MakeHandle(new CarPanelData(), sizeof(CarPanelData), "CarPanel", &kCarPanelLayout));
  }
  template <uint32_t Stride = kSize>
  static CarPanel NewArray(uint32_t count) {
    static_assert(sizeof(CarPanelData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarPanelData*>(::operator new(sizeof(CarPanelData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarPanelData();
    return CarPanel(memory::MakeHandle(p, sizeof(CarPanelData) * count, "CarPanel", &kCarPanelLayout, sizeof(CarPanelData)));
  }
  CarPanel at(uint32_t i) const { return CarPanel(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarPanelData* direct() const { return static_cast<CarPanelData*>(memory::HandleOwn(address, &kCarPanelLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarPanelLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarPanelLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = CarPanelVtable>
  T vtable() const { return T(vtable_bits()); }
  void set_vtable(const View& w) const { set_vtable_bits(w.address); }
  void set_vtable(uint32_t v) const { set_vtable_bits(v); }
  // ShowStats calls SetPrice
  uint32_t shows_price_address() const { return address + 4; }
  uint32_t shows_price_bits() const { if (auto* d = direct()) return d->shows_price; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_shows_price_bits(uint32_t v) const { if (auto* d = direct()) { d->shows_price = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t shows_price() const { if (auto* d = direct()) return d->shows_price; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_shows_price(uint32_t v) const { if (auto* d = direct()) { d->shows_price = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  // the highlighted action (-1 none)
  uint32_t action_address() const { return address + 8; }
  uint32_t action_bits() const { if (auto* d = direct()) return d->action; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_action_bits(uint32_t v) const { if (auto* d = direct()) { d->action = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t action() const { if (auto* d = direct()) return d->action; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_action(uint32_t v) const { if (auto* d = direct()) { d->action = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  // its panel group
  uint32_t group_address() const { return address + 12; }
  uint32_t group_bits() const { if (auto* d = direct()) return d->group; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_group_bits(uint32_t v) const { if (auto* d = direct()) { d->group = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  View group() const { return View(group_bits()); }
  void set_group(uint32_t v) const { set_group_bits(v); }
  uint32_t node_set_address() const { return address + 16; }
  uint32_t node_set_bits() const { if (auto* d = direct()) return d->node_set; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_node_set_bits(uint32_t v) const { if (auto* d = direct()) { d->node_set = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t node_set() const { if (auto* d = direct()) return d->node_set; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_node_set(uint32_t v) const { if (auto* d = direct()) { d->node_set = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t node_address() const { return address + 20; }
  uint32_t node_bits() const { if (auto* d = direct()) return d->node; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_node_bits(uint32_t v) const { if (auto* d = direct()) { d->node = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t node() const { if (auto* d = direct()) return d->node; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_node(uint32_t v) const { if (auto* d = direct()) { d->node = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t stats_node_address() const { return address + 24; }
  uint32_t stats_node_bits() const { if (auto* d = direct()) return d->stats_node; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_stats_node_bits(uint32_t v) const { if (auto* d = direct()) { d->stats_node = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  uint32_t stats_node() const { if (auto* d = direct()) return d->stats_node; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_stats_node(uint32_t v) const { if (auto* d = direct()) { d->stats_node = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  uint32_t price_node_address() const { return address + 28; }
  uint32_t price_node_bits() const { if (auto* d = direct()) return d->price_node; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_price_node_bits(uint32_t v) const { if (auto* d = direct()) { d->price_node = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  uint32_t price_node() const { if (auto* d = direct()) return d->price_node; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_price_node(uint32_t v) const { if (auto* d = direct()) { d->price_node = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  uint32_t buttons_node_address() const { return address + 32; }
  uint32_t buttons_node_bits() const { if (auto* d = direct()) return d->buttons_node; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_buttons_node_bits(uint32_t v) const { if (auto* d = direct()) { d->buttons_node = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  uint32_t buttons_node() const { if (auto* d = direct()) return d->buttons_node; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_buttons_node(uint32_t v) const { if (auto* d = direct()) { d->buttons_node = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  // CarPanelBase::SetCarName's node
  uint32_t name_node_address() const { return address + 36; }
  uint32_t name_node_bits() const { if (auto* d = direct()) return d->name_node; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_name_node_bits(uint32_t v) const { if (auto* d = direct()) { d->name_node = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  uint32_t name_node() const { if (auto* d = direct()) return d->name_node; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_name_node(uint32_t v) const { if (auto* d = direct()) { d->name_node = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  uint32_t show_anim_a_address() const { return address + 40; }
  uint32_t show_anim_a_bits() const { if (auto* d = direct()) return d->show_anim_a; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_show_anim_a_bits(uint32_t v) const { if (auto* d = direct()) { d->show_anim_a = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  uint32_t show_anim_a() const { if (auto* d = direct()) return d->show_anim_a; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_show_anim_a(uint32_t v) const { if (auto* d = direct()) { d->show_anim_a = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  uint32_t show_anim_b_address() const { return address + 44; }
  uint32_t show_anim_b_bits() const { if (auto* d = direct()) return d->show_anim_b; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_show_anim_b_bits(uint32_t v) const { if (auto* d = direct()) { d->show_anim_b = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  uint32_t show_anim_b() const { if (auto* d = direct()) return d->show_anim_b; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_show_anim_b(uint32_t v) const { if (auto* d = direct()) { d->show_anim_b = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  uint32_t hide_anim_a_address() const { return address + 48; }
  uint32_t hide_anim_a_bits() const { if (auto* d = direct()) return d->hide_anim_a; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_hide_anim_a_bits(uint32_t v) const { if (auto* d = direct()) { d->hide_anim_a = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  uint32_t hide_anim_a() const { if (auto* d = direct()) return d->hide_anim_a; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_hide_anim_a(uint32_t v) const { if (auto* d = direct()) { d->hide_anim_a = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  uint32_t hide_anim_b_address() const { return address + 52; }
  uint32_t hide_anim_b_bits() const { if (auto* d = direct()) return d->hide_anim_b; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_hide_anim_b_bits(uint32_t v) const { if (auto* d = direct()) { d->hide_anim_b = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t hide_anim_b() const { if (auto* d = direct()) return d->hide_anim_b; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_hide_anim_b(uint32_t v) const { if (auto* d = direct()) { d->hide_anim_b = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t show_again_anim_address() const { return address + 56; }
  uint32_t show_again_anim_bits() const { if (auto* d = direct()) return d->show_again_anim; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_show_again_anim_bits(uint32_t v) const { if (auto* d = direct()) { d->show_again_anim = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  uint32_t show_again_anim() const { if (auto* d = direct()) return d->show_again_anim; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_show_again_anim(uint32_t v) const { if (auto* d = direct()) { d->show_again_anim = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  uint32_t arrow_a_address() const { return address + 60; }
  uint32_t arrow_a_bits() const { if (auto* d = direct()) return d->arrow_a; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_arrow_a_bits(uint32_t v) const { if (auto* d = direct()) { d->arrow_a = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  uint32_t arrow_a() const { if (auto* d = direct()) return d->arrow_a; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_arrow_a(uint32_t v) const { if (auto* d = direct()) { d->arrow_a = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  uint32_t arrow_b_address() const { return address + 64; }
  uint32_t arrow_b_bits() const { if (auto* d = direct()) return d->arrow_b; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_arrow_b_bits(uint32_t v) const { if (auto* d = direct()) { d->arrow_b = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  uint32_t arrow_b() const { if (auto* d = direct()) return d->arrow_b; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_arrow_b(uint32_t v) const { if (auto* d = direct()) { d->arrow_b = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  // ten animations, one an action
  uint32_t action_anims_address() const { return address + 68; }
  uint8_t* action_anims() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 68 : guest::Pointer(address + 68); }
  // played for a move of -1
  uint32_t scroll_back_address() const { return address + 108; }
  uint32_t scroll_back_bits() const { if (auto* d = direct()) return d->scroll_back; return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(address + 108))); }
  void set_scroll_back_bits(uint32_t v) const { if (auto* d = direct()) { d->scroll_back = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(address + 108, v); }
  uint32_t scroll_back() const { if (auto* d = direct()) return d->scroll_back; return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(address + 108))); }
  void set_scroll_back(uint32_t v) const { if (auto* d = direct()) { d->scroll_back = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(address + 108, v); }
  // played for a move of 1
  uint32_t scroll_on_address() const { return address + 112; }
  uint32_t scroll_on_bits() const { if (auto* d = direct()) return d->scroll_on; return uint32_t((memory::IsHandle(address + 112) ? uint32_t(memory::OwnerLoad(address + 112, 4)) : guest::LoadU32(address + 112))); }
  void set_scroll_on_bits(uint32_t v) const { if (auto* d = direct()) { d->scroll_on = v; return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 4, v); else guest::StoreU32(address + 112, v); }
  uint32_t scroll_on() const { if (auto* d = direct()) return d->scroll_on; return uint32_t((memory::IsHandle(address + 112) ? uint32_t(memory::OwnerLoad(address + 112, 4)) : guest::LoadU32(address + 112))); }
  void set_scroll_on(uint32_t v) const { if (auto* d = direct()) { d->scroll_on = v; return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 4, v); else guest::StoreU32(address + 112, v); }
  uint32_t stats_in_address() const { return address + 116; }
  uint32_t stats_in_bits() const { if (auto* d = direct()) return d->stats_in; return uint32_t((memory::IsHandle(address + 116) ? uint32_t(memory::OwnerLoad(address + 116, 4)) : guest::LoadU32(address + 116))); }
  void set_stats_in_bits(uint32_t v) const { if (auto* d = direct()) { d->stats_in = v; return; } if (memory::IsHandle(address + 116)) memory::OwnerStore(address + 116, 4, v); else guest::StoreU32(address + 116, v); }
  uint32_t stats_in() const { if (auto* d = direct()) return d->stats_in; return uint32_t((memory::IsHandle(address + 116) ? uint32_t(memory::OwnerLoad(address + 116, 4)) : guest::LoadU32(address + 116))); }
  void set_stats_in(uint32_t v) const { if (auto* d = direct()) { d->stats_in = v; return; } if (memory::IsHandle(address + 116)) memory::OwnerStore(address + 116, 4, v); else guest::StoreU32(address + 116, v); }
  uint32_t stats_out_address() const { return address + 120; }
  uint32_t stats_out_bits() const { if (auto* d = direct()) return d->stats_out; return uint32_t((memory::IsHandle(address + 120) ? uint32_t(memory::OwnerLoad(address + 120, 4)) : guest::LoadU32(address + 120))); }
  void set_stats_out_bits(uint32_t v) const { if (auto* d = direct()) { d->stats_out = v; return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 4, v); else guest::StoreU32(address + 120, v); }
  uint32_t stats_out() const { if (auto* d = direct()) return d->stats_out; return uint32_t((memory::IsHandle(address + 120) ? uint32_t(memory::OwnerLoad(address + 120, 4)) : guest::LoadU32(address + 120))); }
  void set_stats_out(uint32_t v) const { if (auto* d = direct()) { d->stats_out = v; return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 4, v); else guest::StoreU32(address + 120, v); }
  uint32_t loop_a_address() const { return address + 124; }
  uint32_t loop_a_bits() const { if (auto* d = direct()) return d->loop_a; return uint32_t((memory::IsHandle(address + 124) ? uint32_t(memory::OwnerLoad(address + 124, 4)) : guest::LoadU32(address + 124))); }
  void set_loop_a_bits(uint32_t v) const { if (auto* d = direct()) { d->loop_a = v; return; } if (memory::IsHandle(address + 124)) memory::OwnerStore(address + 124, 4, v); else guest::StoreU32(address + 124, v); }
  uint32_t loop_a() const { if (auto* d = direct()) return d->loop_a; return uint32_t((memory::IsHandle(address + 124) ? uint32_t(memory::OwnerLoad(address + 124, 4)) : guest::LoadU32(address + 124))); }
  void set_loop_a(uint32_t v) const { if (auto* d = direct()) { d->loop_a = v; return; } if (memory::IsHandle(address + 124)) memory::OwnerStore(address + 124, 4, v); else guest::StoreU32(address + 124, v); }
  uint32_t loop_b_address() const { return address + 128; }
  uint32_t loop_b_bits() const { if (auto* d = direct()) return d->loop_b; return uint32_t((memory::IsHandle(address + 128) ? uint32_t(memory::OwnerLoad(address + 128, 4)) : guest::LoadU32(address + 128))); }
  void set_loop_b_bits(uint32_t v) const { if (auto* d = direct()) { d->loop_b = v; return; } if (memory::IsHandle(address + 128)) memory::OwnerStore(address + 128, 4, v); else guest::StoreU32(address + 128, v); }
  uint32_t loop_b() const { if (auto* d = direct()) return d->loop_b; return uint32_t((memory::IsHandle(address + 128) ? uint32_t(memory::OwnerLoad(address + 128, 4)) : guest::LoadU32(address + 128))); }
  void set_loop_b(uint32_t v) const { if (auto* d = direct()) { d->loop_b = v; return; } if (memory::IsHandle(address + 128)) memory::OwnerStore(address + 128, 4, v); else guest::StoreU32(address + 128, v); }
  // string 1934 + the maker
  uint32_t text_maker_address() const { return address + 132; }
  uint32_t text_maker_bits() const { if (auto* d = direct()) return d->text_maker; return uint32_t((memory::IsHandle(address + 132) ? uint32_t(memory::OwnerLoad(address + 132, 4)) : guest::LoadU32(address + 132))); }
  void set_text_maker_bits(uint32_t v) const { if (auto* d = direct()) { d->text_maker = v; return; } if (memory::IsHandle(address + 132)) memory::OwnerStore(address + 132, 4, v); else guest::StoreU32(address + 132, v); }
  uint32_t text_maker() const { if (auto* d = direct()) return d->text_maker; return uint32_t((memory::IsHandle(address + 132) ? uint32_t(memory::OwnerLoad(address + 132, 4)) : guest::LoadU32(address + 132))); }
  void set_text_maker(uint32_t v) const { if (auto* d = direct()) { d->text_maker = v; return; } if (memory::IsHandle(address + 132)) memory::OwnerStore(address + 132, 4, v); else guest::StoreU32(address + 132, v); }
  // string 1646 + the car's id
  uint32_t text_name_address() const { return address + 136; }
  uint32_t text_name_bits() const { if (auto* d = direct()) return d->text_name; return uint32_t((memory::IsHandle(address + 136) ? uint32_t(memory::OwnerLoad(address + 136, 4)) : guest::LoadU32(address + 136))); }
  void set_text_name_bits(uint32_t v) const { if (auto* d = direct()) { d->text_name = v; return; } if (memory::IsHandle(address + 136)) memory::OwnerStore(address + 136, 4, v); else guest::StoreU32(address + 136, v); }
  uint32_t text_name() const { if (auto* d = direct()) return d->text_name; return uint32_t((memory::IsHandle(address + 136) ? uint32_t(memory::OwnerLoad(address + 136, 4)) : guest::LoadU32(address + 136))); }
  void set_text_name(uint32_t v) const { if (auto* d = direct()) { d->text_name = v; return; } if (memory::IsHandle(address + 136)) memory::OwnerStore(address + 136, 4, v); else guest::StoreU32(address + 136, v); }
  // string 1718 + the car's id
  uint32_t text_third_address() const { return address + 140; }
  uint32_t text_third_bits() const { if (auto* d = direct()) return d->text_third; return uint32_t((memory::IsHandle(address + 140) ? uint32_t(memory::OwnerLoad(address + 140, 4)) : guest::LoadU32(address + 140))); }
  void set_text_third_bits(uint32_t v) const { if (auto* d = direct()) { d->text_third = v; return; } if (memory::IsHandle(address + 140)) memory::OwnerStore(address + 140, 4, v); else guest::StoreU32(address + 140, v); }
  uint32_t text_third() const { if (auto* d = direct()) return d->text_third; return uint32_t((memory::IsHandle(address + 140) ? uint32_t(memory::OwnerLoad(address + 140, 4)) : guest::LoadU32(address + 140))); }
  void set_text_third(uint32_t v) const { if (auto* d = direct()) { d->text_third = v; return; } if (memory::IsHandle(address + 140)) memory::OwnerStore(address + 140, 4, v); else guest::StoreU32(address + 140, v); }
  // two text children (SetTexts)
  uint32_t texts_address() const { return address + 144; }
  uint32_t texts_bits() const { if (auto* d = direct()) return d->texts; return uint32_t((memory::IsHandle(address + 144) ? uint32_t(memory::OwnerLoad(address + 144, 4)) : guest::LoadU32(address + 144))); }
  void set_texts_bits(uint32_t v) const { if (auto* d = direct()) { d->texts = v; return; } if (memory::IsHandle(address + 144)) memory::OwnerStore(address + 144, 4, v); else guest::StoreU32(address + 144, v); }
  uint32_t texts() const { if (auto* d = direct()) return d->texts; return uint32_t((memory::IsHandle(address + 144) ? uint32_t(memory::OwnerLoad(address + 144, 4)) : guest::LoadU32(address + 144))); }
  void set_texts(uint32_t v) const { if (auto* d = direct()) { d->texts = v; return; } if (memory::IsHandle(address + 144)) memory::OwnerStore(address + 144, 4, v); else guest::StoreU32(address + 144, v); }
  // -1 in PanelCBase::PanelCBase
  uint32_t unknown_148_address() const { return address + 148; }
  uint32_t unknown_148_bits() const { if (auto* d = direct()) return d->unknown_148; return uint32_t((memory::IsHandle(address + 148) ? uint32_t(memory::OwnerLoad(address + 148, 4)) : guest::LoadU32(address + 148))); }
  void set_unknown_148_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_148 = v; return; } if (memory::IsHandle(address + 148)) memory::OwnerStore(address + 148, 4, v); else guest::StoreU32(address + 148, v); }
  uint32_t unknown_148() const { if (auto* d = direct()) return d->unknown_148; return uint32_t((memory::IsHandle(address + 148) ? uint32_t(memory::OwnerLoad(address + 148, 4)) : guest::LoadU32(address + 148))); }
  void set_unknown_148(uint32_t v) const { if (auto* d = direct()) { d->unknown_148 = v; return; } if (memory::IsHandle(address + 148)) memory::OwnerStore(address + 148, 4, v); else guest::StoreU32(address + 148, v); }
  uint32_t cursor_address() const { return address + 152; }
  uint32_t cursor_bits() const { if (auto* d = direct()) return d->cursor; return uint32_t((memory::IsHandle(address + 152) ? uint32_t(memory::OwnerLoad(address + 152, 4)) : guest::LoadU32(address + 152))); }
  void set_cursor_bits(uint32_t v) const { if (auto* d = direct()) { d->cursor = v; return; } if (memory::IsHandle(address + 152)) memory::OwnerStore(address + 152, 4, v); else guest::StoreU32(address + 152, v); }
  uint32_t cursor() const { if (auto* d = direct()) return d->cursor; return uint32_t((memory::IsHandle(address + 152) ? uint32_t(memory::OwnerLoad(address + 152, 4)) : guest::LoadU32(address + 152))); }
  void set_cursor(uint32_t v) const { if (auto* d = direct()) { d->cursor = v; return; } if (memory::IsHandle(address + 152)) memory::OwnerStore(address + 152, 4, v); else guest::StoreU32(address + 152, v); }
  // -1 in PanelCBase::PanelCBase
  uint32_t unknown_156_address() const { return address + 156; }
  uint32_t unknown_156_bits() const { if (auto* d = direct()) return d->unknown_156; return uint32_t((memory::IsHandle(address + 156) ? uint32_t(memory::OwnerLoad(address + 156, 4)) : guest::LoadU32(address + 156))); }
  void set_unknown_156_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_156 = v; return; } if (memory::IsHandle(address + 156)) memory::OwnerStore(address + 156, 4, v); else guest::StoreU32(address + 156, v); }
  uint32_t unknown_156() const { if (auto* d = direct()) return d->unknown_156; return uint32_t((memory::IsHandle(address + 156) ? uint32_t(memory::OwnerLoad(address + 156, 4)) : guest::LoadU32(address + 156))); }
  void set_unknown_156(uint32_t v) const { if (auto* d = direct()) { d->unknown_156 = v; return; } if (memory::IsHandle(address + 156)) memory::OwnerStore(address + 156, 4, v); else guest::StoreU32(address + 156, v); }
  // 22 child indices of the stats node: CarPanel::SetStats uses +160 to +180 and +216 to +228, Panel6C::SetStats +160 to +244
  uint32_t stats_children_address() const { return address + 160; }
  uint8_t* stats_children() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 160 : guest::Pointer(address + 160); }
  uint32_t price_child_address() const { return address + 260; }
  uint32_t price_child_bits() const { if (auto* d = direct()) return d->price_child; return uint32_t((memory::IsHandle(address + 260) ? uint32_t(memory::OwnerLoad(address + 260, 4)) : guest::LoadU32(address + 260))); }
  void set_price_child_bits(uint32_t v) const { if (auto* d = direct()) { d->price_child = v; return; } if (memory::IsHandle(address + 260)) memory::OwnerStore(address + 260, 4, v); else guest::StoreU32(address + 260, v); }
  uint32_t price_child() const { if (auto* d = direct()) return d->price_child; return uint32_t((memory::IsHandle(address + 260) ? uint32_t(memory::OwnerLoad(address + 260, 4)) : guest::LoadU32(address + 260))); }
  void set_price_child(uint32_t v) const { if (auto* d = direct()) { d->price_child = v; return; } if (memory::IsHandle(address + 260)) memory::OwnerStore(address + 260, 4, v); else guest::StoreU32(address + 260, v); }
  // text 527 with the price
  uint32_t price_text_a_address() const { return address + 264; }
  uint32_t price_text_a_bits() const { if (auto* d = direct()) return d->price_text_a; return uint32_t((memory::IsHandle(address + 264) ? uint32_t(memory::OwnerLoad(address + 264, 4)) : guest::LoadU32(address + 264))); }
  void set_price_text_a_bits(uint32_t v) const { if (auto* d = direct()) { d->price_text_a = v; return; } if (memory::IsHandle(address + 264)) memory::OwnerStore(address + 264, 4, v); else guest::StoreU32(address + 264, v); }
  uint32_t price_text_a() const { if (auto* d = direct()) return d->price_text_a; return uint32_t((memory::IsHandle(address + 264) ? uint32_t(memory::OwnerLoad(address + 264, 4)) : guest::LoadU32(address + 264))); }
  void set_price_text_a(uint32_t v) const { if (auto* d = direct()) { d->price_text_a = v; return; } if (memory::IsHandle(address + 264)) memory::OwnerStore(address + 264, 4, v); else guest::StoreU32(address + 264, v); }
  // text 435
  uint32_t price_text_b_address() const { return address + 268; }
  uint32_t price_text_b_bits() const { if (auto* d = direct()) return d->price_text_b; return uint32_t((memory::IsHandle(address + 268) ? uint32_t(memory::OwnerLoad(address + 268, 4)) : guest::LoadU32(address + 268))); }
  void set_price_text_b_bits(uint32_t v) const { if (auto* d = direct()) { d->price_text_b = v; return; } if (memory::IsHandle(address + 268)) memory::OwnerStore(address + 268, 4, v); else guest::StoreU32(address + 268, v); }
  uint32_t price_text_b() const { if (auto* d = direct()) return d->price_text_b; return uint32_t((memory::IsHandle(address + 268) ? uint32_t(memory::OwnerLoad(address + 268, 4)) : guest::LoadU32(address + 268))); }
  void set_price_text_b(uint32_t v) const { if (auto* d = direct()) { d->price_text_b = v; return; } if (memory::IsHandle(address + 268)) memory::OwnerStore(address + 268, 4, v); else guest::StoreU32(address + 268, v); }
  // the first of six button children
  uint32_t buttons_address() const { return address + 272; }
  uint32_t buttons_bits() const { if (auto* d = direct()) return d->buttons; return uint32_t((memory::IsHandle(address + 272) ? uint32_t(memory::OwnerLoad(address + 272, 4)) : guest::LoadU32(address + 272))); }
  void set_buttons_bits(uint32_t v) const { if (auto* d = direct()) { d->buttons = v; return; } if (memory::IsHandle(address + 272)) memory::OwnerStore(address + 272, 4, v); else guest::StoreU32(address + 272, v); }
  uint32_t buttons() const { if (auto* d = direct()) return d->buttons; return uint32_t((memory::IsHandle(address + 272) ? uint32_t(memory::OwnerLoad(address + 272, 4)) : guest::LoadU32(address + 272))); }
  void set_buttons(uint32_t v) const { if (auto* d = direct()) { d->buttons = v; return; } if (memory::IsHandle(address + 272)) memory::OwnerStore(address + 272, 4, v); else guest::StoreU32(address + 272, v); }
  // SetTexts' first string
  uint32_t text_a_address() const { return address + 276; }
  uint32_t text_a_bits() const { if (auto* d = direct()) return d->text_a; return uint32_t((memory::IsHandle(address + 276) ? uint32_t(memory::OwnerLoad(address + 276, 4)) : guest::LoadU32(address + 276))); }
  void set_text_a_bits(uint32_t v) const { if (auto* d = direct()) { d->text_a = v; return; } if (memory::IsHandle(address + 276)) memory::OwnerStore(address + 276, 4, v); else guest::StoreU32(address + 276, v); }
  uint32_t text_a() const { if (auto* d = direct()) return d->text_a; return uint32_t((memory::IsHandle(address + 276) ? uint32_t(memory::OwnerLoad(address + 276, 4)) : guest::LoadU32(address + 276))); }
  void set_text_a(uint32_t v) const { if (auto* d = direct()) { d->text_a = v; return; } if (memory::IsHandle(address + 276)) memory::OwnerStore(address + 276, 4, v); else guest::StoreU32(address + 276, v); }
  // SetTexts' second string
  uint32_t text_b_address() const { return address + 280; }
  uint32_t text_b_bits() const { if (auto* d = direct()) return d->text_b; return uint32_t((memory::IsHandle(address + 280) ? uint32_t(memory::OwnerLoad(address + 280, 4)) : guest::LoadU32(address + 280))); }
  void set_text_b_bits(uint32_t v) const { if (auto* d = direct()) { d->text_b = v; return; } if (memory::IsHandle(address + 280)) memory::OwnerStore(address + 280, 4, v); else guest::StoreU32(address + 280, v); }
  uint32_t text_b() const { if (auto* d = direct()) return d->text_b; return uint32_t((memory::IsHandle(address + 280) ? uint32_t(memory::OwnerLoad(address + 280, 4)) : guest::LoadU32(address + 280))); }
  void set_text_b(uint32_t v) const { if (auto* d = direct()) { d->text_b = v; return; } if (memory::IsHandle(address + 280)) memory::OwnerStore(address + 280, 4, v); else guest::StoreU32(address + 280, v); }
  // the two-player word: no caption
  uint32_t two_players_address() const { return address + 316; }
  uint32_t two_players_bits() const { if (auto* d = direct()) return d->two_players; return uint32_t((memory::IsHandle(address + 316) ? uint32_t(memory::OwnerLoad(address + 316, 4)) : guest::LoadU32(address + 316))); }
  void set_two_players_bits(uint32_t v) const { if (auto* d = direct()) { d->two_players = v; return; } if (memory::IsHandle(address + 316)) memory::OwnerStore(address + 316, 4, v); else guest::StoreU32(address + 316, v); }
  uint32_t two_players() const { if (auto* d = direct()) return d->two_players; return uint32_t((memory::IsHandle(address + 316) ? uint32_t(memory::OwnerLoad(address + 316, 4)) : guest::LoadU32(address + 316))); }
  void set_two_players(uint32_t v) const { if (auto* d = direct()) { d->two_players = v; return; } if (memory::IsHandle(address + 316)) memory::OwnerStore(address + 316, 4, v); else guest::StoreU32(address + 316, v); }
};

// image; measure: CarPanel.+0
struct CarPanelVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t deleting_destructor() const { return guest::LoadU32(address + 0); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t deleting_destructor_address() const { return address + 0; }
  // slot 1 (CarPanel::SetStats, Panel6C::SetStats)
  uint32_t set_stats() const { return guest::LoadU32(address + 4); }
  void set_set_stats(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t set_stats_address() const { return address + 4; }
  // slot 2
  uint32_t set_car_name() const { return guest::LoadU32(address + 8); }
  void set_set_car_name(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t set_car_name_address() const { return address + 8; }
  // slot 3
  uint32_t show_arrows() const { return guest::LoadU32(address + 12); }
  void set_show_arrows(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t show_arrows_address() const { return address + 12; }
};

// embedded; measure: ConfirmPanel
struct ConfirmPanel : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  static constexpr uint32_t kVtable = 0x82010B5C;
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 0 in the constructor
  View group() const { return View(guest::LoadU32(address + 4)); }
  void set_group(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t group_address() const { return address + 4; }
  // 2 in the constructor; picks the node ShowNode shows
  uint32_t kind() const { return guest::LoadU32(address + 8); }
  void set_kind(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t kind_address() const { return address + 8; }
  // -1 in the constructor
  uint32_t unknown_12() const { return guest::LoadU32(address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t node_a() const { return guest::LoadU32(address + 16); }
  void set_node_a(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t node_a_address() const { return address + 16; }
  uint32_t node_b() const { return guest::LoadU32(address + 20); }
  void set_node_b(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t node_b_address() const { return address + 20; }
  uint32_t node_c() const { return guest::LoadU32(address + 24); }
  void set_node_c(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t node_c_address() const { return address + 24; }
  uint32_t show_anim() const { return guest::LoadU32(address + 28); }
  void set_show_anim(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t show_anim_address() const { return address + 28; }
  uint32_t loop_anim() const { return guest::LoadU32(address + 32); }
  void set_loop_anim(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t loop_anim_address() const { return address + 32; }
  uint32_t hide_anim() const { return guest::LoadU32(address + 36); }
  void set_hide_anim(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t hide_anim_address() const { return address + 36; }
};

// embedded; measure: PanelSetBase::Step0.r4, PanelSetBase::Step1.r4, PanelSetBase::Step2.r4, PanelSetBase::Step3.r4, PanelSetBase::Step4.r4, PanelSetBase::Step5.r4, PanelSetBase::Step6.r4, PanelSetBase::Step7.r4, PanelSetBase::Step8.r4, PanelSetBase::Step9.r4, PanelSetBase::Step10.r4, PanelSetBase::Step11.r4, PanelSetBase::Step12.r4, PanelSetBase::Step13.r4, PanelSetBase::Step14.r4, PanelSetBase::Step15.r4, PanelSetBase::Step16.r4, PanelSetBase::Step17.r4, PanelSetBase::Step18.r4, PanelSetBase::Step19.r4, PanelSetBase::Step20.r4, PanelSetBase::Step21.r4, PanelSetBase::Step22.r4, PanelSetBase::Step23.r4, PanelSetBase::Step24.r4, PanelSetBase::Step25.r4, PanelSetBase::Step26.r4
struct CarListStepWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0 on a step's first call
  uint32_t first_pass() const { return guest::LoadU32(address + 0); }
  void set_first_pass(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t first_pass_address() const { return address + 0; }
};

// embedded; measure: ModeType8@+848@+42740.+9644
struct PanelSetPairPart : View {
  using View::View;
  static constexpr uint32_t kSize = 3936;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_1900() const { return guest::LoadU32(address + 1900); }
  void set_unknown_1900(uint32_t v) const { guest::StoreU32(address + 1900, v); }
  uint32_t unknown_1900_address() const { return address + 1900; }
  uint32_t unknown_1904() const { return guest::LoadU32(address + 1904); }
  void set_unknown_1904(uint32_t v) const { guest::StoreU32(address + 1904, v); }
  uint32_t unknown_1904_address() const { return address + 1904; }
  uint32_t unknown_1908() const { return guest::LoadU32(address + 1908); }
  void set_unknown_1908(uint32_t v) const { guest::StoreU32(address + 1908, v); }
  uint32_t unknown_1908_address() const { return address + 1908; }
  uint32_t unknown_1912() const { return guest::LoadU32(address + 1912); }
  void set_unknown_1912(uint32_t v) const { guest::StoreU32(address + 1912, v); }
  uint32_t unknown_1912_address() const { return address + 1912; }
  uint32_t unknown_1916() const { return guest::LoadU32(address + 1916); }
  void set_unknown_1916(uint32_t v) const { guest::StoreU32(address + 1916, v); }
  uint32_t unknown_1916_address() const { return address + 1916; }
  uint32_t unknown_1960() const { return guest::LoadU32(address + 1960); }
  void set_unknown_1960(uint32_t v) const { guest::StoreU32(address + 1960, v); }
  uint32_t unknown_1960_address() const { return address + 1960; }
  uint32_t unknown_3872() const { return guest::LoadU32(address + 3872); }
  void set_unknown_3872(uint32_t v) const { guest::StoreU32(address + 3872, v); }
  uint32_t unknown_3872_address() const { return address + 3872; }
  uint32_t unknown_3876() const { return guest::LoadU32(address + 3876); }
  void set_unknown_3876(uint32_t v) const { guest::StoreU32(address + 3876, v); }
  uint32_t unknown_3876_address() const { return address + 3876; }
  uint32_t unknown_3880() const { return guest::LoadU32(address + 3880); }
  void set_unknown_3880(uint32_t v) const { guest::StoreU32(address + 3880, v); }
  uint32_t unknown_3880_address() const { return address + 3880; }
  uint32_t unknown_3884() const { return guest::LoadU32(address + 3884); }
  void set_unknown_3884(uint32_t v) const { guest::StoreU32(address + 3884, v); }
  uint32_t unknown_3884_address() const { return address + 3884; }
  uint32_t unknown_3888() const { return guest::LoadU32(address + 3888); }
  void set_unknown_3888(uint32_t v) const { guest::StoreU32(address + 3888, v); }
  uint32_t unknown_3888_address() const { return address + 3888; }
  uint32_t unknown_3932() const { return guest::LoadU32(address + 3932); }
  void set_unknown_3932(uint32_t v) const { guest::StoreU32(address + 3932, v); }
  uint32_t unknown_3932_address() const { return address + 3932; }
};

// embedded; measure: PanelSet::Free.r3@+4
struct PanelSetBaseAt4 : View {
  using View::View;
  static constexpr uint32_t kSize = 1880;
  uint32_t unknown_784() const { return guest::LoadU32(address + 784); }
  void set_unknown_784(uint32_t v) const { guest::StoreU32(address + 784, v); }
  uint32_t unknown_784_address() const { return address + 784; }
  uint32_t unknown_1300() const { return guest::LoadU32(address + 1300); }
  void set_unknown_1300(uint32_t v) const { guest::StoreU32(address + 1300, v); }
  uint32_t unknown_1300_address() const { return address + 1300; }
  uint32_t unknown_1816() const { return guest::LoadU32(address + 1816); }
  void set_unknown_1816(uint32_t v) const { guest::StoreU32(address + 1816, v); }
  uint32_t unknown_1816_address() const { return address + 1816; }
  uint32_t unknown_1820() const { return guest::LoadU32(address + 1820); }
  void set_unknown_1820(uint32_t v) const { guest::StoreU32(address + 1820, v); }
  uint32_t unknown_1820_address() const { return address + 1820; }
  uint32_t unknown_1876() const { return guest::LoadU32(address + 1876); }
  void set_unknown_1876(uint32_t v) const { guest::StoreU32(address + 1876, v); }
  uint32_t unknown_1876_address() const { return address + 1876; }
};

// embedded; measure: PanelSet::SetKind.r3@+4
struct PanelSetBaseAt4_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 1884;
  uint32_t unknown_32() const { return guest::LoadU32(address + 32); }
  void set_unknown_32(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  uint32_t unknown_36() const { return guest::LoadU32(address + 36); }
  void set_unknown_36(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t unknown_36_address() const { return address + 36; }
  uint32_t unknown_40() const { return guest::LoadU32(address + 40); }
  void set_unknown_40(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  uint32_t unknown_44() const { return guest::LoadU32(address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  uint32_t unknown_52() const { return guest::LoadU32(address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
  uint32_t unknown_56() const { return guest::LoadU32(address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_228() const { return guest::LoadU32(address + 228); }
  void set_unknown_228(uint32_t v) const { guest::StoreU32(address + 228, v); }
  uint32_t unknown_228_address() const { return address + 228; }
  uint32_t unknown_232() const { return guest::LoadU32(address + 232); }
  void set_unknown_232(uint32_t v) const { guest::StoreU32(address + 232, v); }
  uint32_t unknown_232_address() const { return address + 232; }
  uint32_t unknown_1820() const { return guest::LoadU32(address + 1820); }
  void set_unknown_1820(uint32_t v) const { guest::StoreU32(address + 1820, v); }
  uint32_t unknown_1820_address() const { return address + 1820; }
  uint32_t unknown_1880() const { return guest::LoadU32(address + 1880); }
  void set_unknown_1880(uint32_t v) const { guest::StoreU32(address + 1880, v); }
  uint32_t unknown_1880_address() const { return address + 1880; }
};

// The image words docs/functions/car-list.md's code uses.
struct CarListGlobals {
  // 0 in PanelSet, 1 in PanelSetPair
  static uint32_t two_players() { return guest::LoadU32(0x82815EACu); }
  static void set_two_players(uint32_t v) { guest::StoreU32(0x82815EACu, v); }
  static constexpr uint32_t two_players_address() { return 0x82815EACu; }
  // set with each fade, cleared by GroupPair::WaitForFade; a step doesn't move on while set
  static uint32_t fade_wait() { return guest::LoadU32(0x82815EB0u); }
  static void set_fade_wait(uint32_t v) { guest::StoreU32(0x82815EB0u, v); }
  static constexpr uint32_t fade_wait_address() { return 0x82815EB0u; }
  // the power graph screen's record block (its record at +8)
  template <typename T = PowerGraphScreenData>
  static T power_graph() { return T(guest::LoadU32(0x82815EBCu)); }
  static void set_power_graph(const View& v) { guest::StoreU32(0x82815EBCu, v.address); }
  static void set_power_graph(uint32_t v) { guest::StoreU32(0x82815EBCu, v); }
  static constexpr uint32_t power_graph_address() { return 0x82815EBCu; }
  // the players' save data (sub_82105678's), 120,108 bytes each
  static View pair_save_data() { return View(guest::LoadU32(0x826C5B5Cu)); }
  static void set_pair_save_data(uint32_t v) { guest::StoreU32(0x826C5B5Cu, v); }
  static constexpr uint32_t pair_save_data_address() { return 0x826C5B5Cu; }
  // the stand's holders: three slots, by the player
  template <typename T = StandHolder>
  static T stand_holders() { return T(guest::LoadU32(0x826D83E0u)); }
  static void set_stand_holders(const View& v) { guest::StoreU32(0x826D83E0u, v.address); }
  static void set_stand_holders(uint32_t v) { guest::StoreU32(0x826D83E0u, v); }
  static constexpr uint32_t stand_holders_address() { return 0x826D83E0u; }
  // the two-player stand's wait word
  static uint32_t stand_wait() { return guest::LoadU32(0x826379B8u); }
  static void set_stand_wait(uint32_t v) { guest::StoreU32(0x826379B8u, v); }
  static constexpr uint32_t stand_wait_address() { return 0x826379B8u; }
  // ScreenFades::FadeIn's pass count (5)
  static uint32_t fade_in_passes() { return guest::LoadU32(0x82001BECu); }
  static void set_fade_in_passes(uint32_t v) { guest::StoreU32(0x82001BECu, v); }
  static constexpr uint32_t fade_in_passes_address() { return 0x82001BECu; }
  // ScreenFades::FadeOut's (15)
  static uint32_t fade_out_passes() { return guest::LoadU32(0x82001BF0u); }
  static void set_fade_out_passes(uint32_t v) { guest::StoreU32(0x82001BF0u, v); }
  static constexpr uint32_t fade_out_passes_address() { return 0x82001BF0u; }
  // the stats record's +8 scaled by it
  static double stats_scale_a() { return std::bit_cast<double>(guest::LoadU64(0x82009F00u)); }
  static void set_stats_scale_a(double v) { guest::StoreU64(0x82009F00u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t stats_scale_a_address() { return 0x82009F00u; }
  // the stats record's +24 scaled by it
  static double stats_scale_b() { return std::bit_cast<double>(guest::LoadU64(0x82009F08u)); }
  static void set_stats_scale_b(double v) { guest::StoreU64(0x82009F08u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t stats_scale_b_address() { return 0x82009F08u; }
  // the RGB scale (255)
  static float color_scale() { return guest::LoadFloat(0x82000C44u); }
  static void set_color_scale(float v) { guest::StoreFloat(0x82000C44u, v); }
  static constexpr uint32_t color_scale_address() { return 0x82000C44u; }
  // the saturation's and value's unit
  static float color_unit() { return guest::LoadFloat(0x82002200u); }
  static void set_color_unit(float v) { guest::StoreFloat(0x82002200u, v); }
  static constexpr uint32_t color_unit_address() { return 0x82002200u; }
  // added to a color record's value byte when its +0 is set
  static uint32_t color_value_add_raised() { return guest::LoadU32(0x82002FF4u); }
  static void set_color_value_add_raised(uint32_t v) { guest::StoreU32(0x82002FF4u, v); }
  static constexpr uint32_t color_value_add_raised_address() { return 0x82002FF4u; }
  // added to a color record's value byte when its +0 is 0
  static uint32_t color_value_add() { return guest::LoadU32(0x82002FF8u); }
  static void set_color_value_add(uint32_t v) { guest::StoreU32(0x82002FF8u, v); }
  static constexpr uint32_t color_value_add_address() { return 0x82002FF8u; }
};

}  // namespace schema
