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

// --- The dialogs' steps (ShowMessage and the options' dialogs) ---

namespace {

// A kind's question: its string (0: none), the varargs' r4 (the others 0:
// the originals' are the earlier calls' leftovers, the parking area's
// call, 2026-10-30), the default answer it sets (-1: none) and whether it
// turns the sign-in watch's checks back on (0x826BAB40 +0 and +4 = 0).
struct Kind {
  uint16_t string;
  uint16_t r4;
  int8_t answer;
  bool watch;
};

struct ShowLrs {
  uint32_t play12, list_init, set_selection, play13_open, play13_close, sound, run, selection,
      play_selection, play0, loop;
};

struct ShowSpec {
  const Kind* kinds;
  uint32_t kind_count;
  ShowLrs lrs;
};

constexpr Kind kShowKinds[12] = {{62, 0, -1, false}, {61, 0, 1, false}, {0, 0, -1, false},
                                 {0, 0, -1, false},  {71, 0, 0, false}, {72, 0, 1, false},
                                 {65, 0, 1, false},  {66, 240, -1, false}, {63, 0, -1, false},
                                 {64, 0, -1, false}, {67, 0, -1, false}, {65, 0, 1, false}};
constexpr Kind kShow3Kinds[17] = {
    {70, 0, -1, true},  {59, 0, 0, false},  {59, 0, 0, false},  {102, 0, 0, false},
    {87, 0, -1, false}, {60, 0, -1, false}, {88, 0, -1, false}, {103, 0, -1, false},
    {61, 0, 1, false},  {62, 0, -1, false}, {63, 0, -1, false}, {64, 0, -1, false},
    {65, 0, 1, false},  {65, 0, 1, false},  {65, 0, 1, false},  {66, 240, 0, false},
    {67, 0, 0, false}};
constexpr Kind kShow4Kinds[13] = {
    {62, 0, -1, false}, {89, 0, 1, false},  {70, 0, -1, false}, {90, 0, 1, false},
    {91, 0, -1, false}, {92, 0, 1, false},  {92, 0, 1, false},  {93, 0, -1, false},
    {94, 0, -1, false}, {95, 0, -1, false}, {98, 0, -1, false}, {103, 0, -1, false},
    {92, 0, -1, false}};

constexpr ShowSpec kShows[3] = {
    {kShowKinds, 12,
     {0x821A588C, 0x821A58C0, 0x821A58D0, 0x821A5838, 0x821A57D4, 0x821A58E0, 0x821A58F0,
      0x821A5940, 0x821A5964, 0x821A5930, 0x821A5970}},
    {kShow3Kinds, 17,
     {0x821A5C0C, 0x821A5C40, 0x821A5C50, 0x821A5BB8, 0x821A5B54, 0x821A5C60, 0x821A5C70,
      0x821A5CC0, 0x821A5CE4, 0x821A5CB0, 0x821A5CF0}},
    {kShow4Kinds, 13,
     {0x821A5F40, 0x821A5F74, 0x821A5F84, 0x821A5EE8, 0x821A5E80, 0x821A5F94, 0x821A5FA4,
      0x821A5FF4, 0x821A6018, 0x821A5FE4, 0x821A6024}}};

constexpr uint32_t kSignInWatch = 0x826BAB40;

uint64_t PlayNamed(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie,
                   uint64_t named) {
  return calls.Call(kPlayNamed, frame, lr, movie, named, 0, 0);
}

// The question and its note, both with the kind's string (their plain C++).
template <typename Text, typename Note>
const Kind* AskKind(const Kind* kinds, uint32_t count, uint32_t kind, Text text, Note note) {
  if (kind >= count || kinds[kind].string == 0) {
    return nullptr;
  }
  const Kind& k = kinds[kind];
  const uint64_t varargs[7] = {k.r4, 0, 0, 0, 0, 0, 0};
  text(k.string, varargs);
  note(k.string, varargs);
  return &k;
}

}  // namespace

