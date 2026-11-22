// The race menu's start (docs/functions/race-menu.md, RaceMenu::Start): see
// race_menu.h.

#include "race_menu/race_menu.h"

#include <bit>

#include "base/log.h"
#include "guest.h"
#include "race_menu/internal.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_menu {

namespace {

constexpr uint32_t kResetPose = 0x8211ABA8;
constexpr uint32_t kFadeIn = 0x8209D038;     // ScreenFades::FadeIn, through its override

}  // namespace

uint64_t Start(uint8_t* base, uint64_t machine_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218C9C8 RaceMenu::Start");
  const uint32_t machine = uint32_t(machine_r3);
  const uint32_t frame = Frame(base, sp, 176);
  Machine(base, machine).set_answered(0);
  Machine(base, machine).set_previous_page(3);
  Machine(base, machine).set_refresh_texts(1);
  Machine(base, machine).set_back_from_question(0);
  Machine(base, machine).set_transitions_set_up(0);
  Machine(base, machine).set_takes_input(1);
  Machine(base, machine).set_unknown_9816(0);
  Machine(base, machine).set_race_mode(0);
  Machine(base, machine).set_previous_race_mode(0);
  Machine(base, machine).set_event_cursor(0);
  Machine(base, machine).set_previous_event_cursor(0);
  Machine(base, machine).set_scroll_flag(0);
  Machine(base, machine).set_visible_row(0);
  Machine(base, machine).set_previous_visible_row(0);
  Machine(base, machine).set_variant_cursor(0);
  Machine(base, machine).set_previous_variant_cursor(0);
  Machine(base, machine).set_answered_no(0);
  Machine(base, machine).set_menu_mode(0);
  Machine(base, machine).set_page(3);

  // Player 0's save data: the progress bits, the name, the money, the day
  // (at most 999) and its weekday (the remainder of a signed division).
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  Machine(base, machine).set_save(save);
  // The progress bits: the first word of the save's locations (0x199E0).
  Machine(base, machine).set_progress_bits(Load(base, CareerSave(base, save).locations_address()));
  const int32_t day = int32_t(Save(base, save).day());
  // The name: the save's +8 to +27 (the pick's 20 bytes), a word at a time.
  for (uint32_t k = 0; k < 20; k += 4) {
    Store(base, Machine(base, machine).player_name_address() + k,
          Load(base, Save(base, save).pick_head_address() + k));
  }
  Machine(base, machine).set_money(Save(base, save).money());
  Machine(base, machine).set_day(uint32_t(day > 999 ? 999 : day));
  Machine(base, machine).set_weekday(uint32_t(day - (day / 7) * 7));

  SetUpEvents(base, machine_r3, frame, calls);
  SetEventRange(base, machine);

  const uint32_t m = FindMovie(base, kMovie, frame + 80);
  calls.Call(kResetPose, frame, 0x8218CAE0, m);
  Refresh(base, machine_r3, frame, calls);

  // The nodes' floats (copied as words), each node's stores in the
  // original's order.
  const uint32_t zero = G::float_zero(base);
  const float zero_f = std::bit_cast<float>(zero);
  Node(base, ui_movies::FindNode(base, m, 19, 0)).set_y(zero_f);
  Node(base, ui_movies::FindNode(base, m, 20, 0)).set_y(zero_f);
  Node(base, ui_movies::FindNode(base, m, 8, 0)).set_y(zero_f);
  for (const uint32_t child : {2u, 3u, 4u}) {
    const Node node(base, ui_movies::FindNode(base, m, 11, child));
    node.set_x(std::bit_cast<float>(G::float_2228(base)));
    node.set_y(std::bit_cast<float>(G::float_a134(base)));
  }
  Node node(base, ui_movies::FindNode(base, m, 11, 0));
  node.set_y(zero_f);
  node.set_x(std::bit_cast<float>(G::float_c34(base)));
  node = Node(base, ui_movies::FindNode(base, m, 11, 1));
  node.set_x(std::bit_cast<float>(G::float_c34(base)));
  node.set_y(zero_f);

  // The badges (11, 20) to (11, 23) hidden, then shown by the progress bits.
  static constexpr uint32_t kHideLr[] = {0x8218CBE8, 0x8218CBF8, 0x8218CC08, 0x8218CC18};
  for (uint32_t k = 0; k < 4; k++) {
    calls.Call(kHideNode, frame, kHideLr[k], m, 11, 20 + k);
  }
  const uint32_t bits = Machine(base, machine).progress_bits();
  if ((bits & 0x20) != 0 || (bits & 0x4) != 0 || (bits & 0x8) != 0) {
    if ((bits & 0x20) != 0) {
      calls.Call(kShowNode, frame, 0x8218CC34, m, 11, 20);
    }
    calls.Call(kShowNode, frame, 0x8218CC44, m, 11, 21);
    calls.Call(kShowNode, frame, 0x8218CC54, m, 11, 22);
    calls.Call(kShowNode, frame, 0x8218CC64, m, 11, 23);
  } else if ((bits & 0x10) != 0) {
    calls.Call(kShowNode, frame, 0x8218CE00, m, 11, 21);
    calls.Call(kShowNode, frame, 0x8218CC64, m, 11, 22);
  } else {
    calls.Call(kShowNode, frame, 0x8218CC64, m, 11, 22);
  }
  node = Node(base, ui_movies::FindNode(base, m, 10, 0));
  node.set_x(zero_f);
  node.set_y(zero_f);

  // The page object on page 3, the menu.
  Pages pages = G::pages(base);
  uint32_t page = Machine(base, machine).page();
  pages.set_next_page(page);
  pages.set_page(page);
  pages.set_previous_page(page);
  pages.set_unknown_20(kNone);
  pages.set_step(kNone);
  pages.set_step_started(0);
  const uint32_t first = Machine(base, machine).menu_mode();
  const Menu menu = G::menu(base);
  menu.set_first_item(0);
  menu.set_last_item(0);
  menu.set_wraps(0);
  menu.set_moved(0);
  menu.set_list_wrapped(0);
  menu.set_first_row(0);
  menu.set_rows_shown(8);
  menu.set_cursor(first);
  menu.set_previous(first);
  menu.set_side(0);
  menu.set_player(0);
  menu.set_up_down_locked(0);
  menu.set_left_right_steps(0);
  // Then the page object again: transition 0, then page 3.
  pages = G::pages(base);
  page = Machine(base, machine).page();
  const uint32_t old = pages.page();
  if (page != old) {
    pages.set_previous_page(old);
  }
  pages.set_step(0);
  pages.set_queued_step(kNone);
  pages.set_next_page(page);
  pages.set_page(kNone);

  // The hub's music (cue 2 of bank 1) stopped when it plays; the menu's (cue
  // 0) played unless it plays.
  Store(base, frame + 88, 1);
  Store(base, frame + 84, 2);
  uint64_t r3 = CallSlot(base, calls, kIsCuePlayingSlot, frame, 0x8218CD3C, frame + 88, frame + 84);
  if (uint8_t(r3) != 0) {
    Store(base, frame + 92, 2);
    Store(base, frame + 96, 1);
    CallSlot(base, calls, kStopCueSlot, frame, 0x8218CD68, frame + 96, frame + 92);
  }
  Store(base, frame + 100, 0);
  Store(base, frame + 104, 1);
  r3 = CallSlot(base, calls, kIsCuePlayingSlot, frame, 0x8218CD8C, frame + 104, frame + 100);
  if (uint8_t(r3) == 0) {
    Store(base, frame + 80, 0);
    Store(base, frame + 108, 1);
    calls.Call(kSoundPlay, frame, 0x8218CDA8, frame + 108, frame + 80);
  }
  return calls.Call(kFadeIn, frame, 0x8218CDC4, G::fade_in_passes(base), 0, 0, 0);
}

}  // namespace race_menu
