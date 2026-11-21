// The type 6 and type 7 modes' screen constructors as plain C++
// (screen_constructors.h, docs/functions/screen-constructors.md), read from
// the generated code.

#include "screen_constructors/screen_constructors.h"

#include "base/log.h"
#include "boot_mode/boot_mode.h"
#include "guest.h"
#include "schema/car_list.h"
#include "schema/screen_constructors.h"
#include "schema/type67_screens.h"
#include "schema/type6_type7_modes.h"

namespace screen_constructors {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
// `value` in the words from `from` to `to` (both included).
void Fill(uint8_t* base, uint32_t from, uint32_t to, uint32_t value) {
  for (uint32_t a = from; a <= to; a += 4) {
    Store(base, a, value);
  }
}

constexpr uint32_t kNone = 0xFFFFFFFF;  // -1
// The panels' vtable (PanelGroupBase's too).
constexpr uint32_t kPanelVtable = 0x82010974;

// Panel6C's and Panel7C's words.
void PanelC(uint8_t* base, uint32_t panel, uint32_t group, uint32_t vtable, uint32_t w20,
            uint32_t w24, uint32_t w32) {
  PanelCBase(base, panel);
  const schema::CarPanel p(base, panel);
  p.set_group(group);
  p.set_node(w20);
  p.set_vtable(vtable);
  p.set_shows_price(0);
  p.set_stats_node(w24);
  p.set_buttons_node(w32);
  p.set_show_anim_a(109);
  p.set_show_anim_b(kNone);
  p.set_hide_anim_a(kNone);
  p.set_hide_anim_b(kNone);
  p.set_show_again_anim(kNone);
  p.set_arrow_a(112);
  Store(base, p.action_anims_address() + 0, 5);
  Store(base, p.action_anims_address() + 4, 6);
  p.set_scroll_back(15);
  p.set_scroll_on(16);
  p.set_loop_a(0);
  p.set_loop_b(43);
  p.set_text_maker(13);
  p.set_text_name(14);
  p.set_text_third(15);
  p.set_texts(5);
  p.set_cursor(0);
  for (uint32_t i = 0; i < 4; i++) {
    Store(base, p.stats_children_address() + 4 * i, 5 + i);  // +160 to +172: 5 to 8
  }
  Store(base, p.stats_children_address() + 16, 11);  // +176
  Store(base, p.stats_children_address() + 20, 12);  // +180
  Store(base, p.stats_children_address() + 56, 9);   // +216
  Store(base, p.stats_children_address() + 60, 10);  // +220
  Store(base, p.stats_children_address() + 64, 13);  // +224
  Store(base, p.stats_children_address() + 68, 14);  // +228
  p.set_buttons(4);
  p.set_text_a(123);
  p.set_text_b(112);
}

// Panel6D's and Panel7D's words (+28 to +40: `w28` up).
void PanelD(uint8_t* base, uint32_t panel, uint32_t group, uint32_t w12, uint32_t w24,
            uint32_t w28, uint32_t w60) {
  PanelDBase(base, panel);
  const schema::ColorPanel p(base, panel);
  p.set_vtable(kPanelVtable);
  p.set_group(group);
  p.set_node(w12);
  p.set_show_anim(110);
  p.set_hide_anim(111);
  p.set_item_anims(w24);
  for (uint32_t i = 0; i < 4; i++) {
    // +28 to +40: scroll_back, scroll_on, swatches_on, swatches_back
    Store(base, p.scroll_back_address() + 4 * i, w28 + i);
  }
  p.set_arrows_node(w60);
  p.set_cursor(0);
  p.set_swatches(1);
  p.set_swatch_color(2);
}

// Panel6F's and Panel7F's words (the same in both).
void PanelF(uint8_t* base, uint32_t panel, uint32_t group) {
  PanelFBase(base, panel);
  const schema::TabPanel p(base, panel);
  p.set_vtable(kPanelVtable);
  p.set_group(group);
  p.set_texts_node(3);
  p.set_tabs_node(6);
  p.set_buttons_node(0);
  p.set_cursor(0);
  p.set_tab0_text(5);
  p.set_tab1_text(6);
  p.set_tab2_text(7);
  p.set_buttons(4);
  p.set_tab_word_a(1);
  p.set_tab_word_b(0);
  p.set_tab_anims(5);
  p.set_scroll_down(15);
  p.set_scroll_up(16);
  p.set_unknown_108(29);
  p.set_unknown_112(30);
  p.set_show_anim(79);
  p.set_move_anim(73);
  p.set_loop_a(2);
  p.set_loop_c(75);
  p.set_loop_d(83);
}

// The three PanelGroups' words.
void PanelGroupWords(uint8_t* base, uint32_t group, uint32_t w4, uint32_t w12) {
  PanelGroupBase(base, group);
  const schema::PanelGroup g(base, group);
  g.set_movie(w4);
  g.set_vtable(0x82010E8C);
  g.set_caption_node_set(w12);
  g.set_unknown_32(0);
}

// The node tables' four pairs (0, -1) at +428 to +456.
void NodeTablePairs(uint8_t* base, uint32_t table) {
  const schema::PairedNodeTable t(base, table);
  for (uint32_t i = 0; i < 4; i++) {
    Store(base, t.pairs_address() + 8 * i, 0);
    Store(base, t.pairs_address() + 4 + 8 * i, kNone);
  }
}

// slw: 1 shifted left by `amount`'s low six bits (0 from 32 up).
uint32_t Bit(uint32_t amount) {
  return (amount & 0x20) != 0 ? 0 : 1u << (amount & 0x1F);
}

// The panel screens: `group_b`, `panel_c`, `panel_d` and `panel_f` the
// screen type's constructors, `last_zero` the last word cleared (+11676 or
// +11680).
struct PanelScreenType {
  uint32_t vtable;
  void (*group_b)(uint8_t*, uint32_t);
  void (*panel_c)(uint8_t*, uint32_t, uint32_t);
  void (*panel_d)(uint8_t*, uint32_t, uint32_t);
  void (*panel_f)(uint8_t*, uint32_t, uint32_t);
  uint32_t last_zero;
};

void PanelScreen(uint8_t* base, uint32_t screen, uint32_t sp, const New& new_panel,
                 const PanelScreenType& type) {
  const uint32_t frame = sp - kPanelScreenFrame;
  const schema::Type67PanelScreen s(base, screen);
  MachineScreen(base, screen);
  s.set_vtable(type.vtable);
  const uint32_t group_a = s.group_a_address();
  PanelGroup(base, group_a);
  const uint32_t group_b = s.group_b_address();
  type.group_b(base, group_b);
  // Each panel built unless operator new gave 0 (the original's check).
  auto make = [&](uint32_t size, void (*construct)(uint8_t*, uint32_t, uint32_t),
                  uint32_t group) {
    const uint32_t panel = new_panel(size, frame);
    if (panel != 0) {
      construct(base, panel, group);
    }
    return panel;
  };
  const uint32_t c = make(schema::CarPanel::kSize, type.panel_c, group_b);
  const uint32_t d = make(schema::ColorPanel::kSize, type.panel_d, group_b);
  const uint32_t e = make(schema::RowPanel::kSize, PanelE, group_a);
  const uint32_t f = make(schema::TabPanel::kSize, type.panel_f, group_a);
  const uint32_t set = s.set_address();
  PanelSet(base, set, f, e, d, c, group_b, frame);
  const uint32_t pair = s.pair_address();
  GroupPair(base, pair, group_a, group_b);
  s.set_car_list(set);
  s.set_group_pair(pair);
  s.set_set_group_pair(pair);
  // +11672 (the set's +1920) to +11676 or +11680.
  Fill(base, s.set_unknown_1920_address(), screen + type.last_zero, 0);
  s.set_set_unknown_1964(45);
  s.set_set_unknown_1968(44);
  s.set_set_unknown_1972(46);
}

// The empty base many classes share (as ListNode's in archive.md).
constexpr uint32_t kEmptyBaseVtable = 0x82010B5C;

}  // namespace

