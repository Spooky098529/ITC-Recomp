// The type 8 panel screen's panels and panel group
// (docs/functions/type8-screens.md): see type8_screens.h. Read from the
// generated code; the panels' bases are the screen constructors' plain C++
// (src/screen_constructors/) and the car list's (src/car_list/).

#include "type8_screens/type8_screens.h"

#include "base/log.h"
#include "guest.h"
#include "schema/type8_screens.h"
#include "screen_constructors/screen_constructors.h"

namespace type8_screens {

namespace {

using guest::LoadU32;
using guest::StoreU32;

namespace sc = screen_constructors;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// The words by the player: player 0's (the original's unsigned player < 1)
// or player 1's.
uint32_t By(uint32_t player, uint32_t first, uint32_t second) {
  return player < 1 ? first : second;
}

uint64_t ThroughCalls(void* user, uint32_t function, const uint64_t args[8], const double*,
                      uint32_t r1, uint32_t lr) {
  const Calls& calls = *static_cast<const Calls*>(user);
  return calls.call8(function, args, r1, lr);
}

}  // namespace

car_list::Calls CarListCalls(const Calls& calls) {
  return {const_cast<Calls*>(&calls), &ThroughCalls};
}

uint64_t PanelGroup8Construct(uint8_t* base, uint64_t r3, uint32_t movie, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82239C10 PanelGroup8::PanelGroup8");
  const uint32_t group = uint32_t(r3);
  const schema::Type8PanelGroup g(base, group);
  Frame(base, sp, 112);
  sc::PanelGroupBase(base, group);
  g.set_movie(movie);
  g.set_text_node_set(10);
  g.set_vtable(kGroupVtable);
  g.set_caption_node(0);
  return r3;
}

uint64_t Panel8FConstruct(uint8_t* base, uint64_t r3, uint32_t player, uint32_t group,
                          uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82239C70 Panel8F::Panel8F");
  const uint32_t p = uint32_t(r3);
  const schema::Type8TabPanel f(base, p);
  Frame(base, sp, 112);
  sc::PanelFBase(base, p);
  f.set_player(player);
  f.set_group(group);
  f.set_buttons_node(11);
  f.set_vtable(kListNodeVtable);
  f.set_tab_word_a(1);
  f.set_tab_word_b(0);
  f.set_tab0_text(5);
  f.set_tab1_text(6);
  f.set_cursor(0);
  f.set_tab2_text(7);
  f.set_buttons(4);
  f.set_show_anim(369);
  f.set_loop_a(354);
  f.set_loop_b(358);
  f.set_texts_node(5);
  f.set_tabs_node(4);
  f.set_node_set(By(player, 6, 7));
  f.set_tab_anims(By(player, 15, 186));
  f.set_scroll_up(By(player, 26, 197));
  f.set_scroll_down(By(player, 25, 196));
  f.set_move_anim(By(player, 12, 183));
  return r3;
}

uint64_t Panel8EConstruct(uint8_t* base, uint64_t r3, uint32_t player, uint32_t group,
                          uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82239D58 Panel8E::Panel8E");
  const uint32_t p = uint32_t(r3);
  const schema::Type8RowPanel e(base, p);
  Frame(base, sp, 112);
  sc::PanelEBase(base, p);
  e.set_player(player);
  e.set_group(group);
  e.set_row_node0(1);
  e.set_row_count(3);
  e.set_row_node2(3);
  e.set_row_node1(2);
  e.set_spare_row_node(4);
  e.set_logo(3);
  e.set_text_maker(5);
  e.set_text_name(6);
  e.set_text_third(7);
  e.set_node(7);
  e.set_cursor(6);
  e.set_node_set(By(player, 6, 7));
  e.set_unknown_64(By(player, 15, 186));
  e.set_unknown_68(By(player, 25, 196));
  e.set_unknown_72(By(player, 26, 197));
  e.set_show_anim(By(player, 177, 348));
  e.set_row_anims(By(player, 65, 236));
  e.set_arrow_down(By(player, 171, 342));
  e.set_arrow_up(By(player, 170, 341));
  e.set_scroll_down(By(player, 70, 241));
  e.set_scroll_up(By(player, 69, 240));
  e.set_arrows(12);
  e.set_hide_anim(By(player, 180, 351));
  return r3;
}

uint64_t Panel8CConstruct(uint8_t* base, uint64_t r3, uint32_t player, uint32_t group,
                          uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82239E78 Panel8C::Panel8C");
  const uint32_t p = uint32_t(r3);
  const schema::Type8CarPanel c(base, p);
  Frame(base, sp, 128);
  sc::PanelCBase(base, p);
  // The shared words, in the original's order.
  c.set_group(group);
  c.set_player(player);
  c.set_buttons_node(11);
  c.set_stats_child3(13);
  c.set_stats_child2(16);
  c.set_stats_child5(25);
  c.set_stats_child9(11);
  c.set_text_name(10);
  c.set_vtable(kPanel8CVtable);
  c.set_stats_child8(14);
  c.set_stats_child11(18);
  c.set_stats_child12(23);
  c.set_stats_child14(15);
  c.set_stats_child0(10);
  c.set_texts(5);
  c.set_stats_child13(24);
  c.set_stats_child15(12);
  c.set_stats_child16(20);
  c.set_stats_child17(26);
  c.set_stats_child18(22);
  c.set_stats_child20(21);
  c.set_shows_price(0);
  c.set_text_third(8);
  c.set_text_maker(6);
  c.set_stats_child4(19);
  c.set_stats_child6(9);
  c.set_stats_child10(17);
  c.set_stats_child19(28);
  c.set_stats_child21(27);
  c.set_buttons(4);
  c.set_cursor(0);
  c.set_show_again_anim(0xFFFFFFFFu);
  c.set_text_a(123);
  c.set_text_b(112);
  c.set_node(9);
  c.set_arrows_child(8);
  // The player's words.
  if (player < 1) {
    c.set_name_node(19);
    c.set_node_set(6);
    c.set_stats_node(2);
    c.set_show_anim_a(178);
    c.set_show_anim_b(367);
    c.set_action_anim0(74);
    c.set_hide_anim_a(181);
    c.set_hide_anim_b(368);
    c.set_action_anim1(75);
    c.set_scroll_back(84);
    c.set_scroll_on(85);
    c.set_arrow_a(173);
    c.set_stats_out(372);
    c.set_arrow_b(172);
    c.set_stats_in(370);
  } else {
    c.set_name_node(17);
    c.set_node_set(7);
    c.set_stats_node(3);
    c.set_show_anim_a(349);
    c.set_action_anim0(245);
    c.set_hide_anim_a(352);
    c.set_show_anim_b(365);
    c.set_hide_anim_b(366);
    c.set_action_anim1(246);
    c.set_scroll_back(255);
    c.set_arrow_a(344);
    c.set_scroll_on(256);
    c.set_stats_out(373);
    c.set_arrow_b(343);
    c.set_stats_in(371);
  }
  return r3;
}

uint64_t Panel8DConstruct(uint8_t* base, uint64_t r3, uint32_t player, uint32_t group,
                          uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223A100 Panel8D::Panel8D");
  const uint32_t p = uint32_t(r3);
  const schema::Type8ColorPanel d(base, p);
  Frame(base, sp, 112);
  sc::PanelDBase(base, p);
  d.set_player(player);
  d.set_group(group);
  d.set_swatches(1);
  d.set_cursor(0);
  d.set_swatch_color(2);
  d.set_arrow_down_node(17);
  d.set_vtable(kListNodeVtable);
  d.set_node(11);
  d.set_arrows_node(10);
  d.set_arrow_up_node(16);
  d.set_node_set(By(player, 6, 7));
  d.set_show_anim(By(player, 179, 350));
  d.set_item_anims(By(player, 157, 328));
  d.set_hide_anim(By(player, 182, 353));
  d.set_arrow_b(By(player, 175, 346));
  d.set_arrow_a(By(player, 174, 345));
  d.set_swatches_back(By(player, 169, 340));
  d.set_swatches_on(By(player, 168, 339));
  d.set_scroll_on(By(player, 167, 338));
  d.set_scroll_back(By(player, 166, 337));
  return r3;
}

uint64_t ConfirmPanel8Construct(uint8_t* base, uint64_t r3, uint32_t player, uint32_t group,
                                uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223A208 ConfirmPanel8::ConfirmPanel8");
  const uint32_t p = uint32_t(r3);
  const schema::Type8ConfirmPanel c(base, p);
  Frame(base, sp, 112);
  car_list::ConfirmPanelConstruct(base, r3);
  c.set_player(player);
  c.set_group(group);
  c.set_node_c(3);
  c.set_vtable(kEmptyBaseVtable);
  c.set_unknown_12(By(player, 0, 1));
  c.set_node_a(By(player, 6, 7));
  c.set_node_b(2);
  c.set_hide_anim(By(player, 2, 5));
  c.set_loop_anim(By(player, 1, 4));
  c.set_show_anim(By(player, 374, 375));
  return r3;
}

uint64_t PanelGroup8DeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223A2A8 PanelGroup8::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 112);
  car_list::GroupBaseDestruct(base, r3);
  if (flags & 1) {
    const uint64_t args[6] = {r3, flags, 0, 0, 0, 0};
    calls.call_set(car_list::kOperatorDelete, args, 2, frame, 0x8223A2D8);
  }
  return r3;
}

