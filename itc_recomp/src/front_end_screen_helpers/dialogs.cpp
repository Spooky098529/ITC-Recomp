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

namespace {

// MenuList::Init([0x826D8450], 1, 1, 0, 154, 153, 155, 156, 159, 158): the
// last two on the frame's +84 and +92 (the originals store them first).
uint64_t InitSaveList(uint8_t* base, uint32_t frame, uint32_t lr, const Calls& calls) {
  Store(base, frame + 92, 158);
  Store(base, frame + 84, 159);
  const uint64_t args[8] = {Load(base, kSaveList), 1, 1, 0, 154, 153, 155, 156};
  return calls.CallArgs(kMenuListInit, frame, lr, args);
}

// A page block's change to `page`: +8 = +0 unless +0 is `page`, then the
// stores in the order given by the caller.
void PageFrom(uint8_t* base, uint32_t block, uint32_t page) {
  const uint32_t now = Load(base, block);
  if (int32_t(now) != int32_t(page)) {
    Store(base, block + 8, now);
  }
}

uint32_t SaveData(uint8_t* base) { return Load(base, kSaveData); }

constexpr uint64_t kNoVarargs[7] = {0, 0, 0, 0, 0, 0, 0};

}  // namespace

uint64_t SaveScreenStartCheck(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A5450 SaveScreen::StartCheck");
  const uint32_t frame = Frame(base, sp, 128);
  Store(base, frame + 92, 158);
  Store(base, frame + 84, 159);
  Store(base, SaveData(base) + 4, 0);
  Store(base, SaveData(base) + 0, 1);
  Store(base, SaveData(base) + 28, 0);
  const uint64_t args[8] = {Load(base, kSaveList), 1, 1, 0, 154, 153, 155, 156};
  calls.CallArgs(kMenuListInit, frame, 0x821A54B8, args);
  const uint64_t state = calls.Call(kSaveSlotsState, frame, 0x821A54C0, 0);
  uint64_t r3 = 0;
  if (int32_t(uint32_t(state)) != 0) {
    r3 = SaveScreenSetNote(base, 70, kNoVarargs, frame, calls);
    Store(base, SaveData(base) + 12, 3);
    Store(base, SaveData(base) + 24, 0);
  } else {
    r3 = SaveScreenSetNote(base, 69, kNoVarargs, frame, calls);
    Store(base, SaveData(base) + 12, 2);
  }
  Store(base, SaveData(base) + 16, 1);
  const uint32_t pages = Load(base, kSavePages);
  PageFrom(base, pages, 1);
  Store(base, pages + 12, 0);
  Store(base, pages + 4, 1);
  Store(base, pages + 16, 0xFFFFFFFFu);
  Store(base, pages + 0, 0xFFFFFFFFu);
  Store(base, SaveData(base) + 56, 1);
  return r3;
}

uint64_t SaveScreenStart(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A5540 SaveScreen::Start");
  const uint32_t frame = Frame(base, sp, 144);
  Store(base, SaveData(base) + 0, 1);
  const uint64_t state = calls.Call(kSaveSlotsState, frame, 0x821A5564, 0);
  InitSaveList(base, frame, 0x821A55A0, calls);
  Store(base, SaveData(base) + 28, 0);
  Store(base, SaveData(base) + 56, 1);
  uint64_t r3 = 0;
  const uint32_t pages_global = kSavePages;
  if (int32_t(uint32_t(state)) != 0) {
    r3 = SaveScreenSetNote(base, 70, kNoVarargs, frame, calls);
    Store(base, SaveData(base) + 12, 0);
    Store(base, SaveData(base) + 16, 1);
    const uint32_t pages = Load(base, pages_global);
    PageFrom(base, pages, 6);
    Store(base, pages + 16, 0xFFFFFFFFu);
    Store(base, pages + 12, 3);
    Store(base, pages + 4, 6);
    Store(base, pages + 0, 0xFFFFFFFFu);
    Store(base, SaveData(base) + 24, 0);
  } else {
    r3 = SaveScreenSetText(base, 102, kNoVarargs, frame, calls);
    Store(base, SaveData(base) + 12, 3);
    Store(base, SaveData(base) + 16, 0);
    const uint32_t pages = Load(base, pages_global);
    PageFrom(base, pages, 5);
    Store(base, pages + 12, 3);
    Store(base, pages + 16, 0xFFFFFFFFu);
    Store(base, pages + 4, 5);
    Store(base, pages + 0, 0xFFFFFFFFu);
  }
  return r3;
}

uint64_t OptionsSetUpDialogPage(uint8_t* base, uint32_t sub_scene, uint32_t sp,
                                const Calls& calls) {
  const bool save = sub_scene == 13;
  if (save) {
    ITC_NATIVE_RUNNING("sub_821C6D48 Options::SetUpSavePage");
  } else {
    ITC_NATIVE_RUNNING("sub_821C6E50 Options::SetUpCategory6Page");
  }
  // Page 9's words (+620 the state, +628 the kind, +632) and block
  // [0x826D847C], or page 10's (+640, +648, +652) and [0x826D8488].
  const uint32_t state_at = save ? 620 : 640, kind_at = save ? 628 : 648;
  const uint32_t pages_global = save ? kOptionsSavePages : kOptionsCategory6Pages;
  const uint32_t lr_state = save ? 0x821C6D7C : 0x821C6E84;
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, 7, frame + 96);
  const uint64_t state = calls.Call(kSaveSlotsState, frame, lr_state, 0);
  if (int32_t(uint32_t(state)) != 0) {
    OptionsSetDialogNote(base, sub_scene, 70, kNoVarargs, frame, calls);
    Store(base, Load(base, kOptions) + kind_at, 3);
    Store(base, Load(base, kOptions) + kind_at + 4, 0);
  } else {
    OptionsSetDialogNote(base, sub_scene, 69, kNoVarargs, frame, calls);
    Store(base, Load(base, kOptions) + kind_at, 2);
  }
  auto prompts = [&](uint32_t lr) {
    const uint64_t none = 0xFFFFFFFFFFFFFFFFull;
    const uint64_t args[8] = {0, 0, none, none, none, none, 0, 0};
    // ShowPrompts reads r3 to r8; r9 and r10 as the previous call leaves
    // them (0 here).
    calls.CallArgs(kShowPrompts, frame, lr, args);
  };
  if (save) {
    prompts(0x821C6DE8);
    Store(base, Load(base, kOptions) + state_at, 1);
  } else {
    Store(base, Load(base, kOptions) + state_at, 1);
  }
  const uint32_t pages = Load(base, pages_global);
  PageFrom(base, pages, 2);
  Store(base, pages + 0, 2);
  Store(base, pages + 12, 0xFFFFFFFFu);
  Store(base, pages + 16, 0xFFFFFFFFu);
  if (!save) {
    prompts(0x821C6F24);
  }
  const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, 0);
  Store(base, node + 80, 0);
  // SaveBuffers::Create with r3 the node (FindNode's, which it leaves when
  // the buffers are there).
  const uint64_t r3 =
      calls.Call(kSaveBuffersCreate, frame, save ? 0x821C6E34 : 0x821C6F3C, node);
  Store(base, Load(base, kOptions) + 712, 0);
  Store(base, Load(base, kOptions) + 708, 0);
  return r3;
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