// --- The panels and their groups ---

void NodeBase(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82258FB0 NodeBase::NodeBase");
  const schema::NodeBase node(base, object);
  node.set_unknown_4(0);
  node.set_vtable(kEmptyBaseVtable);
}

void PanelGroupBase(uint8_t* base, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_8224EEA8 PanelGroupBase::PanelGroupBase");
  NodeBase(base, group);
  const schema::PanelGroupBase g(base, group);
  g.set_vtable(0x82010974);
  Fill(base, g.unknown_8_address(), g.caption_node_address(), kNone);  // +8 to +32
}

void PanelCBase(uint8_t* base, uint32_t panel) {
  ITC_NATIVE_RUNNING("sub_8224D6D0 PanelCBase::PanelCBase");
  const schema::CarPanel p(base, panel);
  p.set_vtable(0x820111C0);
  p.set_shows_price(1);
  p.set_action(kNone);
  p.set_group(0);
  p.set_node_set(kNone);
  Fill(base, p.stats_node_address(), p.texts_address(), kNone);  // +24 to +140
  // +148 to +244 (unknown_148 to the stats children's 22nd word).
  Fill(base, p.unknown_148_address(), p.stats_children_address() + 84, kNone);
  Fill(base, p.price_child_address(), p.buttons_address(), kNone);  // +260 to +272
  Fill(base, p.text_a_address(), p.two_players_address(), 0);       // +276 to +316
}

