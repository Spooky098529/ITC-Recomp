// The type 8 mode's dialog (docs/functions/type8-mode.md): Type8Dialog's
// handlers, pages and step 0, over movies 16 to 20. See type8_mode.h.
// UiMovies::Find, IsLoaded, UiMovie::ResetPose, PlayNamed and LoopLast
// through their plain C++, as the type 6 and 7 dialogs' (src/type67_modes/
// dialogs.cpp); Menu::Step, Pressed and MenuSound::Play's plain C++ with
// their slot calls through Calls; UiMovie::Run, UiMovies::Open and the
// pages and steps the tables name as guest calls through their overrides.

#include "type8_mode/type8_mode.h"

#include "base/log.h"
#include "guest.h"
#include "menu_helpers/menu_helpers.h"
#include "menu_sounds/menu_sounds.h"
#include "schema/boot_mode.h"
#include "schema/game_modes.h"
#include "schema/menu_helpers.h"
#include "schema/type8_mode.h"
#include "ui_movies/game.h"

namespace type8_mode {

namespace {

using guest::LoadU32;
using guest::StoreU32;

using D = schema::Type8Dialog;
using Pages = schema::PageObject;
using Menu = schema::FrontEndMenu;
using Machine = schema::StateMachine;
using G = schema::Type8ModeGlobals;

constexpr uint32_t kMinusOne = 0xFFFFFFFF;
constexpr uint32_t kRun = 0x8211E1B8;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(base, frame, sp);
  return frame;
}

// UiMovies::Find of the id stored at `at` (its pairs' value).
uint32_t FindMovie(uint8_t* base, uint32_t at) {
  return LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, at));
}

uint64_t SoundThunk(void* user, uint32_t function, const uint64_t args[4], int set, uint32_t r1,
                    uint32_t lr) {
  return static_cast<const Calls*>(user)->call_set(function, args, set, r1, lr);
}

menu_sounds::Calls Sounds(const Calls& calls) {
  return {const_cast<Calls*>(&calls), &SoundThunk};
}

uint64_t MenuThunk(void* user, uint32_t function, uint64_t r3, uint64_t r4, uint32_t r1,
                   uint32_t lr) {
  return static_cast<const Calls*>(user)->call(function, r1, lr, r3, r4);
}

menu_helpers::Calls Menus(const Calls& calls) {
  return {const_cast<Calls*>(&calls), &MenuThunk};
}

// The dispatcher's ring's next record, its count raised.
schema::ModeMessage NextRecord(uint8_t* base, uint32_t dispatcher) {
  const schema::ModeDispatcher d(base, dispatcher);
  const uint32_t count = d.ring_count() + 1;
  const uint32_t end = d.ring_start() + count - 1;
  d.set_ring_count(count);
  return schema::ModeMessage(
      base, d.ring_address() + (end - (end / 50) * 50) * schema::ModeMessage::kSize);
}

// A message (`message`, 0, 0) posted, the id first.
void Post(uint8_t* base, uint32_t dispatcher, uint32_t message) {
  const schema::ModeMessage slot = NextRecord(base, dispatcher);
  slot.set_id(message);
  slot.set_arg0(0);
  slot.set_arg1(0);
}

// The confirm's reopen OpenConfirm and DrawWaiting share after ResetPose
// (movie `movie`): the cursors, the pages, then the state.
void ResetConfirm(uint8_t* base, uint32_t dialog, uint32_t movie) {
  const D d(base, dialog);
  const Pages pages(base, d.pages_address());
  const Menu menu(base, d.menu_address());
  const Machine machine(base, d.machine_address());
  const uint32_t previous = menu.cursor();
  const uint32_t choice = LoadU32(base, movie + 40960);  // the UiMovie's cursor
  menu.set_previous(previous);
  menu.set_cursor(choice);
  pages.set_next_page(0);
  pages.set_page(0);
  pages.set_previous_page(0);
  pages.set_unknown_20(kMinusOne);
  pages.set_step(kMinusOne);
  pages.set_step_started(0);
  if (machine.loaded() == 0) {
    machine.set_state(1);
    return;
  }
  // The page (0, as just stored) kept as the previous page unless 0.
  pages.set_step(0);
  pages.set_queued_step(kMinusOne);
  pages.set_next_page(0);
  pages.set_page(kMinusOne);
  machine.set_state(4);
}

// The page left for step `step` with `next` the page after it: the page
// kept as the previous page unless it's `keep_unless`.
void ToStep(uint8_t* base, uint32_t dialog, uint32_t step, uint32_t next, uint32_t keep_unless) {
  const Pages pages(base, D(base, dialog).pages_address());
  const uint32_t page = pages.page();
  if (page != keep_unless) {
    pages.set_previous_page(page);
  }
  pages.set_queued_step(kMinusOne);
  pages.set_page(kMinusOne);
  pages.set_step(step);
  pages.set_next_page(next);
}

}  // namespace

