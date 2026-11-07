// The type 6 and 7 dialogs' yes/no confirm (docs/functions/type6-type7-modes.md):
// the opens (Type6Dialog::OpenConfirm, Type7Dialog::OpenConfirm,
// Dialog::DrawWaiting) and the steps both dialogs' Update runs
// (Dialog::OpenConfirmStep, CloseConfirmStep, MoveCursorStep), over movie
// 16. UiMovies::Find, UiMovie::ResetPose, PlayNamed and LoopLast through
// their plain C++; MenuSound::Play's plain C++ with its slot call through
// Calls; UiMovie::Run, FinishAll and FinishNamed guest calls through their
// overrides, from frames like the originals'.

#include "type67_modes/type67_modes.h"

#include "base/log.h"
#include "guest.h"
#include "menu_helpers/menu_helpers.h"
#include "menu_sounds/menu_sounds.h"
#include "ui_movies/game.h"

namespace type67_modes {

namespace {

using guest::LoadU32;
using guest::StoreU32;

constexpr uint32_t kDialogMovie = 16;
constexpr uint32_t kRun = 0x8211E1B8;
constexpr uint32_t kFinishNamed = 0x8211DF20;
constexpr uint32_t kFinishAll = 0x8211DFF8;

// Movie 16: its id at the frame's +80, then UiMovies::Find.
uint32_t DialogMovie(uint8_t* base, uint32_t frame) {
  StoreU32(base, frame + 80, kDialogMovie);
  return LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
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
  const uint32_t ring = dispatcher + 204;
  const uint32_t count = LoadU32(base, ring + 600) + 1;
  const uint32_t end = LoadU32(base, ring + 604) + count - 1;
  StoreU32(base, ring + 600, count);
  const uint32_t slot = ring + (end - (end / 50) * 50) * 12;
  StoreU32(base, slot + 4, 0);
  StoreU32(base, slot + 8, 0);
  StoreU32(base, slot + 0, message);
}

// The page left for a step: the previous page (+9648) kept unless the page
// (+9640) is 0, the next page 0, no step queued, `step` started, page -1.
void ToStep(uint8_t* base, uint32_t dialog, uint32_t step) {
  const uint32_t page = LoadU32(base, dialog + 9640);
  if (page != 0) {
    StoreU32(base, dialog + 9648, page);
  }
  StoreU32(base, dialog + 9652, step);
  StoreU32(base, dialog + 9644, 0);
  StoreU32(base, dialog + 9656, 0xFFFFFFFF);
  StoreU32(base, dialog + 9640, 0xFFFFFFFF);
}

// The reopen both opens share after ResetPose: the cursors, the pages, and
// the state (4 with the movies loaded, page 0 through step 0; else 1).
void ResetConfirm(uint8_t* base, uint32_t dialog, uint32_t movie) {
  const uint32_t previous = LoadU32(base, dialog + 9668);
  const uint32_t choice = LoadU32(base, movie + 40960);
  StoreU32(base, dialog + 9672, previous);
  StoreU32(base, dialog + 9668, choice);
  StoreU32(base, dialog + 9644, 0);
  StoreU32(base, dialog + 9640, 0);
  StoreU32(base, dialog + 9648, 0);
  StoreU32(base, dialog + 9660, 0xFFFFFFFF);
  StoreU32(base, dialog + 9652, 0xFFFFFFFF);
  StoreU32(base, dialog + 9664, 0);
  const uint32_t loaded = LoadU32(base, dialog + 9608);
  StoreU32(base, dialog + 9752, 0);
  if (loaded == 0) {
    StoreU32(base, dialog + 9604, 1);
    return;
  }
  const uint32_t page = LoadU32(base, dialog + 9640);
  if (page != 0) {
    StoreU32(base, dialog + 9648, page);
  }
  StoreU32(base, dialog + 9652, 0);
  StoreU32(base, dialog + 9656, 0xFFFFFFFF);
  StoreU32(base, dialog + 9644, 0);
  StoreU32(base, dialog + 9640, 0xFFFFFFFF);
  StoreU32(base, dialog + 9604, 4);
}

}  // namespace

uint64_t DialogOpenConfirm(uint8_t* base, uint32_t dialog, bool type7, uint32_t sp) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_82241B40 Type7Dialog::OpenConfirm");
  } else {
    ITC_NATIVE_RUNNING("sub_822320D8 Type6Dialog::OpenConfirm");
  }
  const uint32_t frame = sp - 112;
  StoreU32(base, frame + 80, kDialogMovie);
  StoreU32(base, dialog + 9744, kDialogMovie);
  const uint32_t movie =
      LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
  const uint64_t r3 = ui_movies::ResetPose(base, movie);
  ResetConfirm(base, dialog, movie);
  if (!type7) {
    StoreU32(base, dialog + 9756, 5);
  }
  return r3;
}

