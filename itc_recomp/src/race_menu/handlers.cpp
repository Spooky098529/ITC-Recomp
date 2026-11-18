// The race menu's machine handlers (docs/functions/race-menu.md): see
// race_menu.h.

#include "race_menu/race_menu.h"

#include "base/log.h"
#include "race_menu/internal.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_menu {

namespace {

constexpr uint32_t kMemCommit = 4096;
constexpr uint32_t kReadWrite = 4;
constexpr uint32_t kMemRelease = 0x8000;
// Player 0's save's answer to the question.
constexpr uint32_t kSaveAnswer = 0x1F870;

}  // namespace

uint64_t Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82188E20 RaceMenu::Create");
  const uint32_t frame = Frame(base, sp, 144);
  const Machine m(base, machine);
  m.set_page(0);
  G::set_pages(base, 0u);
  G::set_menu(base, 0u);
  m.set_loaded(0);
  m.set_menu_mode(0);
  m.set_unknown_9652(1);
  m.set_kept_event_cursor(0);
  m.set_variant(0);
  m.set_unknown_9728(0);
  m.set_history_depth(0);
  m.set_refresh_texts(1);
  m.set_takes_input(1);
  m.set_answered(0);
  m.set_race_set_up(0);
  m.set_unknown_9800(0);
  m.set_unknown_9804(0);
  // The page history (11 words) and the debug list (15 words of 15).
  for (uint32_t i = 0; i < 11; i++) {
    Store(base, m.page_history_address() + 4 * i, 0);
  }
  for (uint32_t i = 0; i < 15; i++) {
    Store(base, m.debug_list_address() + 4 * i, 15);
  }
  m.set_debug_count(0);
  Store(base, m.debug_list_address(), 7);
  uint32_t count = m.debug_count() + 1;
  m.set_debug_count(count);
  Store(base, m.debug_list_address() + 4 * count, 8);
  count = m.debug_count() + 1;
  m.set_debug_count(count);
  const uint32_t pages =
      uint32_t(calls.Call(kVirtualAlloc, frame, 0x82188F08, 0, 28, kMemCommit, kReadWrite));
  G::set_pages(base, pages);
  const uint32_t pages2 =
      uint32_t(calls.Call(kVirtualAlloc, frame, 0x82188F20, 0, 28, kMemCommit, kReadWrite));
  G::set_pages2(base, pages2);
  const uint64_t r3 = calls.Call(kVirtualAlloc, frame, 0x82188F3C, 0, 68, kMemCommit, kReadWrite);
  G::set_menu(base, uint32_t(r3));
  const Pages p = G::pages(base);
  p.set_step_started(0);
  p.set_unknown_20(kNone);
  p.set_next_page(3);
  p.set_page(3);
  p.set_previous_page(3);
  p.set_step(kNone);
  const Pages p2 = G::pages2(base);
  p2.set_next_page(kNone);
  p2.set_page(kNone);
  p2.set_previous_page(kNone);
  p2.set_unknown_20(kNone);
  p2.set_step(kNone);
  p2.set_step_started(0);
  const Menu menu = G::menu(base);
  menu.set_first_item(0);
  menu.set_last_item(0);
  menu.set_wraps(0);
  menu.set_cursor(0);
  menu.set_previous(0);
  menu.set_moved(0);
  menu.set_wrapped(0);
  menu.set_visible_row(0);
  menu.set_previous_row(0);
  menu.set_side(0);
  menu.set_player(0);
  menu.set_up_down_locked(0);
  menu.set_left_right_steps(0);
  menu.set_shoulders_step(0);
  m.set_state(m.reenter() != 0 ? 1 : 9);
  return r3;
}

uint64_t OpenMovie(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821889E0 RaceMenu::OpenMovie");
  const uint32_t frame = Frame(base, sp, 96);
  const uint64_t r3 = calls.Call(kMoviesOpen, frame, 0x82188A00, kMovie, kAsset);
  Machine(base, machine).set_loaded(0);
  return r3;
}

uint64_t WaitForMovie(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821890D8 RaceMenu::WaitForMovie");
  const Machine m(base, uint32_t(r3));
  Frame(base, sp, 112);
  if (m.loaded() == 0) {
    r3 = ui_movies::IsLoaded(base, kMovie);
    if (uint32_t(r3) != 0) {
      m.set_loaded(1);
    }
  }
  if (m.loaded() != 0) {
    const uint32_t reenter = m.reenter();
    m.set_refresh_texts(1);
    m.set_state(reenter != 0 ? 9 : 4);
  }
  return r3;
}

uint64_t OnAnswer(uint8_t* base, uint32_t machine, bool yes, uint32_t sp) {
  if (yes) {
    ITC_NATIVE_RUNNING("sub_821895F0 RaceMenu::OnYes");
  } else {
    ITC_NATIVE_RUNNING("sub_82189648 RaceMenu::OnNo");
  }
  Frame(base, sp, 112);
  const Machine m(base, machine);
  m.set_answered(1);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  Store(base, save + kSaveAnswer, yes ? 1 : 0);
  m.set_answered_no(yes ? 0 : 1);
  return save;
}

