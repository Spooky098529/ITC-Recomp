// The tuning screen's command runners never seen in play, movie 2's and the
// vinyl and sticker editors' (movies 13 and 14)
// (docs/functions/tuning-screen.md): see tuning_screen.h. Called only by the
// update's plain C++ (update.cpp). The UI movie calls (UiMovie::Play,
// Finish, FinishNamed, PlayNamed, LoopNamed, SetText, SetNodeText,
// ScrollText and sub_8211C5F8) through their overrides from each original's
// frame with its return addresses; the movies' nodes (with ShowNode's,
// HideNode's and SetNodeColor's stores) and UiMovie::NamedAnimationTime
// through the UI movies' plain C++, PlaceStateNodes by its plain C++; the unset
// varargs of SetText and SetNodeText passed as 0 (the user's call,
// 2026-10-30).

#include "tuning_screen/tuning_screen.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "tuning_pages/tuning_pages.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <bit>

namespace tuning_screen {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
uint32_t Byte(uint8_t* base, uint32_t address) { return guest::LoadU8(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  Store(base, sp - bytes, sp);
  return sp - bytes;
}

namespace steps = camera_modes::steps;

constexpr uint32_t kSetNodeText = 0x8211C7E8;  // UiMovie::SetNodeText(movie, node, id, ...)
constexpr uint32_t kForty = 0x82001FB0;        // 40.0
constexpr uint32_t kEighty = 0x82001E8C;       // 80.0
constexpr uint32_t kNotPlaying = 0x82001BE0;   // -1.0: NamedAnimationTime's answer for none

// The doubleword at frame + 80 (std), then lfd, fcfid and frsp: the float's
// bits for the stfs.
uint32_t AsFloat(uint8_t* base, uint32_t frame, int64_t x) {
  Store(base, frame + 80, uint32_t(uint64_t(x) >> 32));
  Store(base, frame + 84, uint32_t(x));
  return steps::Bits(steps::Single(double(x)));
}

// The same multiplied by the 40.0 at 0x82001FB0 (fmuls).
uint32_t Scaled(uint8_t* base, uint32_t frame, int64_t x) {
  Store(base, frame + 80, uint32_t(uint64_t(x) >> 32));
  Store(base, frame + 84, uint32_t(x));
  return steps::Bits(steps::Fmuls(steps::Single(double(x)), steps::Lfs(base, kForty)));
}

// mulli by 40 then extsw: the product's low word, signed.
int64_t Times40(uint32_t x) { return int32_t(x * 40u); }

// The animation's flags after a +9716 set-up: +9720 = 0 when set, +9716 = 0.
void ClearFlags(uint8_t* base, uint32_t screen) {
  if (Load(base, screen + 9720) != 0) {
    Store(base, screen + 9720, 0);
  }
  Store(base, screen + 9716, 0);
}

// The queue's step with +9724, then +9724 = 0.
void StepWith9724(uint8_t* base, uint32_t screen) {
  if (Load(base, screen + 9724) != 0) {
    Advance(base, screen);
    Store(base, screen + 9724, 0);
  }
}

// The scene's animation `index` restarted: Finish(-1, index), Play(-1,
// index, 0).
void SceneRestart(const GuestCall& call, uint32_t frame, uint32_t movie, uint64_t index,
                  uint32_t finish_lr, uint32_t play_lr) {
  call(kFinish, {movie, kAllScenes, index}, frame, finish_lr);
  call(kPlay, {movie, kAllScenes, index, 0, 0, 0, 0}, frame, play_lr);
}

// FinishNamed(named), PlayNamed(named, chained), LoopNamed(named) with
// `named` read again for each; LoopNamed's r3.
template <typename Named>
uint64_t Restarted(const GuestCall& call, uint32_t frame, uint32_t movie, const Named& named,
                   uint32_t chained, uint32_t finish_lr, uint32_t play_lr, uint32_t loop_lr) {
  FinishNamed(call, frame, finish_lr, movie, named());
  PlayNamed(call, frame, play_lr, movie, named(), chained);
  return LoopNamed(call, frame, loop_lr, movie, named());
}

}  // namespace

uint64_t Command5(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821D4180 TuningScreen::Command5");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t entry = MovieEntry(base, frame, kMovieId);
  const int32_t category = int32_t(Load(base, screen + kCategory));
  const uint32_t movie = Load(base, entry);
  uint64_t r3 = entry;
  auto type2 = [&] { return uint64_t(Byte(base, screen + kType2)); };
  auto pick = [&] { return int64_t(Byte(base, screen + kType2)); };
  if (category == 1 && Byte(base, screen + kItem) == 3) {
    if (Load(base, screen + 9716) != 0) {
      SceneRestart(call, frame, movie, 28, 0x821D41E8, 0x821D4208);
      ClearFlags(base, screen);
    }
    const uint32_t node = ui_movies::FindNode(base, movie, 32, 1);
    Store(base, node + 16, Scaled(base, frame, pick()));
    r3 = Restarted(call, frame, movie, [&] { return type2() + 734; }, 1, 0x821D4264, 0x821D4284,
                   0x821D44F0);
  } else if (category == 4 && Byte(base, screen + kItem) == 4) {
    // The bonnet: the current choice's picture marked when [+36368] is the
    // type byte.
    if (Load(base, screen + 9716) != 0) {
      SceneRestart(call, frame, movie, 32, 0x821D42C8, 0x821D42E8);
      const uint32_t picture = ui_movies::Child(base, ui_movies::FindNode(base, movie, 30, 0), 2);
      const int64_t choice = int32_t(GetCurrentChoice(base, screen));
      Store(base, picture + 16, Scaled(base, frame, choice));
      const uint32_t type = Byte(base, screen + kType);
      const bool same = int32_t(Load(base, screen + 36368)) == int32_t(type);
      Store(base, picture + 80, same ? 1 : 0);
      ClearFlags(base, screen);
    }
    const uint32_t mark = ui_movies::Child(base, ui_movies::FindNode(base, movie, 30, 1), 0);
    Store(base, mark + 16, Scaled(base, frame, pick()));
    Restarted(call, frame, movie, [&] { return type2() + 571; }, 1, 0x821D43AC, 0x821D43CC,
              0x821D43DC);
    r3 = Restarted(call, frame, movie, [&] { return type2() + 632; }, 0, 0x821D43EC, 0x821D440C,
                   0x821D44F0);
  } else if (category == 6 && Byte(base, screen + kItem) == 0) {
    if (Load(base, screen + 9716) != 0) {
      SceneRestart(call, frame, movie, 36, 0x821D4448, 0x821D4468);
      const uint32_t node = ui_movies::FindNode(base, movie, 32, 1);
      Store(base, node + 16, Scaled(base, frame, pick()));
      ClearFlags(base, screen);
    }
    r3 = Restarted(call, frame, movie, [&] { return type2() + 771; }, 1, 0x821D44C0, 0x821D44E0,
                   0x821D44F0);
  }
  StepWith9724(base, screen);
  return r3;
}

uint64_t Command11(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821E9730 TuningScreen::Command11");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = Load(base, MovieEntry(base, frame, kMovieId));
  if (Load(base, screen + 9716) != 0) {
    if (Load(base, screen + 9720) != 0) {
      PlayNamed(call, frame, 0x821E9798, movie, 980, 0);
      FinishNamed(call, frame, 0x821E97A4, movie, 980);
      Advance(base, screen);
      Store(base, screen + 9720, 0);
    } else {
      FinishNamed(call, frame, 0x821E97E0, movie, 980);
      PlayNamed(call, frame, 0x821E97FC, movie, 980, 1);
      FinishNamed(call, frame, 0x821E9808, movie, 1009);
      PlayNamed(call, frame, 0x821E9824, movie, 1009, 0);
    }
    Store(base, screen + 9716, 0);
  }
  const uint32_t node = ui_movies::FindNode(base, movie, 36, 0);
  Store(base, node + 16, AsFloat(base, frame, Times40(Load(base, screen + 30492))));
  Restarted(call, frame, movie, [&] { return uint64_t(Load(base, screen + 30492)) + 887; }, 1,
            0x821E986C, 0x821E988C, 0x821E989C);
  call(kResetTextScroll, {movie, 10, 0}, frame, 0x821E98AC);
  SetText(call, frame, 0x821E98C0, movie, 10, 0, 642, 0);
  const uint64_t r3 = call(kScrollText, {movie, 10, 0, 1}, frame, 0x821E98D4);
  StepWith9724(base, screen);
  return r3;
}

uint64_t Command12(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821E9CA0 TuningScreen::Command12");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = Load(base, MovieEntry(base, frame, kMovieId));
  if (Load(base, screen + 9716) != 0) {
    FinishNamed(call, frame, 0x821E9CE8, movie, 983);
    PlayNamed(call, frame, 0x821E9D04, movie, 983, 1);
    ClearFlags(base, screen);
  }
  const uint32_t node = ui_movies::FindNode(base, movie, 37, 0);
  Store(base, node + 16, AsFloat(base, frame, Times40(Load(base, screen + 30500))));
  const uint64_t r3 =
      Restarted(call, frame, movie, [&] { return uint64_t(Load(base, screen + 30500)) + 926; }, 1,
                0x821E9D5C, 0x821E9D7C, 0x821E9D8C);
  StepWith9724(base, screen);
  return r3;
}

uint64_t Command54(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821EB8E8 TuningScreen::Command54");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t entry = MovieEntry(base, frame, kMovieId);
  const uint32_t movie = Load(base, entry);
  uint64_t r3 = entry;
  if (Load(base, screen + 9716) != 0) {
    if (Load(base, screen + 9720) != 0) {
      PlayNamed(call, frame, 0x821EB950, movie, 1001, 0);
      r3 = FinishNamed(call, frame, 0x821EB95C, movie, 1001);
      Advance(base, screen);
      Store(base, screen + 9720, 0);
    } else {
      FinishNamed(call, frame, 0x821EB998, movie, 1001);
      r3 = PlayNamed(call, frame, 0x821EB9B4, movie, 1001, 0);
    }
    Store(base, screen + 9716, 0);
  }
  StepWith9724(base, screen);
  return r3;
}

uint64_t Command55(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821EBDC8 TuningScreen::Command55");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = Load(base, MovieEntry(base, frame, kMovieId));
  const uint32_t far = steps::Bits(steps::Lfs(base, kEighty));
  if (Load(base, screen + 9716) != 0) {
    FinishNamed(call, frame, 0x821EBE1C, movie, 1002);
    PlayNamed(call, frame, 0x821EBE38, movie, 1002, 0);
    const uint32_t picture = ui_movies::Child(base, ui_movies::FindNode(base, movie, 28, 0), 2);
    Store(base, picture + 80, 1);
    if (int32_t(GetCurrentChoice(base, screen)) == 3) {
      Store(base, picture + 16, far);
    } else {
      Store(base, picture + 16, AsFloat(base, frame, Times40(GetCurrentChoice(base, screen))));
    }
    ClearFlags(base, screen);
  }
  // The pick [+33548]: 3 puts the mark at 80.0 and plays 430 and 491, else
  // 428 + pick and 489 + pick (the word read again for each).
  const uint32_t mark = ui_movies::Child(base, ui_movies::FindNode(base, movie, 28, 1), 0);
  const uint32_t at = screen + 33548;
  const bool three = int32_t(Load(base, at)) == 3;
  Store(base, mark + 16, three ? far : AsFloat(base, frame, Times40(Load(base, at))));
  auto word = [&] { return uint64_t(Load(base, at)); };
  if (int32_t(Load(base, at)) == 3) {
    Restarted(call, frame, movie, [] { return uint64_t(430); }, 1, 0x821EBF14, 0x821EBF30,
              0x821EBF70);
  } else {
    Restarted(call, frame, movie, [&] { return word() + 428; }, 1, 0x821EBF40, 0x821EBF60,
              0x821EBF70);
  }
  uint64_t r3;
  if (int32_t(Load(base, at)) == 3) {
    r3 = Restarted(call, frame, movie, [] { return uint64_t(491); }, 0, 0x821EBF88, 0x821EBFA4,
                   0x821EBFE4);
  } else {
    r3 = Restarted(call, frame, movie, [&] { return word() + 489; }, 0, 0x821EBFB4, 0x821EBFD4,
                   0x821EBFE4);
  }
  StepWith9724(base, screen);
  return r3;
}

uint64_t Command56(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821EC6A0 TuningScreen::Command56");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = Load(base, MovieEntry(base, frame, kMovieId));
  const uint32_t at = screen + 33556;
  if (Load(base, screen + 9716) != 0) {
    FinishNamed(call, frame, 0x821EC6F0, movie, 999);
    PlayNamed(call, frame, 0x821EC70C, movie, 999, 0);
    const uint32_t node = ui_movies::FindNode(base, movie, 32, 1);
    Store(base, node + 16, AsFloat(base, frame, Times40(Load(base, at))));
    ClearFlags(base, screen);
  }
  const uint64_t r3 =
      Restarted(call, frame, movie, [&] { return uint64_t(Load(base, at)) + 771; }, 1, 0x821EC764,
                0x821EC784, 0x821EC794);
  StepWith9724(base, screen);
  return r3;
}

uint64_t Command57(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp,
                   std::optional<double>* f1) {
  ITC_NATIVE_RUNNING("sub_821ECC68 TuningScreen::Command57");
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t movie = Load(base, MovieEntry(base, frame, kMovieId));
  const uint32_t page = screen + 33564;  // the page: 4 the list's, else its rows
  const double none = steps::Lfs(base, kNotPlaying);
  uint64_t r3 = movie;
  // UiMovie::NamedAnimationTime: f1 set, r3 the movie (left).
  auto time = [&](uint32_t index) {
    const double t =
        double(std::bit_cast<float>(ui_movies::NamedAnimationTime(base, movie, index)));
    *f1 = t;
    r3 = movie;
    return t;
  };
  // With neither 1005 nor 144 playing and +9720 0, the page's animation
  // (79 for the page 4, else 74 + the page) restarted and looped.
  if (time(1005) == none && time(144) == none && Load(base, screen + 9720) == 0) {
    if (int32_t(Load(base, page)) == 4) {
      r3 = Restarted(call, frame, movie, [] { return uint64_t(79); }, 1, 0x821ECCF8, 0x821ECD14,
                     0x821ECD54);
    } else {
      r3 = Restarted(call, frame, movie, [&] { return uint64_t(Load(base, page)) + 74; }, 1,
                     0x821ECD24, 0x821ECD44, 0x821ECD54);
    }
  }
  if (Load(base, screen + 9716) != 0) {
    if (Load(base, screen + 9720) != 0) {
      FinishNamed(call, frame, 0x821ECD80, movie, 144);
      PlayNamed(call, frame, 0x821ECD9C, movie, 144, 0);
      FinishNamed(call, frame, 0x821ECDA8, movie, 1011);
      r3 = PlayNamed(call, frame, 0x821ECDC4, movie, 1011, 0);
      Advance(base, screen);
      if (int32_t(Load(base, page)) == 4) {
        Store(base, screen + 9720, 0);
      }
    } else {
      FinishNamed(call, frame, 0x821ECE10, movie, 1005);
      r3 = PlayNamed(call, frame, 0x821ECE2C, movie, 1005, 0);
    }
    Store(base, screen + 9716, 0);
  }
  // The meter's point [+33572] 0 to 4: one animation played, the next
  // restarted and looped.
  struct Pair {
    uint32_t first, second;
    uint32_t lr[4];
  };
  static constexpr Pair kPairs[5] = {
      {118, 116, {0x821ECFC8, 0x821ECFE4, 0x821ECFF0, 0x821ED00C}},
      {123, 121, {0x821ECF70, 0x821ECF8C, 0x821ECF98, 0x821ECFB4}},
      {128, 126, {0x821ECF18, 0x821ECF34, 0x821ECF40, 0x821ECF5C}},
      {133, 131, {0x821ECEC0, 0x821ECEDC, 0x821ECEE8, 0x821ECF04}},
      {138, 136, {0x821ECE68, 0x821ECE84, 0x821ECE90, 0x821ECEAC}},
  };
  const uint32_t point = Load(base, screen + 33572);
  if (point < 5) {
    const Pair& p = kPairs[point];
    FinishNamed(call, frame, p.lr[0], movie, p.first);
    PlayNamed(call, frame, p.lr[1], movie, p.first, 0);
    FinishNamed(call, frame, p.lr[2], movie, p.second);
    PlayNamed(call, frame, p.lr[3], movie, p.second, 1);
    r3 = LoopNamed(call, frame, 0x821ED018, movie, p.second);
  }
  // The rows (sub-scene 2's children 9 to 13) outside the page 4: child 2
  // named string 379 with the row's number, child 5 shown by
  // GetChoiceFlag, child 6's +108 = 5 × the page + the row.
  if (int32_t(Load(base, page)) != 4) {
    for (uint32_t i = 0; i < 5; i++) {
      const uint32_t row = ui_movies::FindNode(base, movie, 2, i + 9);
      const uint32_t text = ui_movies::Child(base, row, 2);
      call(kSetNodeText, {movie, text, 379, i + 1, 0, 0, 0, 0}, frame, 0x821ED060);
      const uint32_t mark = ui_movies::Child(base, row, 5);
      const bool flag = int32_t(GetChoiceFlag(base, screen, i)) != 0;
      Store(base, mark + 80, flag ? 1 : 0);
      const uint32_t picture = ui_movies::Child(base, row, 6);
      Store(base, picture + 108, Load(base, page) * 5 + i);
      r3 = picture;
    }
  }
  StepWith9724(base, screen);
  return r3;
}

uint64_t Command58(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821EDB70 TuningScreen::Command58");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = Load(base, MovieEntry(base, frame, kMovieId));
  if (Load(base, screen + 9716) != 0) {
    FinishNamed(call, frame, 0x821EDBB8, movie, 143);
    PlayNamed(call, frame, 0x821EDBD4, movie, 143, 1);
    FinishNamed(call, frame, 0x821EDBE0, movie, 1012);
    PlayNamed(call, frame, 0x821EDBFC, movie, 1012, 0);
    ClearFlags(base, screen);
  }
  const uint32_t at = screen + 33576;
  const uint64_t r3 =
      Restarted(call, frame, movie, [&] { return uint64_t(Load(base, at)) + 111; }, 1, 0x821EDC28,
                0x821EDC48, 0x821EDC58);
  StepWith9724(base, screen);
  return r3;
}

// --- The editors' runners (movies 13 and 14) ---

namespace {

// The editors' +9716 set-up: with +9720 the queue's step and +9720 = 0,
// else `named` restarted chained (and `more`); then +9716 = 0.
template <typename More>
void EditorSetUp(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t frame,
                 uint32_t movie, uint64_t named, uint32_t finish_lr, uint32_t play_lr,
                 const More& more) {
  if (Load(base, screen + 9716) == 0) {
    return;
  }
  if (Load(base, screen + 9720) != 0) {
    Advance(base, screen);
    Store(base, screen + 9720, 0);
  } else {
    FinishNamed(call, frame, finish_lr, movie, named);
    PlayNamed(call, frame, play_lr, movie, named, 1);
    more();
  }
  Store(base, screen + 9716, 0);
}

// Help text (2, 0) of movie 13 given string `id` and scrolled
// (sub_8211C5F8, SetText, ScrollText); ScrollText's r3.
uint64_t HelpText(const GuestCall& call, uint32_t frame, uint32_t movie, uint32_t id,
                  const uint32_t (&lr)[3]) {
  call(kResetTextScroll, {movie, 2, 0}, frame, lr[0]);
  SetText(call, frame, lr[1], movie, 2, 0, id, 0);
  return call(kScrollText, {movie, 2, 0, 1}, frame, lr[2]);
}

// Command26 and Command27: as Command25 with their animation and string.
uint64_t PlaceCommand(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp,
                      uint32_t named, uint32_t id, const uint32_t (&lr)[5]) {
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = Load(base, MovieEntry(base, frame, 13));
  EditorSetUp(base, call, screen, frame, movie, named, lr[0], lr[1], [] {});
  PlaceStateNodes(base, screen, frame);
  return HelpText(call, frame, movie, id, {lr[2], lr[3], lr[4]});
}

// The word at +33480 + 4 × `i` (32-bit addresses, as rlwinm and lwzx).
uint32_t SlotAt(uint32_t screen, uint32_t i) { return screen + (i + 8370u) * 4u; }

}  // namespace

uint64_t Command17(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82200100 TuningScreen::Command17");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = Load(base, MovieEntry(base, frame, 13));
  EditorSetUp(base, call, screen, frame, movie, 221, 0x82200190, 0x822001AC, [] {});
  const uint32_t node = ui_movies::FindNode(base, movie, 10, 0);
  // lwa: the word sign-extended.
  Store(base, node + 16, Scaled(base, frame, int32_t(Load(base, screen + 30740))));
  return Restarted(call, frame, movie, [&] { return uint64_t(Load(base, screen + 30740)) + 49; },
                   1, 0x822001F8, 0x82200218, 0x82200228);
}

uint64_t Command19(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82201C58 TuningScreen::Command19");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = Load(base, MovieEntry(base, frame, 13));
  EditorSetUp(base, call, screen, frame, movie, 236, 0x82201CE8, 0x82201D04, [&] {
    // By the set +30724: 4 × [+30732] + 201 (0) or + 175 (1), the word read
    // again for each call.
    const int32_t set = int32_t(Load(base, screen + 30724));
    if (set != 0 && set != 1) {
      return;
    }
    const uint64_t add = set == 0 ? 201 : 175;
    auto named = [&] { return uint64_t(uint32_t(Load(base, screen + 30732) << 2)) + add; };
    FinishNamed(call, frame, set == 0 ? 0x82201D24 : 0x82201D50, movie, named());
    PlayNamed(call, frame, 0x82201D74, movie, named(), 1);
  });
  return HelpText(call, frame, movie, 810, {0x82201D88, 0x82201D9C, 0x82201DB0});
}

uint64_t Command20(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp, uint64_t a,
                   uint64_t b) {
  ITC_NATIVE_RUNNING("sub_82201FA0 TuningScreen::Command20");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t entry = MovieEntry(base, frame, 13);
  const int32_t set = int32_t(Load(base, screen + 30724));
  const uint32_t movie = Load(base, entry);
  uint64_t first, second;
  uint32_t lr[2];
  if (set == 0) {
    first = uint32_t((a + 51) << 2);  // addi, then rlwinm
    second = uint64_t(uint32_t(b << 2)) + 203;
    lr[0] = 0x82201FF4;
    lr[1] = 0x82202010;
  } else if (set == 1) {
    first = uint64_t(uint32_t(a << 2)) + 178;
    second = uint64_t(uint32_t(b << 2)) + 177;
    lr[0] = 0x82202038;
    lr[1] = 0x82202054;
  } else {
    return entry;
  }
  FinishNamed(call, frame, lr[0], movie, first);
  PlayNamed(call, frame, lr[1], movie, first, 1);
  FinishNamed(call, frame, 0x82202068, movie, second);
  return PlayNamed(call, frame, 0x82202084, movie, second, 0);
}

uint64_t Command21(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t sp,
                   bool native_pages) {
  ITC_NATIVE_RUNNING("sub_822087C8 TuningScreen::Command21");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t s = uint32_t(r3);
  const auto word = [&](uint32_t offset) { return Load(base, s + offset); };
  // The work list's words +31228 + 4i (the original's (i + 7807) << 2).
  const auto list = [&](uint32_t i) { return s + uint32_t((i + 7807) << 2); };
  const uint32_t movie = Load(base, MovieEntry(base, frame, 13));
  int32_t n = int32_t(word(30732) - word(30756));
  // One move of the layer: the two records and textures swapped, the decal
  // images' entries a and a - 1 exchanged with movie 13's slots, Command20.
  const auto move = [&](uint64_t first, uint64_t second, const uint32_t lr[2], uint32_t up) {
    if (native_pages) {
      tuning_pages::SwapLayerRecords(base, call, r3, first, second, frame);
      tuning_pages::SwapOrderTextures(base, r3, frame);
    } else {
      call(0x821FD518, {r3, first, second}, frame, lr[0]);
      call(0x82207F38, {r3}, frame, lr[1]);
    }
    const int32_t set = int32_t(word(30724));
    if (set == 0 || set == 1) {
      const uint32_t c = word(30756);
      const uint32_t images = s + 10048;
      const uint32_t shift = (set == 0 ? 5 : 26) + up;
      const uint32_t e1 = images + 36 * (c + shift + 1);
      const uint32_t e2 = images + 36 * (c + shift);
      const uint32_t h1 = Load(base, e1 + 28);
      const uint32_t h2 = Load(base, e2 + 28);
      const uint32_t d1 = Load(base, e1 + 32);
      Store(base, e1 + 28, h2);
      Store(base, e1 + 32, Load(base, e2 + 32));
      Store(base, e2 + 28, h1);
      Store(base, e2 + 32, d1);
      const uint32_t slot = set == 0 ? 764 : 776;
      const uint32_t from = set == 0 ? 10292 : 11048;
      for (uint32_t k = 0; k < 3; k++) {
        const uint32_t entry = MovieEntry(base, frame, 13);
        const uint32_t value = word(from + 36 * k);
        Store(base, Load(base, Load(base, Load(base, entry) + 28) + slot + 4 * k), value);
      }
    }
    Command20(base, call, s, frame, word(9644), word(9648));
  };
  if (n > 0) {
    static constexpr uint32_t kLr[2] = {0x82208878, 0x82208880};
    do {
      const uint32_t c = word(30756);
      n--;
      const uint32_t moved = Load(base, list(c));
      Store(base, s + 30756, c + 1);
      const uint32_t other = Load(base, list(c + 1));
      Store(base, list(c + 1), moved);
      Store(base, list(word(30756) - 1), other);
      const uint32_t a = word(30756);
      Store(base, s + 9644, Load(base, list(a)));
      Store(base, s + 9648, Load(base, list(a - 1)));
      move(uint64_t(a), uint64_t(a) - 1, kLr, 0);
    } while (n != 0);
  } else if (n < 0) {
    static constexpr uint32_t kLr[2] = {0x82208A8C, 0x82208A94};
    uint32_t count = uint32_t(-int64_t(n));
    do {
      const uint32_t c = word(30756);
      const uint32_t moved = Load(base, list(c));
      Store(base, s + 30756, c - 1);
      const uint32_t other = Load(base, list(c - 1));
      Store(base, list(c - 1), moved);
      Store(base, list(word(30756) + 1), other);
      const uint32_t a = word(30756);
      Store(base, s + 9644, Load(base, list(a + 1)));
      Store(base, s + 9648, Load(base, list(a)));
      move(uint64_t(a) + 1, uint64_t(a), kLr, 1);
      count--;
    } while (count != 0);
  }
  // The layer's row: 4 x layer + 202 (set 0) or 4 x (layer + 44) (set 1)
  // finished and played chained.
  const int32_t set = int32_t(word(30724));
  if (set == 0) {
    FinishNamed(call, frame, 0x82208C48, movie, uint64_t(uint32_t(word(30732) << 2)) + 202);
    PlayNamed(call, frame, 0x82208C98, movie, uint64_t(uint32_t(word(30732) << 2)) + 202, 1);
  } else if (set == 1) {
    FinishNamed(call, frame, 0x82208C74, movie, uint32_t((word(30732) + 44) << 2));
    PlayNamed(call, frame, 0x82208C98, movie, uint32_t((word(30732) + 44) << 2), 1);
  }
  const uint64_t answer = LayOutItems(base, s, frame);
  Advance(base, s);
  return answer;
}

uint64_t Command26(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82204980 TuningScreen::Command26");
  return PlaceCommand(base, call, screen, sp, 229, 821,
                      {0x82204A10, 0x82204A2C, 0x82204A48, 0x82204A5C, 0x82204A70});
}

uint64_t Command27(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82204A78 TuningScreen::Command27");
  return PlaceCommand(base, call, screen, sp, 230, 822,
                      {0x82204B08, 0x82204B24, 0x82204B40, 0x82204B54, 0x82204B68});
}

uint64_t Command32Done(uint8_t* base, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F7498 TuningScreen::Command32Done");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t entry = MovieEntry(base, frame, 14);
  const uint32_t movie = Load(base, entry);
  if (Load(base, screen + 33508) == 0) {
    return entry;
  }
  // UiMovie::SetNodeColor (sub_8211A5B8): the node's color; r3 the node.
  uint32_t node = 0;
  for (const uint32_t child : {11u, 12u}) {
    node = ui_movies::FindNode(base, movie, 3, child);
    ui_movies::SetColor(base, node, 1);
  }
  return node;
}

uint64_t Command42(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F7618 TuningScreen::Command42");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = Load(base, MovieEntry(base, frame, 14));
  for (uint32_t named = 182; named < 202; named += 4) {
    FinishNamed(call, frame, 0x821F7654, movie, named - 1);
    FinishNamed(call, frame, 0x821F7660, movie, named);
  }
  const uint32_t position = screen + 30756;
  auto index = [&] { return Load(base, position); };
  auto times4 = [](uint32_t x) { return uint64_t(uint32_t(x << 2)); };
  const uint32_t at = index();
  const uint32_t target = Load(base, SlotAt(screen, at));
  if (int32_t(at) != int32_t(target)) {
    // Toward the target: the word's animation from one end, the next
    // word's from the other, then that word slid one place back.
    const bool up = int32_t(at) < int32_t(target);
    PlayNamed(call, frame, up ? 0x821F76A8 : 0x821F7718, movie, times4(target) + (up ? 182 : 181),
              0);
    const uint32_t next = index() + (up ? 1 : 0xFFFFFFFFu);
    Store(base, position, next);
    PlayNamed(call, frame, up ? 0x821F76E0 : 0x821F7750, movie,
              times4(Load(base, SlotAt(screen, next))) + (up ? 181 : 182), 0);
    const uint32_t now = index();
    Store(base, SlotAt(screen, now + (up ? 0xFFFFFFFFu : 1u)), Load(base, SlotAt(screen, now)));
  }
  Store(base, SlotAt(screen, index()), target);
  // UiMovie::HideNode (sub_8211AC48): the node's +80 = 0; r3 the node.
  const uint32_t node = ui_movies::FindNode(base, movie, 30, index() * 5 + 12);
  Store(base, node + 80, 0);
  const uint32_t now = index();
  if (int32_t(now) != int32_t(Load(base, SlotAt(screen, now)))) {
    Advance(base, screen);
  }
  return node;
}

uint64_t Command52(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t dispatcher,
                   uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F77F0 TuningScreen::Command52");
  const uint32_t screen = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = Load(base, MovieEntry(base, frame, 14));
  const int32_t state = int32_t(Load(base, screen + kState));
  Store(base, screen + 9728, 0);
  uint32_t title = 0;
  switch (state) {
    case 50: title = 18; break;
    case 51: title = 19; break;
    case 52: title = 16; break;
    default: break;
  }
  if (title != 0) {
    const uint32_t entry = MovieEntry(base, frame, 16);
    DialogTexts(base, call, frame, entry, title, {0x821F78AC, 0x821F78C0, 0x821F78D4});
    PostDialogMessage(base, dispatcher, 34);
  }
  // UiMovie::ShowNode (sub_8211AC00): the node's +80 = 1; r3 the node.
  const uint32_t node = ui_movies::FindNode(base, movie, 1, 1);
  Store(base, node + 80, 1);
  return node;
}

void Command53(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821F7948 TuningScreen::Command53");
  Advance(base, screen);
}

namespace {

constexpr uint32_t kOne = 0x82000A30;      // 1.0
constexpr uint32_t kZero = 0x82000A34;     // 0.0
constexpr uint32_t kDim = 0x82002A14;      // the unpicked pair's alpha
constexpr uint32_t kHue = 0x8200B5D4;      // the paint sliders' scales
constexpr uint32_t kByte = 0x8200B5D0;
constexpr uint32_t kBarLong = 0x8200B5CC;  // the light kits' bars
constexpr uint32_t kBarLength = 0x820021FC;

// The screen's word at `field` made a float through frame + 80 (AsFloat),
// times the float at `scale`, and for `length` times that too.
uint32_t Slider(uint8_t* base, uint32_t frame, uint32_t screen, uint32_t field, uint32_t scale,
                uint32_t length = 0) {
  const int64_t x = int32_t(Load(base, screen + field));
  AsFloat(base, frame, x);
  double value = steps::Fmuls(steps::Single(double(x)), steps::Lfs(base, scale));
  if (length != 0) {
    value = steps::Fmuls(value, steps::Lfs(base, length));
  }
  return steps::Bits(value);
}

// Node (sub_scene, child)'s children `first` and `first` + 1's +12 (each
// looked up after the store before it) from the fields `a` and `b`.
void SliderPair(uint8_t* base, uint32_t movie, uint32_t frame, uint32_t screen,
                uint32_t sub_scene, uint32_t child, uint32_t first, uint32_t a, uint32_t b,
                uint32_t scale, uint32_t length = 0) {
  const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, child);
  const uint32_t one = ui_movies::Child(base, node, first);
  Store(base, one + 12, Slider(base, frame, screen, a, scale, length));
  const uint32_t two = ui_movies::Child(base, node, first + 1);
  Store(base, two + 12, Slider(base, frame, screen, b, scale, length));
}

// The named animation from `named` (read again for each call) restarted
// and looped, `play_lr` and `finish_lr` as the original's.
uint64_t Looped(const GuestCall& call, uint32_t frame, uint32_t movie, uint64_t named,
                uint32_t finish_lr, uint32_t play_lr, uint32_t loop_lr) {
  return Restarted(call, frame, movie, [&] { return named; }, 1, finish_lr, play_lr, loop_lr);
}

}  // namespace