std::optional<uint64_t> DialogDrawWaiting(uint8_t* base, uint32_t dialog, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82241C18 Dialog::DrawWaiting");
  if (LoadU32(base, dialog + 9748) == 0) {
    return std::nullopt;
  }
  const uint32_t frame = sp - 128;
  StoreU32(base, frame + 80, kDialogMovie);
  StoreU32(base, dialog + 9748, 0);
  StoreU32(base, dialog + 9744, kDialogMovie);
  const uint32_t movie =
      LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
  const uint64_t r3 = ui_movies::ResetPose(base, movie);
  ResetConfirm(base, dialog, movie);
  return r3;
}

uint64_t DialogOpenConfirmStep(uint8_t* base, uint32_t dialog, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822325E0 Dialog::OpenConfirmStep");
  const uint32_t frame = sp - 128;
  const uint32_t movie = DialogMovie(base, frame);
  if (LoadU32(base, dialog + 9664) == 0) {
    menu_sounds::PlayMenuSound(base, 5, frame, Sounds(calls));
    ui_movies::PlayNamed(base, movie, 5, 0, 0, 0, 0);
    StoreU32(base, dialog + 9664, 1);
  }
  const bool done = uint32_t(calls.call(kRun, frame, 0x82232650, movie, 0)) == 0;
  if (done) {
    StoreU32(base, frame + 80, 1);
    StoreU32(base, frame + 84, 2);
    const uint32_t cursor = LoadU32(base, dialog + 9668);
    const uint32_t named = LoadU32(base, frame + 80 + (cursor << 2));
    ui_movies::PlayNamed(base, movie, named, 1, 0, 0, 0);
    ui_movies::LoopLast(base, movie);
  }
  return done ? 1 : 0;
}

uint64_t DialogCloseConfirmStep(uint8_t* base, uint32_t dialog, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223AA10 Dialog::CloseConfirmStep");
  const uint32_t frame = sp - 128;
  const uint32_t movie = DialogMovie(base, frame);
  if (LoadU32(base, dialog + 9664) == 0) {
    menu_sounds::PlayMenuSound(base, 5, frame, Sounds(calls));
    ui_movies::PlayNamed(base, movie, 6, 0, 0, 0, 0);
    StoreU32(base, dialog + 9664, 1);
  }
  const bool done = uint32_t(calls.call(kRun, frame, 0x8223AA80, movie, 0)) == 0;
  if (done) {
    calls.call(kFinishAll, frame, 0x8223AA94, movie, 0);
    StoreU32(base, dialog + 9740, 9);
  }
  return done ? 1 : 0;
}

uint64_t DialogMoveCursorStep(uint8_t* base, uint32_t dialog, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223AB50 Dialog::MoveCursorStep");
  const uint32_t frame = sp - 128;
  const uint32_t movie = DialogMovie(base, frame);
  if (LoadU32(base, dialog + 9664) == 0) {
    const uint32_t previous = LoadU32(base, dialog + 9672);
    StoreU32(base, frame + 84, 2);
    StoreU32(base, frame + 80, 1);
    calls.call(kFinishNamed, frame, 0x8223ABB0, movie,
               LoadU32(base, frame + 80 + (previous << 2)));
    const uint32_t cursor = LoadU32(base, dialog + 9668);
    ui_movies::PlayNamed(base, movie, LoadU32(base, frame + 80 + (cursor << 2)), 1, 0, 0, 0);
    ui_movies::LoopLast(base, movie);
    StoreU32(base, dialog + 9664, 1);
  }
  calls.call(kRun, frame, 0x8223ABF0, movie, 0);
  return 1;
}

