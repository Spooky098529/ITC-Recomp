// The build screen's transitions (docs/functions/build-screen.md): see
// build_screen.h. The UI movie animation calls (UiMovie::PlayNamed,
// LoopNamed, FinishNamed, Run) through their overrides, from each
// original's frame with its return addresses.

#include "build_screen/build_screen.h"

#include "base/log.h"
#include "guest.h"
#include "mode_tick/mode_tick.h"
#include "race_screens/race_screens.h"

namespace build_screen {

namespace {

constexpr uint32_t kPlayNamed = 0x8211DAC8;
constexpr uint32_t kLoopNamed = 0x8211DDE0;
constexpr uint32_t kFinishNamed = 0x8211DF20;
constexpr uint32_t kRun = 0x8211E1B8;
constexpr uint32_t kFadeOutCounts = 0x826A50E4, kFadeInCounts = 0x826A50DC;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Screen(uint8_t* base) { return Load(base, kScreen); }

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  Store(base, sp - bytes, sp);
  return sp - bytes;
}

// UiMovie::PlayNamed(movie, named, chained, 0, 0, 0) and LoopNamed(movie,
// named) one after the other, with their return addresses.
struct Movie6 {
  const Calls& calls;
  uint32_t frame;
  uint32_t movie;
  void Play(uint32_t lr, uint64_t named, uint64_t chained = 0) const {
    const uint64_t args[8] = {movie, named, chained, 0, 0, 0, 0, 0};
    calls.CallArgs(kPlayNamed, frame, lr, args);
  }
  void Loop(uint32_t lr, uint64_t named) const { calls.Call(kLoopNamed, frame, lr, movie, named); }
  void Finish(uint32_t lr, uint64_t named) const {
    calls.Call(kFinishNamed, frame, lr, movie, named);
  }
  // PlayNamed then LoopNamed of `named`: `lr` PlayNamed's return address,
  // LoopNamed's 12 bytes on.
  void PlayLoop(uint32_t lr, uint64_t named) const {
    Play(lr, named);
    Loop(lr + 12, named);
  }
  uint64_t Run(uint32_t lr) const { return calls.Call(kRun, frame, lr, movie, 0); }
};

// A transition: its work on the first call (the step word 0, then 1), the
// movie run each call.
template <typename Work>
uint64_t Transition(uint8_t* base, uint32_t step, uint32_t sp, uint32_t bytes, uint32_t run_lr,
                    const Calls& calls, Work&& work) {
  const uint32_t frame = Frame(base, sp, bytes);
  const Movie6 m{calls, frame, Movie(base, frame)};
  if (Load(base, step) == 0) {
    work(m);
    Store(base, step, 1);
  }
  return m.Run(run_lr);
}

uint32_t Selection(uint8_t* base) { return race_screens::ListSelection(base, Load(base, kList)); }

// A back transition's end, once the movie's animations are done: the state
// left (+0) kept at +8 unless it is `level`, +4 `level`, +0 and +16 -1, +12
// `highlight` (that level's highlight transition) with its word +24 0.
void Back(uint8_t* base, uint32_t level, uint32_t highlight) {
  const uint32_t machine = Load(base, kMachine);
  const uint32_t left = Load(base, machine + 0);
  if (left != level) {
    Store(base, machine + 8, left);
  }
  Store(base, machine + 16, 0xFFFFFFFFu);
  Store(base, machine + 4, level);
  Store(base, machine + 12, highlight);
  Store(base, machine + 0, 0xFFFFFFFFu);
  Store(base, Load(base, kMachine) + 24, 0);
}

// HighlightGroups and HighlightGroups2: the groups' animations looped (the
// first also 84), the row's, the category's tab played.
void HighlightGroups(uint8_t* base, const Movie6& m, const uint32_t* lrs, int loops,
                     uint32_t row_lr, uint32_t finish_lr, uint32_t tab_lr) {
  constexpr uint32_t kNamed[5] = {44, 42, 43, 80, 84};
  const uint32_t s = Selection(base);
  for (int i = 0; i < loops; i++) {
    m.PlayLoop(lrs[i], kNamed[i]);
  }
  m.PlayLoop(row_lr, uint64_t(s) + 29);
  for (uint32_t k = 0; k < 4; k++) {
    m.Finish(finish_lr, 76 + k);
  }
  m.Play(tab_lr, uint64_t(Load(base, Screen(base) + kCategory)) + 76, 1);
}

}  // namespace