uint64_t Command10(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F35C0 TuningScreen::Command10");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = Load(base, MovieEntry(base, frame, kMovieId));
  if (Load(base, screen + 9716) != 0) {
    FinishNamed(call, frame, 0x821F3610, movie, 979);
    PlayNamed(call, frame, 0x821F362C, movie, 979, 1);
    FinishNamed(call, frame, 0x821F3638, movie, 1009);
    PlayNamed(call, frame, 0x821F3654, movie, 1009, 0);
    ClearFlags(base, screen);
  }
  // The pair's alphas (node (21, 5)'s children 4 and 5) by +30456.
  {
    const uint32_t node = ui_movies::FindNode(base, movie, 21, 5);
    const bool second = Load(base, screen + 30456) != 0;
    Store(base, ui_movies::Child(base, node, 4) + 68, Load(base, second ? kDim : kOne));
    Store(base, ui_movies::Child(base, node, 5) + 68, Load(base, second ? kOne : kDim));
  }
  // The three sliders (nodes (21, 2 to 4)'s children 10 and 11).
  SliderPair(base, movie, frame, screen, 21, 2, 10, 30444, 30476, kHue);
  SliderPair(base, movie, frame, screen, 21, 3, 10, 30448, 30480, kByte);
  SliderPair(base, movie, frame, screen, 21, 4, 10, 30452, 30484, kByte);
  const int32_t kind = int32_t(Load(base, screen + 30440));
  if (kind == 0) {
    const uint32_t row = ui_movies::Child(base, ui_movies::FindNode(base, movie, 21, 1), 1);
    Store(base, row + 68, Load(base, kZero));
    Looped(call, frame, movie, 212, 0x821F39A8, 0x821F39C4, 0x821F39D0);
  } else if (kind == 4) {
    Store(base, ui_movies::Child(base, ui_movies::FindNode(base, movie, 21, 1), 1) + 68,
          Load(base, kOne));
    for (uint32_t child = 2; child <= 4; child++) {
      const uint32_t node = ui_movies::FindNode(base, movie, 21, child);
      Store(base, ui_movies::Child(base, ui_movies::Child(base, node, 5), 1) + 68,
            Load(base, kZero));
    }
    {
      const uint32_t node = ui_movies::FindNode(base, movie, 21, 5);
      Store(base, ui_movies::Child(base, ui_movies::Child(base, node, 3), 0) + 68,
            Load(base, kZero));
    }
    if (Load(base, screen + 30460) == 0) {
      Looped(call, frame, movie, 178, 0x821F3924, 0x821F3940, 0x821F39D0);
    } else {
      Looped(call, frame, movie, 179, 0x821F3950, 0x821F396C, 0x821F39D0);
    }
  }
  call(kResetTextScroll, {movie, 10, 0}, frame, 0x821F39E0);
  SetText(call, frame, 0x821F39F4, movie, 10, 0, 794, 0);
  call(kScrollText, {movie, 10, 0, 1}, frame, 0x821F3A08);
  const uint64_t r3 = tuning_pages::SetPaintGradients(base, screen, frame);
  StepWith9724(base, screen);
  return r3;
}