void PanelDBase(uint8_t* base, uint32_t panel) {
  ITC_NATIVE_RUNNING("sub_8224CFA0 PanelDBase::PanelDBase");
  const schema::ColorPanel p(base, panel);
  p.set_group(0);
  p.set_vtable(kEmptyBaseVtable);
  Fill(base, p.node_set_address(), p.swatch_color_address(), kNone);  // +8 to +80
  p.set_two_players(0);
}

void PanelEBase(uint8_t* base, uint32_t panel) {
  ITC_NATIVE_RUNNING("sub_8224EF78 PanelEBase::PanelEBase");
  const schema::RowPanel p(base, panel);
  p.set_group(0);
  p.set_row_count(0);
  Fill(base, p.row_address(), p.arrows_address(), kNone);  // +8 to +148
  p.set_show_alt(0);
  p.set_two_players(0);
}

void PanelE(uint8_t* base, uint32_t panel, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_82230800 PanelE::PanelE");
  PanelEBase(base, panel);
  const schema::RowPanel p(base, panel);
  p.set_group(group);  // no vtable
  p.set_row_count(4);
  p.set_node(4);
  for (uint32_t i = 0; i < 4; i++) {
    Store(base, p.row_nodes_address() + 4 * i, 5 + i);  // +20 to +32: 5 to 8
    // +124 to +136 (text_maker, text_name, text_third, unknown_136): 5 to 8
    Store(base, p.text_maker_address() + 4 * i, 5 + i);
  }
  p.set_spare_row_node(9);
  p.set_unknown_64(5);
  p.set_unknown_68(15);
  p.set_unknown_72(16);
  p.set_show_anim(80);
  p.set_show_anim_alt(81);
  p.set_hide_anim(82);
  p.set_row_anims(62);
  p.set_scroll_up(67);
  p.set_scroll_down(68);
  p.set_arrow_up(72);
  p.set_loop_a(2);
  p.set_loop_b(83);
  p.set_logo(3);
  p.set_unknown_140(kNone);
  p.set_cursor(1);
  p.set_arrows(3);
}

void PanelFBase(uint8_t* base, uint32_t panel) {
  ITC_NATIVE_RUNNING("sub_8224FA88 PanelFBase::PanelFBase");
  const schema::TabPanel p(base, panel);
  p.set_group(0);
  p.set_vtable(kEmptyBaseVtable);
  Fill(base, p.node_set_address(), p.loop_d_address(), kNone);  // +24 to +136
  p.set_two_players(0);
}

void Panel6C(uint8_t* base, uint32_t panel, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_82231CD0 Panel6C::Panel6C");
  PanelC(base, panel, group, 0x82010980, 7, 0, 5);
}

void Panel7C(uint8_t* base, uint32_t panel, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_822416D8 Panel7C::Panel7C");
  PanelC(base, panel, group, 0x82010EE4, 6, 4, 3);
}

void Panel6D(uint8_t* base, uint32_t panel, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_82231E40 Panel6D::Panel6D");
  PanelD(base, panel, group, 22, 96, 105, 21);
}

void Panel7D(uint8_t* base, uint32_t panel, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_822418B8 Panel7D::Panel7D");
  PanelD(base, panel, group, 20, 94, 103, 19);
}

void Panel6F(uint8_t* base, uint32_t panel, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_822308E8 Panel6F::Panel6F");
  PanelF(base, panel, group);
}

void Panel7F(uint8_t* base, uint32_t panel, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_82240268 Panel7F::Panel7F");
  PanelF(base, panel, group);
}

void PanelGroup(uint8_t* base, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_82240110 PanelGroup::PanelGroup");
  PanelGroupWords(base, group, 4, 1);
}

void PanelGroup6B(uint8_t* base, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_82231C68 PanelGroup6B::PanelGroup6B");
  PanelGroupWords(base, group, 28, 4);
}

void PanelGroup7B(uint8_t* base, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_82241680 PanelGroup7B::PanelGroup7B");
  PanelGroupWords(base, group, 28, 2);
}