uint64_t SaveScreenShowMessage(uint8_t* base, uint32_t which, uint32_t step, uint32_t sp,
                               const Calls& calls) {
  if (which == 0) {
    ITC_NATIVE_RUNNING("sub_821A5668 SaveScreen::ShowMessage");
  } else if (which == 1) {
    ITC_NATIVE_RUNNING("sub_821A5980 SaveScreen::ShowMessage3");
  } else {
    ITC_NATIVE_RUNNING("sub_821A5D00 SaveScreen::ShowMessage4");
  }
  const ShowSpec& spec = kShows[which];
  const ShowLrs& lr = spec.lrs;
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t movie = FindMovie(base, 44, frame + 96);
  if (Load(base, step) == 0) {
    const uint32_t kind = Load(base, SaveData(base) + 12);
    const Kind* k = AskKind(
        spec.kinds, spec.kind_count, kind,
        [&](uint32_t id, const uint64_t* v) { SaveScreenSetText(base, id, v, frame, calls); },
        [&](uint32_t id, const uint64_t* v) { SaveScreenSetNote(base, id, v, frame, calls); });
    if (k != nullptr && k->watch) {
      Store(base, kSignInWatch + 0, 0);
      Store(base, kSignInWatch + 4, 0);
    }
    if (k != nullptr && k->answer >= 0) {
      Store(base, SaveData(base) + 48, uint32_t(k->answer));
    }
    const uint32_t state = Load(base, SaveData(base) + 16);
    if (state == 0) {
      PlayNamed(calls, frame, lr.play12, movie, 12);
      Store(base, frame + 84, 159);
      Store(base, frame + 92, 158);
      const uint64_t args[8] = {Load(base, kSaveList), 2, 2, 1, 154, 153, 155, 156};
      calls.CallArgs(kMenuListInit, frame, lr.list_init, args);
      calls.Call(kListSetSelection, frame, lr.set_selection, Load(base, kSaveList),
                 Load(base, SaveData(base) + 48));
      Store(base, SaveData(base) + 56, 1);
    } else if (state == 1) {
      PlayNamed(calls, frame, lr.play13_open, movie, 13);
      Store(base, ui_movies::FindNode(base, movie, 0, 0) + 80, 0);
      Store(base, ui_movies::Child(base, ui_movies::FindNode(base, movie, 0, 2), 0) + 80, 1);
      Store(base, SaveData(base) + 56, 1);
    } else if (state == 2) {
      PlayNamed(calls, frame, lr.play13_close, movie, 13);
      Store(base, ui_movies::FindNode(base, movie, 0, 0) + 80, 1);
      Store(base, ui_movies::Child(base, ui_movies::FindNode(base, movie, 0, 2), 0) + 80, 0);
      Store(base, SaveData(base) + 24, 0);
      Store(base, SaveData(base) + 56, 0);
    }
    calls.Call(kMenuSoundPlay, frame, lr.sound, 5);
    Store(base, step, 1);
  }
  const bool done = uint32_t(calls.Call(kRun, frame, lr.run, movie, 0)) == 0;
  if (done) {
    const int32_t state = int32_t(Load(base, SaveData(base) + 16));
    if (state == 0) {
      const uint64_t named =
          calls.Call(kListSelection, frame, lr.selection, Load(base, kSaveList)) + 2;
      PlayNamed(calls, frame, lr.play_selection, movie, named);
      calls.Call(kLoopNamed, frame, lr.loop, movie, named);
    } else if (state == 2) {
      PlayNamed(calls, frame, lr.play0, movie, 0);
      calls.Call(kLoopNamed, frame, lr.loop, movie, 0);
    }
  }
  return done ? 1 : 0;
}

namespace {

constexpr Kind kSaveDialogKinds[13] = {
    {62, 0, -1, false}, {83, 0, 1, false},  {69, 0, -1, false}, {70, 0, -1, false},
    {84, 0, 1, false},  {85, 0, -1, false}, {86, 0, 1, false},  {86, 0, 1, false},
    {87, 0, -1, false}, {88, 0, -1, false}, {60, 0, -1, false}, {103, 0, -1, false},
    {104, 0, -1, false}};
constexpr Kind kCategory6DialogKinds[12] = {
    {62, 0, -1, false}, {61, 0, 1, false},  {0, 0, -1, false},    {0, 0, -1, false},
    {71, 0, 1, false},  {72, 0, 1, false},  {65, 0, 1, false},    {66, 240, -1, false},
    {63, 0, -1, false}, {64, 0, -1, false}, {67, 0, -1, false},   {65, 0, 1, false}};

constexpr uint32_t kOptionsMenu = 0x826D8484;  // the dialogs' two-item menu

}  // namespace

