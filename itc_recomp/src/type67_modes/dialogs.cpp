// The type 6 and 7 dialogs' yes/no confirm (docs/functions/type6-type7-modes.md):
// the opens (Type6Dialog::OpenConfirm, Type7Dialog::OpenConfirm,
// Dialog::DrawWaiting) and the steps both dialogs' Update runs
// (Dialog::OpenConfirmStep, CloseConfirmStep, MoveCursorStep), over movie
// 16. UiMovies::Find, UiMovie::ResetPose, PlayNamed and LoopLast through
// their plain C++; MenuSound::Play's plain C++ with its slot call through
// Calls; UiMovie::Run, FinishAll and FinishNamed guest calls through their
// overrides, from frames like the originals'.

#include "type67_modes/type67_modes.h"
#include "type67_modes/views.h"

#include "base/log.h"
#include "game_modes/game_modes.h"
#include "guest.h"
#include "menu_helpers/menu_helpers.h"
#include "menu_sounds/menu_sounds.h"
#include "ui_movies/game.h"

namespace type67_modes {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using Dialog = schema::Type67Dialog;

constexpr uint32_t kDialogMovie = 16;
constexpr uint32_t kRun = 0x8211E1B8;
constexpr uint32_t kFinishNamed = 0x8211DF20;
constexpr uint32_t kFinishAll = 0x8211DFF8;

// Movie 16 through UiMovies::Find (the original's id at its frame's +80,
// a C++ value since E5 step 6).
uint32_t DialogMovie(uint8_t* base) {
  return schema::UiMovieWord(base, ui_movies::PairsFindKey(base, ui_movies::kMovieTable, kDialogMovie))
      .value();
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

// A message (`message`, 0, 0) queued in a dispatcher's ring (+204), as the
// pages do it inline.
void Post(uint8_t* base, uint32_t dispatcher, uint32_t message) {
  const schema::ModeDispatcher d(base, dispatcher);
  const uint32_t count = d.ring_count() + 1;
  const uint32_t end = d.ring_start() + count - 1;
  d.set_ring_count(count);
  const game_modes::RingSlot slot{d.ring() + (end - (end / 50) * 50) * 12};
  slot.set_arg0(0);
  slot.set_arg1(0);
  slot.set_id(message);
}

// The page left for a step: the previous page (+9648) kept unless the page
// (+9640) is 0, the next page 0, no step queued, `step` started, page -1.
void ToStep(const Dialog& dialog, uint32_t step) {
  const uint32_t page = dialog.page();
  if (page != 0) {
    dialog.set_previous_page(page);
  }
  dialog.set_step(step);
  dialog.set_next_page(0);
  dialog.set_queued_step(0xFFFFFFFF);
  dialog.set_page(0xFFFFFFFF);
}

// The reopen both opens share after ResetPose: the cursors, the pages, and
// the state (4 with the movies loaded, page 0 through step 0; else 1).
void ResetConfirm(uint8_t* base, const Dialog& dialog, uint32_t movie) {
  const uint32_t previous = dialog.menu_cursor();
  const uint32_t choice = LoadU32(base, movie + 40960);
  dialog.set_menu_previous(previous);
  dialog.set_menu_cursor(choice);
  dialog.set_next_page(0);
  dialog.set_page(0);
  dialog.set_previous_page(0);
  dialog.set_page_word_20(0xFFFFFFFF);
  dialog.set_step(0xFFFFFFFF);
  dialog.set_step_started(0);
  const uint32_t loaded = dialog.loaded();
  dialog.set_is_notice(0);
  if (loaded == 0) {
    dialog.set_state(1);
    return;
  }
  const uint32_t page = dialog.page();
  if (page != 0) {
    dialog.set_previous_page(page);
  }
  dialog.set_step(0);
  dialog.set_queued_step(0xFFFFFFFF);
  dialog.set_next_page(0);
  dialog.set_page(0xFFFFFFFF);
  dialog.set_state(4);
}

}  // namespace

uint64_t DialogOpenConfirm(uint8_t* base, uint32_t dialog, bool type7, uint32_t sp) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_82241B40 Type7Dialog::OpenConfirm");
  } else {
    ITC_NATIVE_RUNNING("sub_822320D8 Type6Dialog::OpenConfirm");
  }
  (void)sp;  // its frame's one slot (the movie id) a C++ value (E5 step 6)
  const Dialog d(base, dialog);
  d.set_movie(kDialogMovie);
  const uint32_t movie = DialogMovie(base);
  const uint64_t r3 = ui_movies::ResetPose(base, movie);
  ResetConfirm(base, d, movie);
  if (!type7) {
    d.set_type6_status(5);
  }
  return r3;
}