uint64_t DialogOnEnter(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8223A408 Type8Dialog::OnEnter");
  const D d(base, uint32_t(r3));
  const Pages pages(base, d.pages_address());
  const Menu menu(base, d.menu_address());
  const Machine machine(base, d.machine_address());
  machine.set_loaded(0);
  pages.set_next_page(0);
  pages.set_page(0);
  pages.set_previous_page(0);
  pages.set_unknown_20(kMinusOne);
  pages.set_step(kMinusOne);
  pages.set_step_started(0);
  menu.set_first_item(0);
  menu.set_last_item(1);
  menu.set_wraps(1);
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
  d.set_shown(0);
  d.set_queued_state(21);
  d.set_movie(0);
  const uint32_t open = machine.reenter();
  d.set_reopen(0);
  machine.set_state(int32_t(open) != 0 ? 1 : 9);
  return r3;
}

uint64_t DialogOpenMovies(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223A398 Type8Dialog::OpenMovies");
  const uint32_t frame = Frame(base, sp, 96);
  for (uint32_t i = 0; i < 4; i++) {
    calls.open(16 + i, 22559 + i, frame, 0x8223A3B0 + 12 * i);
  }
  return calls.open(20, 22587, frame, 0x8223A3E0);
}

uint64_t DialogWaitForMovies(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223ADC0 Type8Dialog::WaitForMovies");
  const D d(base, uint32_t(r3));
  const Machine machine(base, d.machine_address());
  Frame(base, sp, 96);
  if (int32_t(machine.loaded()) != 0) {
    d.set_shown(1);
    machine.set_state(9);
    return r3;
  }
  uint64_t answer = r3;
  for (uint32_t id = 16; id <= 20; id++) {
    answer = ui_movies::IsLoaded(base, id);
    if (int32_t(answer) == 0) {
      return answer;
    }
  }
  machine.set_loaded(1);
  return answer;
}

uint64_t DialogDraw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223AC00 Type8Dialog::Draw");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t page = Pages(base, D(base, uint32_t(r3)).pages_address()).page();
  StoreU32(base, frame + 80, 0x8223A6B8);
  StoreU32(base, frame + 84, 0x8223A858);
  if (page == kMinusOne) {
    return r3;
  }
  const uint32_t target = LoadU32(base, frame + 80 + (page << 2));
  return calls.call(target, frame, 0x8223AC44, r3, r4);
}

uint64_t DialogUpdate(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223AC58 Type8Dialog::Update");
  const D d(base, uint32_t(r3));
  const Pages pages(base, d.pages_address());
  const uint32_t frame = Frame(base, sp, 160);
  StoreU32(base, frame + 80, d.movie());
  const uint32_t movie = FindMovie(base, frame + 80);
  const uint32_t step = pages.step();
  // The steps: OpenConfirmStep, Dialog::CloseConfirmStep, OpenNoticeStep,
  // CloseNoticeStep, MoveCursorStep.
  static constexpr uint32_t kSteps[5] = {0x8223A940, 0x8223AA10, 0x822326A8, 0x8223AAA8,
                                         0x8223AB50};
  for (uint32_t i = 0; i < 5; i++) {
    StoreU32(base, frame + 96 + 4 * i, kSteps[i]);
  }
  uint64_t answer = movie;
  if (step == kMinusOne) {
    answer = calls.call(kRun, frame, 0x8223AD44, movie, 0);
  } else {
    bool done = true;
    if (int32_t(step) >= 0) {
      const uint32_t target = LoadU32(base, frame + 96 + (step << 2));
      answer = calls.call(target, frame, 0x8223ACF4, r3, frame + 80);
      done = int32_t(answer) != 0;
    }
    if (done) {
      const uint32_t next = pages.queued_step();
      if (next == kMinusOne) {
        pages.set_page(pages.next_page());
        pages.set_step(kMinusOne);
      } else {
        pages.set_unknown_20(0);
        pages.set_step(next);
        pages.set_queued_step(kMinusOne);
      }
      pages.set_step_started(0);
    }
  }
  const uint32_t state = d.queued_state();
  if (state == 21) {
    return answer;
  }
  if (state == 9) {
    Post(base, dispatcher, 13);
  }
  Machine(base, d.machine_address()).set_state(d.queued_state());
  d.set_queued_state(21);
  return answer;
}

uint64_t DialogOpenConfirm(uint8_t* base, uint32_t dialog, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223A510 Type8Dialog::OpenConfirm");
  const uint32_t frame = Frame(base, sp, 112);
  const D d(base, dialog);
  StoreU32(base, frame + 80, 16);
  d.set_movie(16);
  const uint32_t movie = FindMovie(base, frame + 80);
  const uint64_t r3 = ui_movies::ResetPose(base, movie);
  Menu(base, d.menu_address()).set_player(G::dialog_player(base));
  ResetConfirm(base, dialog, movie);
  return r3;
}

