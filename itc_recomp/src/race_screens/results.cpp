// The results at +1028 and +1032 (RaceResults:: and SplitResults::,
// docs/functions/race-screens-unseen.md): see unseen.h. Read from the
// generated code; never seen in play.

#include "race_screens/unseen.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_screens {

namespace {

constexpr uint64_t kNone = ~uint64_t(0);

uint32_t Get(uint8_t* base, uint32_t global) { return guest::LoadU32(base, global); }

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  guest::StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// A lis and addi pair's value as the register holds it (sign-extended).
uint64_t Register(uint32_t address) { return uint64_t(int64_t(int32_t(address))); }

// UiMovie::PlayNamed(movie, named, 0, 0, 0, 0) through its override.
uint64_t PlayNamed(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie,
                   uint64_t named) {
  const uint64_t args[8] = {movie, named, 0, 0, 0, 0, 0, 0};
  return calls.CallArgs(kPlayNamed, frame, lr, args);
}

uint64_t LoopNamed(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie,
                   uint64_t named) {
  return calls.Call(kLoopNamed, frame, lr, movie, named);
}

uint64_t FinishNamed(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie,
                     uint64_t named) {
  return calls.Call(kFinishNamed, frame, lr, movie, named);
}

uint64_t Run(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie) {
  return calls.Call(kRun, frame, lr, movie, 0);
}

// UiMovie::SetText(movie, 5, child, id, r7 to r9) through its override, the
// unset varargs 0 (the user's call for the parking area, 2026-10-30).
uint64_t SetText(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t child,
                 uint64_t id, uint64_t r7 = 0, uint64_t r8 = 0, uint64_t r9 = 0) {
  const uint64_t args[8] = {movie, 5, child, id, r7, r8, r9, 0};
  return calls.CallArgs(kSetText, frame, lr, args);
}

// The screens' movies: [0x82814E18]'s +20, [0x82814E1C]'s +8.
uint32_t ResultsMovie(uint8_t* base) { return Get(base, Get(base, kRaceResults) + 20); }
uint32_t SplitMovie(uint8_t* base) { return Get(base, Get(base, kSplitResults) + 8); }

// An action mapper entry's (8 bytes at kMapperEntries) slot 10 on `action`:
// false when the entry's +4 is 1, else the slot's low byte.
bool Pressed(uint8_t* base, uint32_t entry, uint32_t action, uint32_t frame, uint32_t lr,
             bool native_slots, const Calls& calls) {
  if (int32_t(guest::LoadU32(base, entry + 4)) == 1) return false;
  return (MapperPressed(base, guest::LoadU32(base, entry), action, frame, lr, native_slots,
                        calls) &
          0xFF) != 0;
}

uint32_t Entry(uint8_t* base, uint32_t controller) {
  return guest::LoadU32(base, controller) * 8 + kMapperEntries;
}

// A transition's first call (the step word 0): the word 1, then `work()`.
template <typename Work>
void First(uint8_t* base, uint32_t step, Work&& work) {
  if (guest::LoadU32(base, step) == 0) {
    guest::StoreU32(base, step, 1);
    work();
  }
}

}  // namespace

// --- The results at +1028 ---

uint64_t RaceResultsSetTexts(uint8_t* base, uint32_t screen, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221F628 RaceResults::SetTexts");
  const uint32_t frame = Frame(base, sp, 160);
  // The places' strings (item 7), stored in the frame as the original's.
  constexpr uint32_t kPlaces[11] = {395, 396, 397, 398, 401, 402, 404, 403, 409, 410, 411};
  uint32_t time = guest::LoadU32(base, screen + 4);
  const uint32_t movie = guest::LoadU32(base, screen + 20);
  for (uint32_t k = 0; k < 11; k++) guest::StoreU32(base, frame + 80 + 4 * k, kPlaces[k]);
  // Past 59 minutes: 59:59.999 (the sign bit kept).
  if ((time & 0x7FFF0000u) > 0x3B0000u) time = (time & 0x80000000u) | 0x3BEFE7u;
  SetText(calls, frame, 0x8221F6C8, movie, 2, 1976);
  SetText(calls, frame, 0x8221F6DC, movie, 3, 1977);
  SetText(calls, frame, 0x8221F6F0, movie, 4, 1978);
  SetText(calls, frame, 0x8221F704, movie, 5, 1979);
  SetText(calls, frame, 0x8221F724, movie, 6, 528, (time >> 16) & 0x7FFF, (time >> 10) & 0x3F,
          time & 0x3FF);
  const uint32_t place = guest::LoadU32(
      base, (guest::LoadU32(base, screen + 8) << 2) + frame + 80);
  SetText(calls, frame, 0x8221F744, movie, 7, place);
  uint64_t r3 = SetText(calls, frame, 0x8221F75C, movie, 8,
                        uint64_t(guest::LoadU32(base, screen + 12)) + 1718);
  const uint32_t result = guest::LoadU32(base, screen + 16);
  if (result < 3) {
    r3 = SetText(calls, frame, 0x8221F7C8, movie, 9, 1985 + result);
  } else if (result < 10) {
    r3 = SetText(calls, frame, 0x8221F7A0, movie, 9, 1984, uint64_t(result) + 1);
  } else if (result == 10) {
    r3 = SetText(calls, frame, 0x8221F7C8, movie, 9, 1988);
  }
  return r3;
}

