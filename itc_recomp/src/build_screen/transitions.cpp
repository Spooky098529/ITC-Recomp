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

}  // namespace build_screen