uint64_t DialogOpenNotice(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8223A4A0 Type8Dialog::OpenNotice");
  const D d(base, uint32_t(r3));
  const Pages pages(base, d.pages_address());
  const Machine machine(base, d.machine_address());
  d.set_movie(19);
  pages.set_next_page(1);
  pages.set_page(1);
  pages.set_previous_page(1);
  pages.set_unknown_20(kMinusOne);
  pages.set_step(kMinusOne);
  pages.set_step_started(0);
  if (machine.loaded() == 0) {
    machine.set_state(1);
    return r3;
  }
  // The page (1, as just stored) kept unless 1.
  pages.set_queued_step(kMinusOne);
  pages.set_next_page(1);
  pages.set_page(kMinusOne);
  pages.set_step(2);
  machine.set_state(4);
  return r3;
}

uint64_t DialogDrawWaiting(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223A5F0 Type8Dialog::DrawWaiting");
  const uint32_t dialog = uint32_t(r3);
  const D d(base, dialog);
  const uint32_t frame = Frame(base, sp, 128);
  if (d.reopen() == 0) {
    return r3;
  }
  StoreU32(base, frame + 80, 16);
  d.set_reopen(0);
  d.set_movie(16);
  const uint32_t movie = FindMovie(base, frame + 80);
  const uint64_t answer = ui_movies::ResetPose(base, movie);
  ResetConfirm(base, dialog, movie);
  return answer;
}

uint64_t DialogConfirmPage(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                           const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223A6B8 Type8Dialog::ConfirmPage");
  const uint32_t dialog = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t menu = D(base, dialog).menu_address();
  menu_helpers::Step(base, menu, frame, calls.native_action_slots, Menus(calls));
  uint64_t answer =
      menu_helpers::Pressed(base, menu, 159, frame, calls.native_action_slots, Menus(calls));
  if (int32_t(answer) != 0) {
    answer = menu_sounds::PlayMenuSound(base, 0, frame, Sounds(calls));
    ToStep(base, dialog, 1, 0, 0);
    const bool yes = int32_t(Menu(base, menu).cursor()) == 0;
    const schema::ModeMessage slot = NextRecord(base, dispatcher);
    slot.set_arg0(0);
    slot.set_arg1(0);
    slot.set_id(yes ? 9 : 10);
    return answer;
  }
  answer = menu_helpers::Pressed(base, menu, 158, frame, calls.native_action_slots, Menus(calls));
  if (int32_t(answer) != 0) {
    answer = menu_sounds::PlayMenuSound(base, 2, frame, Sounds(calls));
    ToStep(base, dialog, 1, 0, 0);
    Post(base, dispatcher, 12);
    return answer;
  }
  if (int32_t(Menu(base, menu).moved()) == 0) {
    return answer;
  }
  answer = menu_sounds::PlayMenuSound(base, 4, frame, Sounds(calls));
  ToStep(base, dialog, 4, 0, 0);
  return answer;
}

uint64_t DialogNoticePage(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                          const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223A858 Type8Dialog::NoticePage");
  const uint32_t dialog = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t menu = D(base, dialog).menu_address();
  menu_helpers::Step(base, menu, frame, calls.native_action_slots, Menus(calls));
  uint64_t answer =
      menu_helpers::Pressed(base, menu, 159, frame, calls.native_action_slots, Menus(calls));
  if (int32_t(answer) == 0) {
    answer =
        menu_helpers::Pressed(base, menu, 158, frame, calls.native_action_slots, Menus(calls));
    if (int32_t(answer) == 0) {
      return answer;
    }
  }
  const bool accept = int32_t(menu_helpers::Pressed(base, menu, 159, frame,
                                                    calls.native_action_slots, Menus(calls))) != 0;
  answer = menu_sounds::PlayMenuSound(base, accept ? 0 : 2, frame, Sounds(calls));
  ToStep(base, dialog, 3, 1, 1);
  Post(base, dispatcher, 11);
  return answer;
}

uint64_t DialogOpenConfirmStep(uint8_t* base, uint32_t dialog, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223A940 Type8Dialog::OpenConfirmStep");
  const uint32_t frame = Frame(base, sp, 128);
  StoreU32(base, frame + 80, 16);
  const uint32_t movie = FindMovie(base, frame + 80);
  const D d(base, dialog);
  const Pages pages(base, d.pages_address());
  if (pages.step_started() == 0) {
    menu_sounds::PlayMenuSound(base, 5, frame, Sounds(calls));
    ui_movies::PlayNamed(base, movie, 5, 0, 0, 0, 0);
    pages.set_step_started(1);
  }
  const bool done = uint32_t(calls.call(kRun, frame, 0x8223A9B0, movie, 0)) == 0;
  if (done) {
    ui_movies::ResetPose(base, movie);
    const uint32_t cursor = Menu(base, d.menu_address()).cursor();
    StoreU32(base, frame + 80, 1);
    StoreU32(base, frame + 84, 2);
    const uint32_t named = LoadU32(base, frame + 80 + (cursor << 2));
    ui_movies::PlayNamed(base, movie, named, 1, 0, 0, 0);
    ui_movies::LoopLast(base, movie);
  }
  return done ? 1 : 0;
}

}  // namespace type8_mode