uint64_t RaceResultsOpen(uint8_t* base, uint32_t screen, uint32_t block, uint32_t sp,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221F990 RaceResults::Open");
  const uint32_t frame = Frame(base, sp, 112);
  for (uint32_t i = 0; i < 5; i++) {
    guest::StoreU32(base, screen + 4 * i, guest::LoadU32(base, block + 4 * i));
  }
  guest::StoreU32(base, frame + 80, 7);
  const uint32_t movie =
      guest::LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
  guest::StoreU32(base, screen + 20, movie);
  calls.Call(kResetPose, frame, 0x8221F9EC, movie);
  PagesInit(base, screen + 24, 0);
  PagesGo(base, screen + 24, 0, 0, 1);
  return RaceResultsSetTexts(base, screen, frame, calls);
}

uint64_t RaceResultsMainPage(uint8_t* base, uint32_t sp, bool native_slots, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221F870 RaceResults::MainPage");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t entry = Entry(base, kControllerEntries);
  if (Pressed(base, entry, 159, frame, 0x8221F8BC, native_slots, calls) ||
      Pressed(base, entry, 158, frame, 0x8221F8E8, native_slots, calls)) {
    calls.Call(kMenuSoundPlay, frame, 0x8221F8F8, 0);
    PagesGo(base, Get(base, kRaceResults) + 24, 1, 2, 0xFFFFFFFFu);
  }
  return 0;
}

uint64_t RaceResultsUpdate(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                           bool native_slots, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221F930 RaceResults::Update");
  const uint32_t frame = Frame(base, sp, 96);
  // MainPage, the shared "returns 1".
  guest::StoreU32(base, frame + 80, 0x8221F870);
  guest::StoreU32(base, frame + 84, 0x825C8390);
  const int32_t page = int32_t(guest::LoadU32(base, uint32_t(r3) + 24));
  switch (page) {
    case -1:
      return 0;
    case 0:
      return RaceResultsMainPage(base, frame, native_slots, calls);
    case 1:
      return vehicle_accessors::DebugSwitchOn();
    default: {
      // Past the table: the frame's word, as the original's bctrl.
      const uint32_t handler = guest::LoadU32(base, (uint32_t(page) << 2) + frame + 80);
      const uint64_t regs[8] = {r3, r4, 0, 0, 0, 0, Register(0x8221F870), uint64_t(frame) + 80};
      return calls.CallArgs(handler, frame, 0x8221F974, regs);
    }
  }
}

uint64_t RaceResultsIntro(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221F428 RaceResults::Intro");
  const uint32_t frame = Frame(base, sp, 96);
  First(base, step, [&] {
    const uint32_t screen = Get(base, kRaceResults);
    const int32_t kind = int32_t(guest::LoadU32(base, screen));
    PlayNamed(calls, frame, 0x8221F480, guest::LoadU32(base, screen + 20), kind == 0 ? 3 : 2);
  });
  return uint32_t(Run(calls, frame, 0x8221F490, ResultsMovie(base))) == 0 ? 1 : 0;
}

uint64_t RaceResultsShow(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221F4B0 RaceResults::Show");
  const uint32_t frame = Frame(base, sp, 112);
  First(base, step, [&] {
    const uint32_t movie = ResultsMovie(base);
    PlayNamed(calls, frame, 0x8221F500, movie, 4);
    LoopNamed(calls, frame, 0x8221F50C, movie, 4);
    if (int32_t(guest::LoadU32(base, Get(base, kRaceResults))) != 0) {
      PlayNamed(calls, frame, 0x8221F538, movie, 6);
      LoopNamed(calls, frame, 0x8221F554, movie, 6);
    } else {
      PlayNamed(calls, frame, 0x8221F548, movie, 7);
      LoopNamed(calls, frame, 0x8221F554, movie, 7);
    }
  });
  Run(calls, frame, 0x8221F564, ResultsMovie(base));
  return 1;
}