uint64_t HighlightGroupsA(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219C528 BuildScreen::HighlightGroups");
  static constexpr uint32_t kLrs[5] = {0x8219C58C, 0x8219C5B4, 0x8219C5DC, 0x8219C604,
                                       0x8219C62C};
  Transition(base, step, sp, 128, 0x8219C6C4, calls, [&](const Movie6& m) {
    HighlightGroups(base, m, kLrs, 5, 0x8219C658, 0x8219C674, 0x8219C6B0);
  });
  return 1;
}

uint64_t HighlightGroupsB(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219C6D0 BuildScreen::HighlightGroups2");
  static constexpr uint32_t kLrs[4] = {0x8219C734, 0x8219C75C, 0x8219C784, 0x8219C7AC};
  Transition(base, step, sp, 128, 0x8219C844, calls, [&](const Movie6& m) {
    HighlightGroups(base, m, kLrs, 4, 0x8219C7D8, 0x8219C7F4, 0x8219C830);
  });
  return 1;
}

uint64_t FadeOut(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219C850 BuildScreen::FadeOut");
  Transition(base, step, sp, 112, 0x8219C8C0, calls, [&](const Movie6&) {
    mode_tick::ScreenFadesFadeOut(base, Load(base, 0x82001BF0), 0, 0, 0, 0);
  });
  return (Load(base, kFadeOutCounts) | Load(base, kFadeInCounts)) == 0 ? 1 : 0;
}

uint64_t HighlightBuilds(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219C8F8 BuildScreen::HighlightBuilds");
  Transition(base, step, sp, 128, 0x8219C9F8, calls, [&](const Movie6& m) {
    const uint32_t s = Selection(base);
    m.PlayLoop(0x8219C95C, 71);
    m.PlayLoop(0x8219C984, 70);
    m.PlayLoop(0x8219C9AC, 69);
    m.PlayLoop(0x8219C9D8, uint64_t(s) + 61);
  });
  return 1;
}

uint64_t HighlightStand(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219CA08 BuildScreen::HighlightStand");
  Transition(base, step, sp, 112, 0x8219CAFC, calls, [&](const Movie6& m) {
    m.PlayLoop(0x8219CA64, 96);
    m.PlayLoop(0x8219CA8C, 97);
    m.PlayLoop(0x8219CAB4, 108);
    m.PlayLoop(0x8219CADC, 93);
  });
  return 1;
}

uint64_t HighlightStand2(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219CB18 BuildScreen::HighlightStand2");
  Transition(base, step, sp, 112, 0x8219CBBC, calls, [&](const Movie6& m) {
    m.PlayLoop(0x8219CB74, 108);
    m.PlayLoop(0x8219CB9C, 93);
  });
  return 1;
}

uint64_t HighlightDetails(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219CBD8 BuildScreen::HighlightDetails");
  Transition(base, step, sp, 112, 0x8219CC54, calls,
             [&](const Movie6& m) { m.PlayLoop(0x8219CC34, 74); });
  return 1;
}

uint64_t Open(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A08D8 BuildScreen::Open");
  const uint64_t done = Transition(base, step, sp, 128, 0x821A0A28, calls, [&](const Movie6& m) {
    m.Play(0x821A092C, 101, 1);
    m.PlayLoop(0x821A0948, 100);
    m.PlayLoop(0x821A0970, 1);
    m.PlayLoop(0x821A0998, 83);
    if (Load(base, Screen(base) + kOpened) == 0) {
      m.Play(0x821A09DC, 81, 1);
    }
    Store(base, Screen(base) + kCategory, 0);
    Store(base, Screen(base) + kGroupSelection, 0);
    Store(base, Screen(base) + kGroupScroll, 0);
    SetUpGroups(base, m.frame, calls);
  });
  return uint32_t(done) == 0 ? 1 : 0;
}

uint64_t BackToGroups(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A0A38 BuildScreen::BackToGroups");
  const uint64_t done = Transition(base, step, sp, 128, 0x821A0AE4, calls, [&](const Movie6& m) {
    const uint32_t s = Selection(base);
    m.Finish(0x821A0A8C, 71);
    m.Finish(0x821A0A98, 70);
    m.Finish(0x821A0AA4, 69);
    m.Finish(0x821A0AB0, uint64_t(s) + 61);
    m.Play(0x821A0ACC, 103);
    SetUpGroups(base, m.frame, calls);
  });
  if (uint32_t(done) == 0) {
    Back(base, 0, 2);
  }
  return 0;
}