void OnBack(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82188938 RaceMenu::OnBack");
  const Machine m(base, machine);
  const uint32_t before = m.previous_page();
  m.set_back_from_question(1);
  m.set_answered_no(0);
  Pages pages;
  if (before == 5) {
    const uint32_t choice = m.variant_cursor();
    m.set_page(before);
    m.set_refresh_texts(1);
    m.set_previous_variant_cursor(choice);
    pages = G::pages(base);
    const uint32_t page = pages.page();
    if (page != 5) {
      pages.set_previous_page(page);
    }
    pages.set_queued_step(before);
    pages.set_page(kNone);
  } else if (before == 4) {
    m.set_variant_cursor(0);
    m.set_previous_variant_cursor(0);
    m.set_page(before);
    m.set_refresh_texts(1);
    pages = G::pages(base);
    const uint32_t page = pages.page();
    if (page != 4) {
      pages.set_previous_page(page);
    }
    pages.set_queued_step(3);
    pages.set_page(kNone);
  } else {
    return;
  }
  pages.set_next_page(before);
  pages.set_step(12);
}

uint64_t OnDialogClosed(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821896A0 RaceMenu::OnDialogClosed");
  const uint32_t frame = Frame(base, sp, 128);
  const Machine m(base, machine);
  const uint32_t pair = FindPair(base, kMovie, frame + 80);
  uint64_t r3 = pair;
  const uint32_t movie = Load(base, pair);
  if (m.answered() != 0) {
    calls.Call(kFadeOut, frame, 0x821896F4, G::fade_out_passes(base), 0, 0, 0);
    r3 = calls.Call(kRun, frame, 0x82189700, movie, 0);
    m.set_page(10);
    const Pages pages = G::pages(base);
    const uint32_t page = pages.page();
    if (page != 10) {
      pages.set_previous_page(page);
    }
    pages.set_page(10);
    pages.set_step(kNone);
    pages.set_queued_step(kNone);
  }
  m.set_takes_input(1);
  return r3;
}

uint64_t Unload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218AF08 RaceMenu::Unload");
  const uint32_t frame = Frame(base, sp, 128);
  const Machine m(base, machine);
  // The movies to delete (1 and 16) beside the key.
  Store(base, frame + 88, 1);
  Store(base, frame + 92, 16);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  uint64_t r3 = calls.Call(kFinishAll, frame, 0x8218AF44, movie);
  if (m.delete_movies() != 0) {
    for (uint32_t k = 0; k < 2; k++) {
      r3 = calls.Call(kMoviesDelete, frame, 0x8218AF60, Load(base, frame + 88 + 4 * k));
    }
    m.set_loaded(0);
  }
  return r3;
}

void LeaveToHub(uint8_t* base, uint32_t machine, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_8218AF80 RaceMenu::LeaveToHub");
  const Machine m(base, machine);
  Post(base, mode, m.leave_message());
  m.set_state(9);
}

uint64_t Destroy(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82188FE0 RaceMenu::Destroy");
  const uint32_t frame = Frame(base, sp, 112);
  const Machine m(base, machine);
  calls.Call(kVirtualFree, frame, 0x82189014, G::pages(base).address, 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x82189028, G::pages2(base).address, 0, kMemRelease);
  const uint64_t r3 =
      calls.Call(kVirtualFree, frame, 0x8218903C, G::menu(base).address, 0, kMemRelease);
  uint32_t state;
  if (m.day() == 2) {
    // The boot movies: movie 1, then the mode after them.
    const uint32_t list = G::boot_movie_list_address();
    for (uint32_t k = 0; k < 5; k++) {
      Store(base, list + 4 * k, 11);
    }
    Store(base, list + 20, 13);
    Store(base, list + 28, 0);
    Store(base, list + 24, 1);
    Store(base, list + 0, 1);
    Store(base, list + 20, m.race_set_up() == 1 ? 1 : 5);
    state = 20;
  } else {
    state = m.race_set_up() == 1 ? 11 : 12;
  }
  m.set_state(state);
  return r3;
}

void Back(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82188A20 RaceMenu::Back");
  const Machine m(base, machine);
  const uint8_t depth = uint8_t(m.history_depth() + 255);
  m.set_history_depth(depth);
  const uint32_t page = Load(base, m.page_history_address() + 4 * uint32_t(depth));
  m.set_page(page);
  const Pages pages = G::pages(base);
  pages.set_next_page(page);
  pages.set_page(page);
  pages.set_previous_page(page);
  pages.set_unknown_20(kNone);
  pages.set_step(kNone);
  pages.set_step_started(0);
  const Pages pages2 = G::pages2(base);
  pages2.set_next_page(kNone);
  pages2.set_page(kNone);
  pages2.set_previous_page(kNone);
  pages2.set_unknown_20(kNone);
  pages2.set_step(kNone);
  pages2.set_step_started(0);
  m.set_refresh_texts(1);
}

void SetEventRange(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82189158 RaceMenu::SetEventRange");
  const Machine m(base, machine);
  const uint32_t bits = m.progress_bits();
  uint32_t last, variants;
  if ((bits & 0x20) != 0) {
    last = 14;
    variants = 9;
    m.set_first_event(0);
  } else if ((bits & 0x4) != 0 || (bits & 0x8) != 0) {
    last = 12;
    variants = 8;
    m.set_first_event(0);
  } else {
    m.set_first_event(0);
    if ((bits & 0x10) != 0) {
      last = 7;
      variants = 5;
    } else {
      last = 5;
      variants = 4;
    }
  }
  m.set_variant_events(variants);
  m.set_last_event(last);
}

}  // namespace race_menu
