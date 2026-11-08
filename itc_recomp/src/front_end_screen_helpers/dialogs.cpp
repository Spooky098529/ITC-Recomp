// The save screen's and the options' dialog text setters and the save
// screen's notice step (docs/functions/front-end-screen-helpers.md): see
// front_end_screen_helpers.h.

#include "front_end_screen_helpers/front_end_screen_helpers.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace front_end_screen_helpers {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(base, frame, sp);
  return frame;
}

// UiMovies::Find(id) as the originals make it: the id stored at `key` (a
// frame word), the movie table's pair for it, its movie.
uint32_t FindMovie(uint8_t* base, uint32_t id, uint32_t key) {
  Store(base, key, id);
  return Load(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, key));
}

// The varargs (r4 to r10) stored in the caller's parameter area, as the
// originals' prologues store them (doublewords).
void StoreVarargs(uint8_t* base, uint32_t sp, const uint64_t varargs[7]) {
  for (uint32_t i = 0; i < 7; i++) {
    Store(base, sp + 24 + 8 * i, uint32_t(varargs[i] >> 32));
    Store(base, sp + 28 + 8 * i, uint32_t(varargs[i]));
  }
}

// What a dialog's text setter does after child 0's text.
enum class Answers {
  kYesNo,       // children 3 and 4 strings 2 and 3
  kYesNoShown,  // the same, then child 8's +80 = 1
  kOne,         // child 3 string 4, child 4 cleared, child 8's +80 = 0
};

struct TextSetter {
  uint32_t movie_id, sub_scene;
  Answers answers;
  uint32_t lr_v, lr_third, lr_fourth;
};

uint64_t SetText(uint8_t* base, const TextSetter& s, uint64_t id, const uint64_t varargs[7],
                 uint32_t sp, const Calls& calls) {
  StoreVarargs(base, sp, varargs);
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, s.movie_id, frame + 80);
  const uint32_t node = ui_movies::FindNode(base, movie, s.sub_scene, 1);
  const uint32_t first = ui_movies::Child(base, node, 0);
  const uint32_t second = ui_movies::Child(base, node, 1);
  const uint32_t third = ui_movies::Child(base, second, 3);
  const uint32_t fourth = ui_movies::Child(base, second, 4);
  const uint32_t eighth = s.answers == Answers::kYesNo ? 0 : ui_movies::Child(base, second, 8);
  Store(base, frame + 80, sp + 24);
  calls.Call(kSetNodeTextV, frame, s.lr_v, movie, first, id, Load(base, frame + 80));
  uint64_t r3 = 0;
  if (s.answers == Answers::kOne) {
    calls.Call(kSetNodeText, frame, s.lr_third, movie, third, 4);
    r3 = calls.Call(kClearNodeText, frame, s.lr_fourth, movie, fourth);
    Store(base, eighth + 80, 0);
  } else {
    calls.Call(kSetNodeText, frame, s.lr_third, movie, third, 2);
    r3 = calls.Call(kSetNodeText, frame, s.lr_fourth, movie, fourth, 3);
    if (s.answers == Answers::kYesNoShown) {
      Store(base, eighth + 80, 1);
    }
  }
  return r3;
}

uint64_t SetNote(uint8_t* base, uint32_t movie_id, uint32_t sub_scene, uint32_t lr, uint64_t id,
                 const uint64_t varargs[7], uint32_t sp, const Calls& calls) {
  StoreVarargs(base, sp, varargs);
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, movie_id, frame + 80);
  const uint32_t node = ui_movies::Child(base, ui_movies::FindNode(base, movie, sub_scene, 2), 1);
  Store(base, frame + 80, sp + 24);
  return calls.Call(kSetNodeTextV, frame, lr, movie, node, id, Load(base, frame + 80));
}

}  // namespace

uint64_t SaveScreenSetText(uint8_t* base, uint64_t id, const uint64_t varargs[7], uint32_t sp,
                           const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A51C8 SaveScreen::SetText");
  return SetText(base, {44, 0, Answers::kYesNoShown, 0x821A528C, 0x821A529C, 0x821A52AC}, id,
                 varargs, sp, calls);
}

uint64_t SaveScreenSetNote(uint8_t* base, uint64_t id, const uint64_t varargs[7], uint32_t sp,
                           const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A52C0 SaveScreen::SetNote");
  return SetNote(base, 44, 0, 0x821A5348, id, varargs, sp, calls);
}

uint64_t SaveScreenSetMessage(uint8_t* base, uint64_t id, const uint64_t varargs[7], uint32_t sp,
                              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A5360 SaveScreen::SetMessage");
  return SetText(base, {44, 0, Answers::kOne, 0x821A5424, 0x821A5434, 0x821A5440}, id, varargs,
                 sp, calls);
}

uint64_t OptionsSetDialogText(uint8_t* base, uint32_t sub_scene, uint64_t id,
                              const uint64_t varargs[7], uint32_t sp, const Calls& calls) {
  if (sub_scene == 13) {
    ITC_NATIVE_RUNNING("sub_821C0D70 Options::SetSaveDialogText");
    return SetText(base, {7, 13, Answers::kYesNo, 0x821C0E24, 0x821C0E34, 0x821C0E44}, id,
                   varargs, sp, calls);
  }
  ITC_NATIVE_RUNNING("sub_821C0BF0 Options::SetCategory6DialogText");
  return SetText(base, {7, 14, Answers::kYesNo, 0x821C0CA4, 0x821C0CB4, 0x821C0CC4}, id, varargs,
                 sp, calls);
}

uint64_t OptionsSetDialogNote(uint8_t* base, uint32_t sub_scene, uint64_t id,
                              const uint64_t varargs[7], uint32_t sp, const Calls& calls) {
  if (sub_scene == 13) {
    ITC_NATIVE_RUNNING("sub_821C0E50 Options::SetSaveDialogNote");
    return SetNote(base, 7, 13, 0x821C0ED8, id, varargs, sp, calls);
  }
  ITC_NATIVE_RUNNING("sub_821C0CD0 Options::SetCategory6DialogNote");
  return SetNote(base, 7, 14, 0x821C0D58, id, varargs, sp, calls);
}

uint64_t SaveScreenShowNotice(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A63B0 SaveScreen::ShowNotice");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, 44, frame + 96);
  if (Load(base, step) == 0) {
    Store(base, step, 1);
    if (Load(base, Load(base, kSaveData) + 12) == 0) {
      const uint64_t varargs[7] = {0, 0, 0, 0, 0, 0, 0};
      SaveScreenSetMessage(base, 107, varargs, frame, calls);
    }
    if (Load(base, Load(base, kSaveData) + 16) == 0) {
      calls.Call(kPlayNamed, frame, 0x821A6438, movie, 12, 0, 0);
    }
    calls.Call(kMenuSoundPlay, frame, 0x821A6440, 5);
  }
  const uint64_t ran = calls.Call(kRun, frame, 0x821A644C, movie, 0);
  const bool done = uint32_t(ran) == 0;
  if (done && Load(base, Load(base, kSaveData) + 16) == 0) {
    calls.Call(kPlayNamed, frame, 0x821A6484, movie, 2, 0, 0);
    calls.Call(kLoopNamed, frame, 0x821A6490, movie, 2);
  }
  return done ? 1 : 0;
}

}  // namespace front_end_screen_helpers
