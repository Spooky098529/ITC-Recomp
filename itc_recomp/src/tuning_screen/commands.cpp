// The tuning screen's movie 2 command runners never seen in play
// (docs/functions/tuning-screen.md): see tuning_screen.h. Called only by the
// update's plain C++ (update.cpp). The UI movie calls (UiMovie::Play,
// Finish, FinishNamed, PlayNamed, LoopNamed, SetText, SetNodeText,
// ScrollText and sub_8211C5F8) through their overrides from each original's
// frame with its return addresses; movie 2's nodes and
// UiMovie::NamedAnimationTime through the UI movies' plain C++; the unset
// varargs of SetText and SetNodeText passed as 0 (the user's call,
// 2026-10-30).

#include "tuning_screen/tuning_screen.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
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

}  // namespace tuning_screen