uint64_t OptionsShowDialog(uint8_t* base, uint32_t sub_scene, uint32_t step, uint32_t sp,
                           const Calls& calls) {
  const bool save = sub_scene == 13;
  if (save) {
    ITC_NATIVE_RUNNING("sub_821C6638 Options::ShowSaveDialog");
  } else {
    ITC_NATIVE_RUNNING("sub_821C69D0 Options::ShowCategory6Dialog");
  }
  // Page 9's words (+620 the state, +628 the kind, +632, +636 the default
  // answer) and animations, or page 10's (+640 to +656).
  const uint32_t words = save ? 620 : 640;
  const uint32_t open_named = save ? 853 : 869, shown_named = save ? 854 : 870;
  const uint32_t cursor_named = save ? 843 : 859, close_named = save ? 841 : 857;
  // The calls' return addresses: MenuSound::Play, PlayNamed in states 0, 1
  // and 2, ShowPrompts, Run, the two PlayNamed after it and LoopNamed.
  const uint32_t lr_sound = save ? 0x821C6680 : 0x821C6A18;
  const uint32_t lr_play0 = save ? 0x821C68C4 : 0x821C6C38;
  const uint32_t lr_play1 = save ? 0x821C6868 : 0x821C6BDC;
  const uint32_t lr_play2 = save ? 0x821C67FC : 0x821C6B70;
  const uint32_t lr_prompts = save ? 0x821C6938 : 0x821C6CAC;
  const uint32_t lr_run = save ? 0x821C6948 : 0x821C6CBC;
  const uint32_t lr_cursor = save ? 0x821C69B8 : 0x821C6D2C;
  const uint32_t lr_close = save ? 0x821C6988 : 0x821C6CFC;
  const uint32_t lr_loop = save ? 0x821C69C4 : 0x821C6D38;
  auto o = [&]() { return Load(base, kOptions); };
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t movie = FindMovie(base, 7, frame + 96);
  if (Load(base, step) == 0) {
    calls.Call(kMenuSoundPlay, frame, lr_sound, 5);
    const uint32_t kind = Load(base, o() + words + 8);
    const Kind* k = AskKind(
        save ? kSaveDialogKinds : kCategory6DialogKinds, save ? 13 : 12, kind,
        [&](uint32_t id, const uint64_t* v) {
          OptionsSetDialogText(base, sub_scene, id, v, frame, calls);
        },
        [&](uint32_t id, const uint64_t* v) {
          OptionsSetDialogNote(base, sub_scene, id, v, frame, calls);
        });
    if (k != nullptr && k->answer >= 0) {
      Store(base, o() + words + 16, uint32_t(k->answer));
    }
    // The menu's words all 0 (in the original's order), +16 and +20 1 for
    // state 0.
    auto reset_menu = [&](uint32_t first) {
      const uint32_t m = Load(base, kOptionsMenu);
      Store(base, m + 16, first);
      Store(base, m + 12, 0);
      Store(base, m + 20, first);
      for (const uint32_t at : {0u, 4u, 8u, 40u, 48u, 52u, 36u, 64u, 24u, 28u, 32u}) {
        Store(base, m + at, 0);
      }
    };
    const uint64_t none = 0xFFFFFFFFFFFFFFFFull;
    const uint32_t state = Load(base, o() + words);
    if (state <= 2) {
      uint64_t first_two = 0;
      if (state == 0) {
        PlayNamed(calls, frame, lr_play0, movie, open_named);
        reset_menu(1);
        const uint32_t m = Load(base, kOptionsMenu);
        const uint32_t was = Load(base, m + 0);
        const uint32_t answer = Load(base, o() + words + 16);
        Store(base, m + 4, was);
        Store(base, m + 0, answer);
      } else if (state == 1) {
        Store(base, ui_movies::FindNode(base, movie, sub_scene, 0) + 80, 0);
        Store(base,
              ui_movies::Child(base, ui_movies::FindNode(base, movie, sub_scene, 2), 0) + 80, 1);
        PlayNamed(calls, frame, lr_play1, movie, shown_named);
        const uint32_t m = Load(base, kOptionsMenu);
        for (const uint32_t at :
             {12u, 16u, 20u, 0u, 4u, 8u, 40u, 48u, 52u, 36u, 64u, 24u, 28u, 32u}) {
          Store(base, m + at, 0);
        }
      } else {
        Store(base, ui_movies::FindNode(base, movie, sub_scene, 0) + 80, 1);
        Store(base,
              ui_movies::Child(base, ui_movies::FindNode(base, movie, sub_scene, 2), 0) + 80, 0);
        PlayNamed(calls, frame, lr_play2, movie, shown_named);
        Store(base, o() + words + 12, 0);
        first_two = none;
      }
      const uint64_t args[8] = {first_two, first_two, none, none, none, none, 0, 0};
      calls.CallArgs(kShowPrompts, frame, lr_prompts, args);
    }
    Store(base, step, 1);
  }
  const bool done = uint32_t(calls.Call(kRun, frame, lr_run, movie, 0)) == 0;
  if (done) {
    const int32_t state = int32_t(Load(base, o() + words));
    if (state == 0) {
      const uint64_t named = uint64_t(Load(base, Load(base, kOptionsMenu) + 0)) + cursor_named;
      PlayNamed(calls, frame, lr_cursor, movie, named);
      calls.Call(kLoopNamed, frame, lr_loop, movie, named);
    } else if (state == 2) {
      PlayNamed(calls, frame, lr_close, movie, close_named);
      calls.Call(kLoopNamed, frame, lr_loop, movie, close_named);
    }
  }
  return done ? 1 : 0;
}

}  // namespace front_end_screen_helpers