uint64_t DialogConfirmPage(uint8_t* base, uint64_t r3, uint32_t dispatcher, bool type7,
                           uint32_t sp, const Calls& calls) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_82241EF8 Type7Dialog::ConfirmPage");
  } else {
    ITC_NATIVE_RUNNING("sub_82232328 Type6Dialog::ConfirmPage");
  }
  const uint32_t dialog = uint32_t(r3);
  const uint32_t shown = type7 ? 9760 : 9756;
  const uint32_t frame = sp - 128;
  StoreU32(base, dialog + 9752, 0);
  if (LoadU32(base, dialog + shown) == 4) {
    StoreU32(base, dialog + shown, 1);
    return r3;
  }
  StoreU32(base, dialog + shown, 1);
  const uint32_t menu = dialog + 9668;
  menu_helpers::Step(base, menu, frame, calls.native_action_slots, Menus(calls));
  uint64_t answer =
      menu_helpers::Pressed(base, menu, 159, frame, calls.native_action_slots, Menus(calls));
  if (int32_t(answer) != 0) {
    answer = menu_sounds::PlayMenuSound(base, 0, frame, Sounds(calls));
    ToStep(base, dialog, 1);
    const bool yes = LoadU32(base, menu) == 0;
    Post(base, dispatcher, type7 ? (yes ? 8 : 9) : (yes ? 9 : 10));
    if (type7) {
      StoreU32(base, dialog + 9760, 2);
    }
    return answer;
  }
  answer = menu_helpers::Pressed(base, menu, 158, frame, calls.native_action_slots, Menus(calls));
  if (int32_t(answer) != 0) {
    answer = menu_sounds::PlayMenuSound(base, 2, frame, Sounds(calls));
    ToStep(base, dialog, 1);
    Post(base, dispatcher, type7 ? 11 : 12);
    if (type7) {
      StoreU32(base, dialog + 9760, 2);
    }
    return answer;
  }
  if (LoadU32(base, dialog + 9676) == 0) {
    return answer;
  }
  answer = menu_sounds::PlayMenuSound(base, 4, frame, Sounds(calls));
  ToStep(base, dialog, 4);
  return answer;
}

uint64_t DialogUpdate(uint8_t* base, uint64_t r3, uint32_t dispatcher, bool type7, uint32_t sp,
                      const Calls& calls) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_82242218 Type7Dialog::Update");
  } else {
    ITC_NATIVE_RUNNING("sub_82232798 Type6Dialog::Update");
  }
  const uint32_t dialog = uint32_t(r3);
  const uint32_t frame = sp - 160;
  StoreU32(base, frame + 80, LoadU32(base, dialog + 9744));
  const uint32_t movie =
      LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
  const uint32_t step = LoadU32(base, dialog + 9652);
  // The steps' table: OpenConfirmStep, CloseConfirmStep, OpenNoticeStep,
  // CloseNoticeStep, MoveCursorStep.
  static constexpr uint32_t kSteps[5] = {0x822325E0, 0x8223AA10, 0x822326A8, 0x8223AAA8,
                                         0x8223AB50};
  for (uint32_t i = 0; i < 5; i++) {
    StoreU32(base, frame + 96 + 4 * i, kSteps[i]);
  }
  uint64_t answer = movie;
  if (step == 0xFFFFFFFF) {
    answer = calls.call(kRun, frame, type7 ? 0x82242304 : 0x82232884, movie, 0);
  } else {
    bool done = true;
    if (int32_t(step) >= 0) {
      const uint32_t target = LoadU32(base, frame + 96 + (step << 2));
      answer = calls.call(target, frame, type7 ? 0x822422B4 : 0x82232834, r3, frame + 80);
      done = int32_t(answer) != 0;
    }
    if (done) {
      const uint32_t p = dialog + 9640;
      const uint32_t next = LoadU32(base, p + 16);
      if (next == 0xFFFFFFFF) {
        StoreU32(base, p + 0, LoadU32(base, p + 4));
        StoreU32(base, p + 12, 0xFFFFFFFF);
      } else {
        StoreU32(base, p + 20, 0);
        StoreU32(base, p + 12, next);
        StoreU32(base, p + 16, 0xFFFFFFFF);
      }
      StoreU32(base, p + 24, 0);
    }
  }
  const uint32_t state = LoadU32(base, dialog + 9740);
  if (state == 21) {
    return answer;
  }
  if (state == 9) {
    Post(base, dispatcher, type7 ? 12 : 13);
  }
  StoreU32(base, dialog + 9604, LoadU32(base, dialog + 9740));
  if (type7) {
    StoreU32(base, dialog + 9740, 21);
    StoreU32(base, dialog + 9760, 3);
  } else {
    const uint32_t shown = LoadU32(base, dialog + 9756);
    StoreU32(base, dialog + 9740, 21);
    if (shown != 5) {
      StoreU32(base, dialog + 9756, 3);
    }
  }
  return answer;
}

}  // namespace type67_modes