uint64_t RaceResultsClose(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221F580 RaceResults::Close");
  const uint32_t frame = Frame(base, sp, 112);
  First(base, step, [&] {
    const uint32_t movie = ResultsMovie(base);
    FinishNamed(calls, frame, 0x8221F5C0, movie, 6);
    FinishNamed(calls, frame, 0x8221F5CC, movie, 7);
    FinishNamed(calls, frame, 0x8221F5D8, movie, 4);
    PlayNamed(calls, frame, 0x8221F5F4, movie, 5);
  });
  return uint32_t(Run(calls, frame, 0x8221F604, ResultsMovie(base))) == 0 ? 1 : 0;
}

uint64_t RaceResultsDraw(uint8_t* base, uint32_t screen, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221F7D0 RaceResults::Draw");
  const uint32_t frame = Frame(base, sp, 112);
  // Intro, Show, Close.
  constexpr uint32_t kHandlers[3] = {0x8221F428, 0x8221F4B0, 0x8221F580};
  const int32_t transition = int32_t(guest::LoadU32(base, screen + 36));
  for (uint32_t k = 0; k < 3; k++) guest::StoreU32(base, frame + 80 + 4 * k, kHandlers[k]);
  if (transition < 0) {
    return Run(calls, frame, 0x8221F858, guest::LoadU32(base, screen + 20));
  }
  const uint32_t step = screen + 48;
  uint64_t result;
  switch (transition) {
    case 0: result = RaceResultsIntro(base, step, frame, calls); break;
    case 1: result = RaceResultsShow(base, step, frame, calls); break;
    case 2: result = RaceResultsClose(base, step, frame, calls); break;
    default: {
      // Past the table: the frame's word, as the original's bctrl.
      const uint32_t handler = guest::LoadU32(base, (uint32_t(transition) << 2) + frame + 80);
      const uint64_t regs[8] = {step, 0, 0, 0, 0, Register(kHandlers[0]), Register(kHandlers[1]),
                                uint64_t(frame) + 80};
      result = calls.CallArgs(handler, frame, 0x8221F834, regs);
    }
  }
  PagesFinish(base, screen + 24, uint32_t(result));
  return uint64_t(screen) + 24;
}

// --- The results at +1032 ---

uint64_t SplitResultsOpen(uint8_t* base, uint32_t screen, uint64_t outcome, uint64_t mode,
                          uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82220050 SplitResults::Open");
  const uint32_t frame = Frame(base, sp, 112);
  guest::StoreU32(base, screen + 4, uint32_t(mode));
  guest::StoreU32(base, screen + 0, uint32_t(outcome));
  guest::StoreU32(base, frame + 80, 8);
  const uint32_t movie =
      guest::LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
  guest::StoreU32(base, screen + 8, movie);
  calls.Call(kResetPose, frame, 0x82220094, movie);
  PagesInit(base, screen + 12, 0);
  PagesGo(base, screen + 12, 0, 0, 1);
  guest::StoreU32(base, kSplitTicks, 0);
  guest::StoreU32(base, kSplitExchanged, 0);
  return uint64_t(screen) + 12;
}

uint64_t SplitResultsMainPage(uint8_t* base, uint32_t sp, bool native_slots, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221FE70 SplitResults::MainPage");
  const uint32_t frame = Frame(base, sp, 192);
  const uint32_t first = Entry(base, kControllerEntries);
  const uint32_t second = Entry(base, kControllerEntries + 4);
  // Stages::Copy of the current stage into the frame's +80 (its plain C++,
  // or the original through its adapter where it declines).
  const uint32_t stages = Get(base, kRaceData) + 0x59620;
  if (!vehicle_accessors::StagesCopy(base, frame + 80, stages, 0xFFFFFFFFu)) {
    calls.Call(kStagesCopy, frame, 0x8221FEC0, frame + 80, stages, kNone);
  }
  if (int32_t(guest::LoadU32(base, Get(base, kRaceData) + 0xA3AF0)) == 6 &&
      int32_t(guest::LoadU32(base, frame + 80)) != 1) {
    // The network battle: once the other console's result came, 210 ticks
    // counted, then the screen closed (no sound).
    if (int32_t(Get(base, kSplitExchanged)) == 1) {
      const uint32_t ticks = Get(base, kSplitTicks) + 1;
      guest::StoreU32(base, kSplitTicks, ticks);
      if (ticks > 210) PagesGo(base, Get(base, kSplitResults) + 12, 1, 2, 0xFFFFFFFFu);
    }
    return 0;
  }
  if (Pressed(base, first, 159, frame, 0x8221FF34, native_slots, calls) ||
      Pressed(base, first, 158, frame, 0x8221FF60, native_slots, calls) ||
      Pressed(base, second, 159, frame, 0x8221FF8C, native_slots, calls) ||
      Pressed(base, second, 158, frame, 0x8221FFB8, native_slots, calls)) {
    calls.Call(kMenuSoundPlay, frame, 0x8221FFC8, 0);
    PagesGo(base, Get(base, kSplitResults) + 12, 1, 2, 0xFFFFFFFFu);
  }
  return 0;
}