uint64_t Command13(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821EA460 TuningScreen::Command13");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = Load(base, MovieEntry(base, frame, kMovieId));
  if (Load(base, screen + 9716) != 0) {
    FinishNamed(call, frame, 0x821EA4B0, movie, 985);
    PlayNamed(call, frame, 0x821EA4CC, movie, 985, 1);
    FinishNamed(call, frame, 0x821EA4D8, movie, 228);
    PlayNamed(call, frame, 0x821EA4F4, movie, 228, 1);
    FinishNamed(call, frame, 0x821EA500, movie, 238);
    PlayNamed(call, frame, 0x821EA51C, movie, 238, 1);
    FinishNamed(call, frame, 0x821EA528, movie, 248);
    PlayNamed(call, frame, 0x821EA544, movie, 248, 1);
    Store(base, ui_movies::Child(base, ui_movies::FindNode(base, movie, 23, 1), 1) + 68,
          Load(base, kZero));
    ClearFlags(base, screen);
  }
  // The three bars' sliders (nodes (23, 2 to 4)'s children 6 and 7).
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t field = 30512 + 4 * k;
    SliderPair(base, movie, frame, screen, 23, 2 + k, 6, field, field, kBarLong, kBarLength);
  }
  const auto word = [&](uint32_t offset) { return uint64_t(Load(base, screen + offset)); };
  switch (Load(base, screen + 30508)) {
    case 0: Looped(call, frame, movie, 304, 0x821EA814, 0x821EA830, 0x821EA83C); break;
    case 1: Looped(call, frame, movie, 305, 0x821EA7E4, 0x821EA800, 0x821EA83C); break;
    case 2: Looped(call, frame, movie, 306, 0x821EA7B4, 0x821EA7D0, 0x821EA83C); break;
    case 3:
      Looped(call, frame, movie, 307, 0x821EA744, 0x821EA760, 0x821EA76C);
      Restarted(call, frame, movie, [&] { return word(30524) + 294; }, 1, 0x821EA77C, 0x821EA79C,
                0x821EA83C);
      break;
    case 4:
      Restarted(call, frame, movie, [&] { return word(30528) + 219; }, 1, 0x821EA70C, 0x821EA72C,
                0x821EA83C);
      break;
    default: break;
  }
  const uint64_t r3 = tuning_pages::SetLightKind0Bars(base, screen, frame);
  StepWith9724(base, screen);
  return r3;
}