uint64_t ChangeCategory(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219FC60 BuildScreen::ChangeCategory");
  Transition(base, step, sp, 160, 0x8219FE00, calls, [&](const Movie6& m) {
    const uint32_t delta = Load(base, Load(base, kList) + 28);
    do {
      const uint32_t at = Screen(base) + kCategory;
      Store(base, at, Load(base, at) + delta);
      const uint32_t category = Load(base, Screen(base) + kCategory);
      if (int32_t(category) < 0) {
        Store(base, Screen(base) + kCategory, 4);
      } else if (int32_t(category) > 4) {
        Store(base, Screen(base) + kCategory, 0);
      } else {
        Store(base, Screen(base) + kCategory, category);
      }
    } while (FilterGroups(base, m.frame) == 0);
    for (uint32_t k = 0; k < 4; k++) {
      m.Finish(0x8219FD10, 76 + k);
    }
    m.Play(0x8219FD40, uint64_t(Load(base, Screen(base) + kCategory)) + 76, 1);
    m.Finish(0x8219FD58, uint64_t(Selection(base)) + 29);
    Store(base, Screen(base) + kGroupSelection, 0);
    Store(base, Screen(base) + kGroupScroll, 0);
    InitGroupLists(base, m.frame, calls);
    const uint32_t s = Selection(base);
    const uint64_t row = uint64_t(s) + 29;
    m.Play(0x8219FDA8, row);
    m.Loop(0x8219FDB4, row);
    DrawGroupRows(base, m.frame, calls);
    ShowGroup(base, m.frame, Load(base, Screen(base) + ((s + 0x10000 - 14799) << 2)), calls);
    GroupArrows(base, m.frame, Load(base, Load(base, kList) + 4),
                Load(base, Screen(base) + kShownCount));
  });
  return 1;
}

uint64_t OpenBuilds(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A1138 BuildScreen::OpenBuilds");
  const uint64_t done = Transition(base, step, sp, 128, 0x821A1244, calls, [&](const Movie6& m) {
    const uint32_t s = Selection(base);
    m.Finish(0x821A118C, 44);
    m.Finish(0x821A1198, 42);
    m.Finish(0x821A11A4, 43);
    m.Finish(0x821A11B0, 80);
    m.Finish(0x821A11BC, uint64_t(s) + 29);
    m.Play(0x821A11D8, 28);
    m.Finish(0x821A11E4, 28);
    m.Play(0x821A1200, 102);
    KeepGroupList(base, m.frame);
    Store(base, Screen(base) + kBuildSelection, 0);
    Store(base, Screen(base) + kBuildScroll, 0);
    SetUpBuilds(base, m.frame, calls);
  });
  return uint32_t(done) == 0 ? 1 : 0;
}

uint64_t BackToStand(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219FF78 BuildScreen::BackToStand");
  const uint64_t done = Transition(base, step, sp, 112, 0x8219FFF8, calls, [&](const Movie6& m) {
    m.Finish(0x8219FFC4, 74);
    m.Play(0x8219FFE0, 107);
    RestoreStand(base, m.frame, calls);
  });
  if (uint32_t(done) == 0) {
    Back(base, 2, 13);
  }
  return 0;
}

uint64_t OpenDetails(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219FEC8 BuildScreen::OpenDetails");
  const uint64_t done = Transition(base, step, sp, 112, 0x8219FF58, calls, [&](const Movie6& m) {
    m.Finish(0x8219FF14, 108);
    m.Finish(0x8219FF20, 93);
    m.Play(0x8219FF3C, 106);
    KeepStandList(base, m.frame);
    SetUpDetails(base, m.frame, calls);
  });
  return uint32_t(done) == 0 ? 1 : 0;
}


uint64_t OpenStand(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls,
                   const ReloadCall& reload) {
  ITC_NATIVE_RUNNING("sub_8219FE10 BuildScreen::OpenStand");
  Transition(base, step, sp, 128, 0x8219FEBC, calls, [&](const Movie6& m) {
    const uint32_t s = Selection(base);
    m.Finish(0x8219FE64, 71);
    m.Finish(0x8219FE70, 70);
    m.Finish(0x8219FE7C, 69);
    m.Finish(0x8219FE88, uint64_t(s) + 61);
    m.Play(0x8219FEA4, 104);
    SetUpStand(base, m.frame, calls, reload);
  });
  return 1;
}

}  // namespace build_screen