uint64_t Panel8CDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223A348 Panel8C::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 112);
  car_list::CarPanelBaseDestruct(base, r3);
  if (flags & 1) {
    const uint64_t args[6] = {r3, flags, 0, 0, 0, 0};
    calls.call_set(car_list::kOperatorDelete, args, 2, frame, 0x8223A378);
  }
  return r3;
}

uint64_t Panel8CSetStats(uint8_t* base, uint64_t r3, uint32_t record, uint32_t sp,
                         const car_list::Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223A090 Panel8C::SetStats");
  return car_list::Panel6CSetStats(base, uint32_t(r3), record, sp, calls);
}

uint64_t Panel8CShowArrows(uint8_t* base, uint64_t r3, uint64_t shown, uint32_t sp,
                           const car_list::Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223A098 Panel8C::ShowArrows");
  const uint32_t p = uint32_t(r3), frame = Frame(base, sp, 112);
  const schema::Type8CarPanel c(base, p);
  if (int32_t(c.arrows_child()) < 0) {
    return r3;
  }
  const uint32_t m =
      uint32_t(type67_screens::PanelGroupMovie(base, c.group().address, frame));
  const uint32_t child = c.arrows_child();
  const uint32_t sub_scene = c.node_set();
  if (uint32_t(shown) == 0) {
    return calls.Call(car_list::kHideNode, frame, 0x8223A0E4, m, sub_scene, child);
  }
  return calls.Call(car_list::kShowNode, frame, 0x8223A0DC, m, sub_scene, child);
}

}  // namespace type8_screens
