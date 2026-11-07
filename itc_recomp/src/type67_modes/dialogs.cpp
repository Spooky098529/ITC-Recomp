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

}  // namespace type67_modes