std::optional<uint64_t> DialogDrawWaiting(uint8_t* base, uint32_t dialog, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82241C18 Dialog::DrawWaiting");
  const Dialog d(base, dialog);
  if (d.reopen() == 0) {
    return std::nullopt;
  }
  (void)sp;  // its frame's one slot (the movie id) a C++ value (E5 step 6)
  d.set_reopen(0);
  d.set_movie(kDialogMovie);
  const uint32_t movie = DialogMovie(base);
  const uint64_t r3 = ui_movies::ResetPose(base, movie);
  ResetConfirm(base, d, movie);
  return r3;
}

uint64_t DialogOpenConfirmStep(uint8_t* base, uint32_t dialog, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822325E0 Dialog::OpenConfirmStep");
  const uint32_t frame = sp - 128;
  const Dialog d(base, dialog);
  const uint32_t movie = DialogMovie(base);
  if (d.step_started() == 0) {
    menu_sounds::PlayMenuSound(base, 5, frame, Sounds(calls));
    ui_movies::PlayNamed(base, movie, 5, 0, 0, 0, 0);
    d.set_step_started(1);
  }
  const bool done = uint32_t(calls.call(kRun, frame, 0x82232650, movie, 0)) == 0;
  if (done) {
    const uint32_t names[2] = {1, 2};  // the frame's +80 (E5 step 6: a C++ array)
    const uint32_t cursor = d.menu_cursor();
    const uint32_t named = names[cursor];
    ui_movies::PlayNamed(base, movie, named, 1, 0, 0, 0);
    ui_movies::LoopLast(base, movie);
  }
  return done ? 1 : 0;
}

uint64_t DialogCloseConfirmStep(uint8_t* base, uint32_t dialog, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223AA10 Dialog::CloseConfirmStep");
  const uint32_t frame = sp - 128;
  const Dialog d(base, dialog);
  const uint32_t movie = DialogMovie(base);
  if (d.step_started() == 0) {
    menu_sounds::PlayMenuSound(base, 5, frame, Sounds(calls));
    ui_movies::PlayNamed(base, movie, 6, 0, 0, 0, 0);
    d.set_step_started(1);
  }
  const bool done = uint32_t(calls.call(kRun, frame, 0x8223AA80, movie, 0)) == 0;
  if (done) {
    calls.call(kFinishAll, frame, 0x8223AA94, movie, 0);
    d.set_queued_state(9);
  }
  return done ? 1 : 0;
}

uint64_t DialogMoveCursorStep(uint8_t* base, uint32_t dialog, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223AB50 Dialog::MoveCursorStep");
  const uint32_t frame = sp - 128;
  const Dialog d(base, dialog);
  const uint32_t movie = DialogMovie(base);
  if (d.step_started() == 0) {
    const uint32_t previous = d.menu_previous();
    const uint32_t names[2] = {1, 2};  // the frame's +80 (E5 step 6: a C++ array)
    calls.call(kFinishNamed, frame, 0x8223ABB0, movie, names[previous]);
    const uint32_t cursor = d.menu_cursor();
    ui_movies::PlayNamed(base, movie, names[cursor], 1, 0, 0, 0);
    ui_movies::LoopLast(base, movie);
    d.set_step_started(1);
  }
  calls.call(kRun, frame, 0x8223ABF0, movie, 0);
  return 1;
}

uint64_t DialogNoticeStep(uint8_t* base, uint32_t dialog, bool close, uint32_t sp,
                          const Calls& calls) {
  if (close) {
    ITC_NATIVE_RUNNING("sub_8223AAA8 Dialog::CloseNoticeStep");
  } else {
    ITC_NATIVE_RUNNING("sub_822326A8 Dialog::OpenNoticeStep");
  }
  const uint32_t frame = sp - 112;
  StoreU32(base, frame, sp);
  // Movie 19 (the original's id at the frame's +80, a C++ value).
  const uint32_t movie =
      schema::UiMovieWord(base, ui_movies::PairsFindKey(base, ui_movies::kMovieTable, 19)).value();
  const Dialog d(base, dialog);
  if (d.step_started() == 0) {
    menu_sounds::PlayMenuSound(base, 5, frame, Sounds(calls));
    ui_movies::PlayNamed(base, movie, close ? 3 : 2, 0, 0, 0, 0);
    d.set_step_started(1);
  }
  const bool done =
      uint32_t(calls.call(kRun, frame, close ? 0x8223AB20 : 0x82232720, movie, 0)) == 0;
  if (done && close) {
    d.set_queued_state(9);
  }
  return done ? 1 : 0;
}