void PanelSetBase(uint8_t* base, uint32_t set_base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224AD28 PanelSetBase::PanelSetBase");
  const schema::PanelSetBase s(base, set_base);
  s.set_vtable(0x820110C4);
  // The original clears eleven words below its r1 (it has no frame) and
  // reads ten of them back into its fields.
  Fill(base, sp - 48, sp - 8, 0);
  s.set_page_menu_items(0);
  s.set_page_menu_player(0);
  s.set_rows_menu_items(Load(base, sp - 44));
  s.set_rows_menu_player(Load(base, sp - 40));
  s.set_list0_count(Load(base, sp - 36));
  s.set_list1_count(Load(base, sp - 32));
  s.set_list2_count(Load(base, sp - 28));
  s.set_color_count(Load(base, sp - 24));
  const uint32_t at_20 = Load(base, sp - 20);
  s.set_unknown_224(kNone);
  s.set_answer_record(at_20);
  s.set_unknown_208(Load(base, sp - 16));
  s.set_unknown_212(Load(base, sp - 12));
  s.set_unknown_216(Load(base, sp - 8));
  s.set_unknown_220(0);
  Fill(base, s.tab_shown_address(), s.color_scroll_address(), 0);  // +228 to +248
  s.set_rows_shown(0);
  s.set_unknown_1920(1);
  s.set_built(0);
  s.set_done(0);
  s.set_open_kind(1);
  Fill(base, s.power_graph_asked_address(), s.busy_playing_address(), 0);  // +16 to +28
  schema::CarListGlobals::set_two_players(base, 0);
  s.set_page_drawn(kNone);
  s.set_page_changed(1);
  s.set_may_leave(1);
  s.set_cancelled(0);
  s.set_fade_with_steps(1);
  s.set_player(kNone);
  Fill(base, s.back_object_address(), s.confirm_panel_address(), 0);  // +1892 to +1912
  s.set_group_b(0);
  Fill(base, s.busy_anim_a_address(), s.busy_anim_done_address(), kNone);  // +1960 to +1968
}

void PanelSet(uint8_t* base, uint32_t set, uint32_t f, uint32_t e, uint32_t d, uint32_t c,
              uint32_t group_b, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224B660 PanelSet::PanelSet");
  const schema::PanelSet s(base, set);
  s.set_vtable(0x820110C8);
  PanelSetBase(base, s.base_vtable_address(), sp - kPanelSetFrame);
  // The panels get the word as it reads after each store (not checked for 0).
  schema::CarListGlobals::set_two_players(base, 0);
  s.set_tabs(f);
  schema::TabPanel(base, f).set_two_players(schema::CarListGlobals::two_players(base));
  s.set_rows(e);
  schema::RowPanel(base, e).set_two_players(schema::CarListGlobals::two_players(base));
  s.set_color_panel(d);
  schema::ColorPanel(base, d).set_two_players(schema::CarListGlobals::two_players(base));
  s.set_car_panel(c);
  schema::CarPanel(base, c).set_two_players(schema::CarListGlobals::two_players(base));
  s.set_group_b(group_b);
}

void GroupPair(uint8_t* base, uint32_t pair, uint32_t group_a, uint32_t group_b) {
  ITC_NATIVE_RUNNING("sub_82249C08 GroupPair::GroupPair");
  const schema::GroupPair p(base, pair);
  p.set_group_a(group_a);
  p.set_group_b(group_b);
  p.set_vtable(0x82011084);
}

// --- The screens' bases ---

void MachineScreen(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_8224CF58 MachineScreen::MachineScreen");
  const schema::MachineScreen s(base, screen);
  boot_mode::ScreenMachineConstruct(base, s.machine_address());
  s.set_opens_itself(0);
  s.set_vtable(0x82011168);
}

void Type6Screen0(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_82235BD8 Type6Screen0::Type6Screen0");
  boot_mode::ScreenMachineConstruct(base, schema::Type6Screen0(base, screen).machine_address());
}

void Type7Screen0(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_82245FF8 Type7Screen0::Type7Screen0");
  const schema::Type7Screen0 s(base, screen);
  boot_mode::ScreenMachineConstruct(base, s.machine_address());
  s.set_vtable(kEmptyBaseVtable);
}

// --- The list screens' parts ---

void NodeTable(uint8_t* base, uint32_t table) {
  ITC_NATIVE_RUNNING("sub_82247BE8 NodeTable::NodeTable");
  const schema::NodeTable t(base, table);
  t.set_vtable(0x82010FF0);
  // +8 to +308 (node_set_a to the children's last word).
  Fill(base, t.node_set_a_address(), t.children_address() + 12, kNone);
  Fill(base, t.item_masks_address(), t.item_masks_address() + 104, 0);  // +312 to +416
  t.set_extra_node_base(kNone);
  t.set_text_node_base(kNone);
}

void PairedNodeTable(uint8_t* base, uint32_t table) {
  ITC_NATIVE_RUNNING("sub_82247C60 PairedNodeTable::PairedNodeTable");
  NodeTable(base, table);
  schema::PairedNodeTable(base, table).set_vtable(0x82010FF8);
  NodeTablePairs(base, table);
}

void ColorNodeTable(uint8_t* base, uint32_t table) {
  ITC_NATIVE_RUNNING("sub_82247D08 ColorNodeTable::ColorNodeTable");
  NodeTable(base, table);
  const schema::ColorNodeTable t(base, table);
  t.set_vtable(0x82011000);
  NodeTablePairs(base, table);  // +460 to +472 (the marks) not written
  // +476 to +548 (color_child, the two grids).
  Fill(base, t.color_child_address(), t.grid_b_address() + 32, kNone);
  Fill(base, t.color_a_address(), t.color_a_address() + 12, 255);  // +552 to +564
  Store(base, t.color_b_address() + 0, 205);
  Store(base, t.color_b_address() + 4, 235);
  Store(base, t.color_b_address() + 8, 67);
  Store(base, t.color_b_address() + 12, 255);
}