uint64_t Command14(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821EAFA0 TuningScreen::Command14");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = Load(base, MovieEntry(base, frame, kMovieId));
  if (Load(base, screen + 9716) != 0) {
    FinishNamed(call, frame, 0x821EAFF0, movie, 987);
    PlayNamed(call, frame, 0x821EB00C, movie, 987, 1);
    FinishNamed(call, frame, 0x821EB018, movie, 320);
    PlayNamed(call, frame, 0x821EB034, movie, 320, 1);
    Store(base, ui_movies::Child(base, ui_movies::FindNode(base, movie, 24, 1), 1) + 68,
          Load(base, kZero));
    ClearFlags(base, screen);
  }
  // The bar's slider (node (24, 3)'s children 6 and 7), the row's number.
  SliderPair(base, movie, frame, screen, 24, 3, 6, 30552, 30552, kBarLong, kBarLength);
  const auto word = [&](uint32_t offset) { return uint64_t(Load(base, screen + offset)); };
  call(kSetNodeText, {movie, ui_movies::FindNode(base, movie, 24, 14), 450, word(30548) + 1, 0, 0,
                      0, 0},
       frame, 0x821EB114);
  switch (Load(base, screen + 30544)) {
    case 0: Looped(call, frame, movie, 384, 0x821EB218, 0x821EB234, 0x821EB240); break;
    case 1: Looped(call, frame, movie, 385, 0x821EB1E8, 0x821EB204, 0x821EB240); break;
    case 2:
      Looped(call, frame, movie, 386, 0x821EB178, 0x821EB194, 0x821EB1A0);
      Restarted(call, frame, movie, [&] { return word(30556) + 366; }, 1, 0x821EB1B0, 0x821EB1D0,
                0x821EB240);
      break;
    case 3:
      Restarted(call, frame, movie, [&] { return word(30560) + 309; }, 1, 0x821EB140, 0x821EB160,
                0x821EB240);
      break;
    default: break;
  }
  const uint64_t r3 = tuning_pages::SetLightKind1Bar(base, screen, frame);
  StepWith9724(base, screen);
  return r3;
}

}  // namespace tuning_screen