uint64_t DialogConfirmPage(uint8_t* base, uint64_t r3, uint32_t dispatcher, bool type7,
                           uint32_t sp, const Calls& calls) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_82241EF8 Type7Dialog::ConfirmPage");
  } else {
    ITC_NATIVE_RUNNING("sub_82232328 Type6Dialog::ConfirmPage");
  }
  const Dialog dialog(base, uint32_t(r3));
  const uint32_t frame = sp - 128;
  dialog.set_is_notice(0);
  if (DialogStatus(dialog, type7) == 4) {
    SetDialogStatus(dialog, type7, 1);
    return r3;
  }
  SetDialogStatus(dialog, type7, 1);
  const uint32_t menu = dialog.menu_address();
  menu_helpers::Step(base, menu, frame, calls.native_action_slots, Menus(calls));
  uint64_t answer =
      menu_helpers::Pressed(base, menu, 159, frame, calls.native_action_slots, Menus(calls));
  if (int32_t(answer) != 0) {
    answer = menu_sounds::PlayMenuSound(base, 0, frame, Sounds(calls));
    ToStep(dialog, 1);
    const bool yes = dialog.menu_cursor() == 0;
    Post(base, dispatcher, type7 ? (yes ? 8 : 9) : (yes ? 9 : 10));
    if (type7) {
      dialog.set_type7_status(2);
    }
    return answer;
  }
  answer = menu_helpers::Pressed(base, menu, 158, frame, calls.native_action_slots, Menus(calls));
  if (int32_t(answer) != 0) {
    answer = menu_sounds::PlayMenuSound(base, 2, frame, Sounds(calls));
    ToStep(dialog, 1);
    Post(base, dispatcher, type7 ? 11 : 12);
    if (type7) {
      dialog.set_type7_status(2);
    }
    return answer;
  }
  if (dialog.menu_moved() == 0) {
    return answer;
  }
  answer = menu_sounds::PlayMenuSound(base, 4, frame, Sounds(calls));
  ToStep(dialog, 4);
  return answer;
}

uint64_t DialogUpdate(uint8_t* base, uint64_t r3, uint32_t dispatcher, bool type7, uint32_t sp,
                      const Calls& calls) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_82242218 Type7Dialog::Update");
  } else {
    ITC_NATIVE_RUNNING("sub_82232798 Type6Dialog::Update");
  }
  const Dialog dialog(base, uint32_t(r3));
  const uint32_t frame = sp - 160;
  StoreU32(base, frame + 80, dialog.movie());
  const uint32_t movie =
      schema::UiMovieWord(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80)).value();
  const uint32_t step = dialog.step();
  // The steps' table: OpenConfirmStep, CloseConfirmStep, OpenNoticeStep,
  // CloseNoticeStep, MoveCursorStep.
  // (The original builds it at the frame's +96; read only here, so a C++
  // table since E5 step 6. The movie id at +80 stays: its address is the
  // step's r4.)
  static constexpr uint32_t kSteps[5] = {0x822325E0, 0x8223AA10, 0x822326A8, 0x8223AAA8,
                                         0x8223AB50};
  uint64_t answer = movie;
  if (step == 0xFFFFFFFF) {
    answer = calls.call(kRun, frame, type7 ? 0x82242304 : 0x82232884, movie, 0);
  } else {
    bool done = true;
    if (int32_t(step) >= 0) {
      const uint32_t target = kSteps[step];
      answer = calls.call(target, frame, type7 ? 0x822422B4 : 0x82232834, r3, frame + 80);
      done = int32_t(answer) != 0;
    }
    if (done) {
      const uint32_t next = dialog.queued_step();
      if (next == 0xFFFFFFFF) {
        dialog.set_page(dialog.next_page());
        dialog.set_step(0xFFFFFFFF);
      } else {
        dialog.set_page_word_20(0);
        dialog.set_step(next);
        dialog.set_queued_step(0xFFFFFFFF);
      }
      dialog.set_step_started(0);
    }
  }
  const uint32_t state = dialog.queued_state();
  if (state == 21) {
    return answer;
  }
  if (state == 9) {
    Post(base, dispatcher, type7 ? 12 : 13);
  }
  dialog.set_state(dialog.queued_state());
  if (type7) {
    dialog.set_queued_state(21);
    dialog.set_type7_status(3);
  } else {
    const uint32_t shown = dialog.type6_status();
    dialog.set_queued_state(21);
    if (shown != 5) {
      dialog.set_type6_status(3);
    }
  }
  return answer;
}

}  // namespace type67_modes