void ListScreen(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_82252290 ListScreen::ListScreen");
  const schema::ListScreen s(base, screen);
  boot_mode::ScreenMachineConstruct(base, s.machine_address());
  s.set_vtable(0x82011218);
  s.set_list_part(0);
  s.set_movie(1);
  s.set_unknown_9684(1);
  s.set_part_nodes(0);
  Fill(base, s.title_child_address(), s.selection_child_address(), kNone);  // +9712 to +9768
  s.set_previous_children(0);
  s.set_selection_children(0);  // +9780 not written
  s.set_children_count(0);
  s.set_shown_child_on(1);
  // +9792 to +9932, then five runs of ten words from +9936 (to +10132).
  Fill(base, s.unknown_9792_address(), s.item_anims_c_address() + 36, kNone);
  s.set_scroll_text_shown(0);
  s.set_scroll_text(kNone);
}

void ListPartBase(uint8_t* base, uint32_t part) {
  ITC_NATIVE_RUNNING("sub_822523F8 ListPartBase::ListPartBase");
  const schema::ListPart p(base, part);
  p.set_vtable(0x820112A0);
  Fill(base, p.yes_address(), p.inner_address(), 0);  // +4 to +16
}

void ListPartInnerBase(uint8_t* base, uint32_t inner) {
  ITC_NATIVE_RUNNING("sub_822525E0 ListPartInnerBase::ListPartInnerBase");
  const schema::ListPartInner p(base, inner);
  p.set_vtable(0x820112CC);
  p.set_yes(0);
  p.set_back(0);
}

void ListPartInner(uint8_t* base, uint32_t inner) {
  ITC_NATIVE_RUNNING("sub_822526A0 ListPartInner::ListPartInner");
  ListPartInnerBase(base, inner);
  const schema::ListPartInner p(base, inner);
  p.set_vtable(0x820112F8);
  p.set_menu_player(0);
  p.set_menu_enabled(0);
  p.set_prompt_state(3);
  p.set_leave_asked(0);
}

void ListPart(uint8_t* base, uint32_t part) {
  ITC_NATIVE_RUNNING("sub_82252908 ListPart::ListPart");
  ListPartBase(base, part);
  const schema::ListPart p(base, part);
  p.set_vtable(0x82011314);
  ListPartInner(base, p.inner_storage_address());
  p.set_movie(16);
  p.set_inner(p.inner_storage_address());
}

void ColorListScreen(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_822550F8 ColorListScreen::ColorListScreen");
  ListScreen(base, screen);
  const schema::ColorListScreen s(base, screen);
  s.set_vtable(0x820114E0);
  s.set_course_list_player(0);
  s.set_course_list_enabled(0);
  s.set_color_list_player(0);
  s.set_color_list_enabled(0);
  s.set_toggle_player(0);
  s.set_toggle_enabled(0);
  s.set_kind_list_player(0);
  s.set_kind_list_enabled(0);
  ColorNodeTable(base, s.color_table_address());
  const uint32_t count = s.movie();
  s.set_node_table(s.color_table_address());
  s.set_table_movie(count);
  for (uint32_t i = 0; i < 5; i++) {
    // +11016 to +11032 (text_items to text_colors): 545 to 549
    Store(base, s.text_items_address() + 4 * i, 545 + i);
  }
  s.set_text_part(kNone);
}