uint64_t SplitResultsUpdate(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                            bool native_slots, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221FFF0 SplitResults::Update");
  const uint32_t frame = Frame(base, sp, 96);
  // MainPage, the shared "returns 1".
  guest::StoreU32(base, frame + 80, 0x8221FE70);
  guest::StoreU32(base, frame + 84, 0x825C8390);
  const int32_t page = int32_t(guest::LoadU32(base, uint32_t(r3) + 12));
  switch (page) {
    case -1:
      return 0;
    case 0:
      return SplitResultsMainPage(base, frame, native_slots, calls);
    case 1:
      return vehicle_accessors::DebugSwitchOn();
    default: {
      const uint32_t handler = guest::LoadU32(base, (uint32_t(page) << 2) + frame + 80);
      const uint64_t regs[8] = {r3, r4, 0, 0, 0, 0, Register(0x8221FE70), uint64_t(frame) + 80};
      return calls.CallArgs(handler, frame, 0x82220034, regs);
    }
  }
}

uint64_t SplitResultsIntro(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221FAC8 SplitResults::Intro");
  const uint32_t frame = Frame(base, sp, 112);
  First(base, step, [&] {
    const uint32_t mode = guest::LoadU32(base, Get(base, kSplitResults) + 4);
    const uint32_t movie = SplitMovie(base);
    if (mode > 2) return;
    // The group -1 node 0's +80: 1 for modes 0 and 2, 0 for mode 1.
    const uint32_t node = ui_movies::FindNode(base, movie, 0xFFFFFFFFu, 0);
    guest::StoreU32(base, node + 80, mode == 1 ? 0 : 1);
    uint64_t named;
    if (mode == 2) {
      named = 5;
    } else {
      const uint32_t outcome = guest::LoadU32(base, Get(base, kSplitResults));
      if (outcome > 2) return;
      static constexpr uint64_t kNamed[2][3] = {{0, 1, 2}, {3, 4, 2}};
      named = kNamed[mode][outcome];
    }
    PlayNamed(calls, frame, 0x8221FBC8, movie, named);
  });
  return uint32_t(Run(calls, frame, 0x8221FBD8, SplitMovie(base))) == 0 ? 1 : 0;
}

uint64_t SplitResultsShow(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221FBE8 SplitResults::Show");
  const uint32_t frame = Frame(base, sp, 112);
  First(base, step, [&] {
    const uint32_t movie = SplitMovie(base);
    PlayNamed(calls, frame, 0x8221FC38, movie, 6);
    LoopNamed(calls, frame, 0x8221FC44, movie, 6);
    const uint32_t mode = guest::LoadU32(base, Get(base, kSplitResults) + 4);
    if (mode > 2) return;
    uint64_t named;
    uint32_t lr;
    if (mode == 2) {
      named = 13;
      lr = 0x8221FC7C;
    } else {
      const uint32_t outcome = guest::LoadU32(base, Get(base, kSplitResults));
      if (outcome > 2) return;
      static constexpr uint64_t kNamed[2][3] = {{8, 9, 10}, {11, 12, 10}};
      static constexpr uint32_t kLr[2][3] = {{0x8221FD60, 0x8221FD3C, 0x8221FCBC},
                                             {0x8221FD04, 0x8221FCE0, 0x8221FCBC}};
      named = kNamed[mode][outcome];
      lr = kLr[mode][outcome];
    }
    PlayNamed(calls, frame, lr, movie, named);
    LoopNamed(calls, frame, 0x8221FD6C, movie, named);
  });
  Run(calls, frame, 0x8221FD7C, SplitMovie(base));
  return 1;
}

uint64_t SplitResultsClose(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221FD98 SplitResults::Close");
  const uint32_t frame = Frame(base, sp, 112);
  First(base, step, [&] {
    const uint32_t movie = SplitMovie(base);
    constexpr uint32_t kFinished[7][2] = {{8, 0x8221FDD8},  {9, 0x8221FDE4},  {10, 0x8221FDF0},
                                          {6, 0x8221FDFC},  {11, 0x8221FE08}, {12, 0x8221FE14},
                                          {13, 0x8221FE20}};
    for (const auto& f : kFinished) FinishNamed(calls, frame, f[1], movie, f[0]);
    PlayNamed(calls, frame, 0x8221FE3C, movie, 7);
  });
  return uint32_t(Run(calls, frame, 0x8221FE4C, SplitMovie(base))) == 0 ? 1 : 0;
}

}  // namespace race_screens