void Type7ListScreen(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_82242BE8 Type7ListScreen::Type7ListScreen");
  ColorListScreen(base, screen);
  const schema::Type7ListScreen s(base, screen);
  s.set_vtable(0x82010F18);
  ListPart(base, s.part_address());
  s.set_list_part(s.part_address());
  s.set_open_anim(165);
  s.set_first_to_items_anim(166);
  s.set_items_to_first_anim(168);
  s.set_items_to_colors_anim(167);
  s.set_colors_to_items_anim(169);
  s.set_open_loop_a(170);
  s.set_open_loop_b(0);
  s.set_shown_loop_a(47);
  s.set_items_loop(171);
  s.set_shown_loop_b(5);
  s.set_shown_loop_d(82);
  s.set_toggle_anim_a(6);
  s.set_toggle_anim_b(7);
  s.set_selection_anim(87);
  s.set_item_up_anim(139);
  s.set_item_down_anim(140);
  s.set_first_back_anim(100);
  s.set_first_forward_anim(101);
  s.set_color_back_anim(20);
  s.set_color_forward_anim(21);
  s.set_text_sub_scene(6);
  s.set_selection_sub_scene(16);
  s.set_text_child_b(11);
  s.set_table_word_10916(79);
  s.set_table_word_10920(76);
  s.set_table_word_10924(77);
  s.set_table_word_10932(78);
  s.set_table_word_10936(80);
  s.set_table_word_10940(81);
  s.set_table_word_10952(67);
  s.set_table_word_10956(64);
  s.set_table_word_10960(65);
  s.set_table_word_10968(66);
  s.set_table_word_10972(68);
  s.set_table_word_10976(69);
  s.set_table_node_set_a(12);  // the node table's +8 and +12, stored directly
  s.set_table_node_set_b(11);
  const uint32_t table = s.node_table().address;
  s.set_text_child_a(10);
  s.set_title_child(0);
  s.set_arrow_on_frame(0);
  s.set_arrow_child_a(8);
  s.set_arrow_child_b(9);
  s.set_arrow_off_frame(1);
  s.set_selection_child(3);
  s.set_shown_child(0);
  Store(base, schema::ColorNodeTable(base, table).children_address(), 6);
  for (uint32_t i = 1; i < 4; i++) {
    const schema::ColorNodeTable t(base, s.node_table().address);
    Store(base, t.children_address() + 4 * i, 6 + i);  // +300 to +308: 7 to 9
  }
  s.set_children_count(2);
  s.set_shown_child_on(1);
  // 15 items, through the table at +10432 directly: +16 + 4i = 48 + i,
  // +156 + 4i = 1 + i, +312 + 4i = the record's mask.
  for (uint32_t i = 0; i < 15; i++) {
    Store(base, s.table_item_anim_0_address() + 4 * i, 48 + i);
    Store(base, s.table_item_node_0_address() + 4 * i, 1 + i);
    const schema::Type7ListRecord record(
        base, schema::ScreenConstructorsGlobals::type7_list_records_address() +
                  schema::Type7ListRecord::kSize * i);
    const int32_t count = record.count();
    uint32_t mask = 0;
    if (count >= 2) {
      mask = Bit(record.bit_b()) | Bit(record.bit_a());
    } else if (count != 0) {
      mask = Bit(record.bit_a());
    }
    Store(base, s.table_item_mask_0_address() + 4 * i, mask);
  }
  s.set_table_extra_node_base(5);
  s.set_table_text_node_base(20);
  for (uint32_t i = 0; i < 3; i++) {
    Store(base, s.item_anims_address() + 4 * i, 129 + i);
    Store(base, s.first_anims_address() + 4 * i, 90 + i);
    Store(base, s.color_anims_address() + 4 * i, 10 + i);
  }
  s.set_color_cursor_node(9);
  s.set_color_cursor_child(0);
  s.set_icon_sub_scene(2);
  s.set_scroll_sub_scene(1);
  s.set_scroll_child(0);
  s.set_item_cursor_node(21);
  s.set_item_anim_b_0(141);
  s.set_selection_children(0x826371A4);
  s.set_item_anim_b_1(142);
  s.set_item_anim_b_2(143);
  s.set_item_anim_c_0(155);
  s.set_item_anim_c_2(157);
  s.set_icon_child(4);
  s.set_item_anim_c_1(156);
  s.set_item_cursor_child(0);
  s.set_unknown_9684(0);
  s.set_scroll_down_anim(152);
  s.set_scroll_up_anim(151);
}

void NodeListScreen(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_82256F68 NodeListScreen::NodeListScreen");
  ListScreen(base, screen);
  const schema::NodeListScreen s(base, screen);
  s.set_vtable(0x82011558);
  s.set_kind_list_player(0);
  s.set_kind_list_enabled(0);
  s.set_course_list_player(0);
  s.set_course_list_enabled(0);
  s.set_toggle_player(0);
  s.set_toggle_enabled(0);
  PairedNodeTable(base, s.paired_table_address());
  s.set_node_table(s.paired_table_address());
  s.set_movie(25);
  s.set_table_movie(25);
}

void TitledListScreen(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_822570F0 TitledListScreen::TitledListScreen");
  NodeListScreen(base, screen);
  const schema::Type6ListScreen s(base, screen);  // TitledListScreen's layout
  s.set_vtable(0x820115C8);
  s.set_scroll_text_id(923);
}

void Type6ListScreen(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_822330F0 Type6ListScreen::Type6ListScreen");
  TitledListScreen(base, screen);
  const schema::Type6ListScreen s(base, screen);
  s.set_vtable(0x820109A8);
  ListPart(base, s.part_address());
  s.set_list_part(s.part_address());
  s.set_title_child(0);
  s.set_text_child_a(10);
  s.set_text_child_b(11);
  s.set_selection_sub_scene(15);
  s.set_text_sub_scene(7);
  s.set_icon_sub_scene(2);
  s.set_scroll_sub_scene(1);
  s.set_item_cursor_node(8);
  s.set_shown_child(0);
  s.set_arrow_child_a(8);
  s.set_arrow_child_b(9);
  s.set_selection_node(19);
  s.set_selection_child(3);
  s.set_children_count(0);
  s.set_shown_child_on(0);
  s.set_arrow_on_frame(0);
  s.set_arrow_off_frame(1);
  s.set_icon_child(4);
  s.set_scroll_child(0);
  s.set_item_cursor_child(0);
  s.set_open_anim(104);
  s.set_open_loop_a(105);
  s.set_open_loop_b(0);
  s.set_shown_loop_a(98);
  s.set_items_loop(107);
  s.set_shown_loop_b(5);
  s.set_shown_loop_c(106);
  s.set_toggle_anim_a(6);
  s.set_toggle_anim_b(7);
  s.set_selection_anim(103);
  s.set_item_up_anim(20);
  s.set_item_down_anim(21);
  s.set_scroll_down_anim(33);
  s.set_scroll_up_anim(32);
  for (uint32_t i = 0; i < 8; i++) {
    Store(base, s.item_anims_address() + 4 * i, 10 + i);  // +9936 to +9964: 10 to 17
  }
  // The node table's words, each through [+9672] read again.
  auto table = [&] { return schema::PairedNodeTable(base, s.node_table().address); };
  table().set_node_set_a(10);
  table().set_node_set_b(9);
  for (uint32_t i = 0; i < 4; i++) {
    const auto t = table();
    Store(base, t.children_address() + 4 * i, 6 + i);  // +296 to +308: 6 to 9
  }
  for (uint32_t i = 0; i < 27; i++) {
    const auto t = table();
    Store(base, t.item_nodes_address() + 4 * i, 1 + i);  // +156 to +260: 1 to 27
    const auto u = table();
    Store(base, u.item_anims_address() + 4 * i, 46 + i);  // +16 to +120: 46 to 72
  }
  for (uint32_t i = 0; i < 8; i++) {
    const auto t = table();
    Store(base, t.more_anims_address() + 4 * i, 77 + i);  // +124 to +152: 77 to 84
  }
  table().set_extra_node_base(6);
  table().set_text_node_base(55);
}

uint32_t NodeScreen(uint8_t* base, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8225A3D8 NodeScreen::NodeScreen");
  const uint32_t frame = sp - kNodeScreenFrame;
  Store(base, sp + 20, screen);  // its caller's frame
  boot_mode::ScreenMachineConstruct(base, schema::NodeScreen(base, screen).machine_address());
  PairedNodeTable(base, screen);  // the screen's +0
  Store(base, frame + 92, 15);
  schema::NodeScreen(base, screen).set_vtable(0x820116B8);
  Store(base, frame + 84, 0);
  Store(base, frame + 88, 0);
  Store(base, frame + 80, 0);
  Store(base, frame + 100, schema::NodeScreen(base, screen).record_texts_address() + 40);
  Store(base, frame + 96, 10);
  const schema::NodeScreen s(base, Load(base, sp + 20));
  s.set_course_list_enabled(0);
  s.set_course_list_player(0);
  s.set_toggle_enabled(Load(base, frame + 84));
  s.set_toggle_player(Load(base, frame + 88));
  s.set_table_movie(15);
  const uint32_t runs = Load(base, frame + 100);
  s.set_movie(15);
  s.set_sub_scene(kNone);
  Fill(base, s.open_anim_address(), s.item_down_anim_address(), kNone);  // +10284 to +10340
  Fill(base, s.records_anim_address(), s.text_child_b_address(), kNone);  // +10392 to +10412
  s.set_arrow_on_frame(kNone);
  s.set_arrow_off_frame(kNone);
  // Three runs of ten words through the frame's word (+10416 to +10532).
  for (uint32_t i = 0; i < 10; i++) {
    Store(base, runs - 40 + 4 * i, kNone);
    Store(base, runs + 4 * i, kNone);
    Store(base, runs + 40 + 4 * i, kNone);
  }
  Fill(base, s.item_anims_address(), s.item_anims_address() + 36, kNone);  // +10352 to +10388
  return s.address;
}

void Type6NodeScreen(uint8_t* base, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82236C30 Type6NodeScreen::Type6NodeScreen");
  NodeScreen(base, screen, sp - kType6NodeScreenFrame);
  const schema::NodeScreen s(base, screen);
  s.set_vtable(0x82010A80);
  // The node table (the screen's +0).
  s.set_table_node_set_a(11);
  s.set_table_node_set_b(10);
  for (uint32_t i = 0; i < 27; i++) {
    Store(base, s.table_item_anims_address() + 4 * i, 48 + i);  // +16 to +120: 48 to 74
    Store(base, s.table_item_nodes_address() + 4 * i, 1 + i);   // +156 to +260: 1 to 27
  }
  for (uint32_t i = 0; i < 8; i++) {
    Store(base, s.table_more_anims_address() + 4 * i, 79 + i);  // +124 to +152: 79 to 86
  }
  for (uint32_t i = 0; i < 4; i++) {
    Store(base, s.table_children_address() + 4 * i, 6 + i);  // +296 to +308: 6 to 9
  }
  s.set_table_extra_node_base(6);
  s.set_table_text_node_base(55);
  s.set_sub_scene(8);
  s.set_cursor_node(5);
  s.set_open_anim(101);
  s.set_open_loop_b(0);
  s.set_shown_loop_a(47);
  s.set_shown_loop_b(102);
  s.set_shown_loop_c(43);
  s.set_toggle_anim_a(44);
  s.set_toggle_anim_b(45);
  s.set_item_up_anim(16);
  s.set_item_down_anim(17);
  s.set_scroll_down_anim(29);
  s.set_scroll_up_anim(28);
  s.set_records_anim(103);
  s.set_arrow_child_a(8);
  s.set_arrow_child_b(9);
  s.set_title_child(0);
  s.set_text_child_a(10);
  s.set_text_child_b(11);
  s.set_arrow_on_frame(0);
  s.set_arrow_off_frame(1);
  s.set_course_node(15);
  s.set_cursor_child(0);
  for (uint32_t i = 0; i < 6; i++) {
    Store(base, s.item_anims_address() + 4 * i, 6 + i);  // +10352 to +10372: 6 to 11
  }
  for (uint32_t i = 0; i < 10; i++) {
    Store(base, s.record_texts_address() + 4 * i, 30 + 5 * i);       // 30, 35, ... 75
    Store(base, s.record_texts_address() + 40 + 4 * i, 31 + 5 * i);  // 31, 36, ... 76
    Store(base, s.record_texts_address() + 80 + 4 * i, 32 + 5 * i);  // 32, 37, ... 77
  }
}

void Type6PanelScreen(uint8_t* base, uint32_t screen, uint32_t sp, const New& new_panel) {
  ITC_NATIVE_RUNNING("sub_822309C8 Type6PanelScreen::Type6PanelScreen");
  PanelScreen(base, screen, sp, new_panel,
              {0x82010920, PanelGroup6B, Panel6C, Panel6D, Panel6F, 11676});
}

void Type7PanelScreen(uint8_t* base, uint32_t screen, uint32_t sp, const New& new_panel) {
  ITC_NATIVE_RUNNING("sub_822403A0 Type7PanelScreen::Type7PanelScreen");
  PanelScreen(base, screen, sp, new_panel,
              {0x82010E90, PanelGroup7B, Panel7C, Panel7D, Panel7F, 11680});
}

void Type6Screens(uint8_t* base, uint32_t block, uint32_t sp, const New& new_panel) {
  ITC_NATIVE_RUNNING("sub_822366F0 Type6Screens::Type6Screens");
  const uint32_t members = sp - kScreensFrame;
  const schema::Type6Screens b(base, block);
  b.set_vtable(0x82010A78);
  Type6Screen0(base, block + 4);
  boot_mode::ScreenMachineConstruct(base, block + 9672);
  Type6ListScreen(base, b.list_screen_address());
  Type6NodeScreen(base, b.node_screen_address(), members);
  Type6PanelScreen(base, b.panel_screen_address(), members, new_panel);
  boot_mode::ScreenMachineConstruct(base, block + 52696);
  b.set_power_graph_vtable(0x82010A68);
  boot_mode::ScreenMachineConstruct(base, block + 62336);
  boot_mode::ScreenMachineConstruct(base, block + 71976);
  boot_mode::ScreenMachineConstruct(base, block + 81616);
  b.set_answer_word(0);
  b.set_answer(0);
  b.set_phase(0);
}

void Type7Screens(uint8_t* base, uint32_t block, uint32_t sp, const New& new_panel) {
  ITC_NATIVE_RUNNING("sub_82246B28 Type7Screens::Type7Screens");
  const uint32_t members = sp - kScreensFrame;
  const schema::Type7Screens b(base, block);
  b.set_vtable(0x82010FE0);
  Type7Screen0(base, block + 4);
  boot_mode::ScreenMachineConstruct(base, block + 9664);
  Type7ListScreen(base, b.list_screen_address());
  Type7PanelScreen(base, b.panel_screen_address(), members, new_panel);
  boot_mode::ScreenMachineConstruct(base, block + 42356);
  b.set_power_graph_vtable(0x82010FD0);
  boot_mode::ScreenMachineConstruct(base, block + 51996);
  boot_mode::ScreenMachineConstruct(base, block + 61636);
  b.set_answer_word(0);
  b.set_answer(0);
  b.set_phase(0);
}

}  // namespace screen_constructors
