// Today's result's machine, pages and transitions
// (docs/functions/todays-result.md): see todays_result.h. The varargs UI
// movie setters get 0 for the arguments their callers leave unset (the
// user's call of 2026-10-30).

#include "base/log.h"
#include "career/career.h"
#include "todays_result/internal.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>

namespace todays_result {

namespace {

// The pages' table in Draw's frame: WaitPage, LeavePage.
constexpr uint32_t kPageTable[2] = {0x8225A9B0, 0x8225AB30};
// The transitions' table in Update's frame (+88 on): Intro, ShowLoop,
// FadeOut.
constexpr uint32_t kTransitionTable[3] = {0x8225ABD8, 0x8225AC88, 0x8225AD20};

// The floats ReadDay loads: 0.0 and the percentage's 100.
constexpr uint32_t kZero = 0x82000A34;
constexpr uint32_t kHundred = 0x82002A0C;

constexpr uint64_t kAll = ~uint64_t(0);

uint32_t Pages(uint8_t* base) { return Load(base, kResultPages); }
uint32_t Data(uint8_t* base) { return Load(base, kResultData); }

uint64_t RunMovie(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie) {
  return calls.Call(kRun, frame, lr, movie, 0);
}

void Store64(uint8_t* base, uint32_t at, uint64_t value) {
  Store(base, at, uint32_t(value >> 32));
  Store(base, at + 4, uint32_t(value));
}

int32_t Half(uint8_t* base, uint32_t at) { return int16_t(guest::LoadU16(base, at)); }

// A count over another as the originals take its percentage: both
// converted from their 64-bit words at the frame's `first` and `second`
// (stored there first, as the original), rounded to floats, divided and
// multiplied by `hundred` in doubles, each rounded to a float.
float Percent(uint8_t* base, uint32_t first, uint32_t second, int64_t a, int64_t b,
              double hundred) {
  Store64(base, first, uint64_t(a));
  Store64(base, second, uint64_t(b));
  const double fa = double(float(double(a)));
  const double fb = double(float(double(b)));
  const double ratio = double(float(fa / fb));
  return float(ratio * hundred);
}

// UiMovie::SetText(movie, -1, node, string, value) with r8 to r10 0.
void SetText(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint32_t node,
             uint64_t string, uint64_t value) {
  const uint64_t args[8] = {movie, kAll, node, string, value, 0, 0, 0};
  calls.CallArgs(kSetText, frame, lr, args);
}
// The same with a double (f1, and its bits in r7, stored at the frame's +48
// as the original passes it).
void SetTextDouble(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie,
                   uint32_t node, uint64_t string, float value) {
  const double f1 = double(value);
  const uint64_t bits = std::bit_cast<uint64_t>(f1);
  Store64(base, frame + 48, bits);
  const uint64_t args[8] = {movie, kAll, node, string, bits, 0, 0, 0};
  calls.call(calls.user, kSetText, args, &f1, frame, lr);
}

}  // namespace

uint64_t ResultCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225A7A0 TodaysResult::Create");
  const uint32_t frame = Frame(base, sp, 112);
  Store(base, machine + kMachineLoaded, 0);
  const uint32_t pages = uint32_t(calls.Call(kVirtualAlloc, frame, 0x8225A7D4, 0, 28, 0x1000, 4));
  Store(base, kResultPages, pages);
  Store(base, pages + 4, 0);
  Store(base, pages + 0, 0);
  Store(base, pages + 8, 0);
  Store(base, pages + 24, 0);
  Store(base, pages + 20, kNone);
  Store(base, pages + 12, kNone);
  const uint32_t menu = uint32_t(calls.Call(kVirtualAlloc, frame, 0x8225A810, 0, 68, 0x1000, 4));
  Store(base, kResultMenu, menu);
  for (const uint32_t field : {12u, 20u, 0u, 4u}) {
    Store(base, menu + field, 0);
  }
  Store(base, menu + 16, 4);
  for (const uint32_t field : {8u, 40u, 48u, 52u, 36u, 64u, 24u, 28u, 32u}) {
    Store(base, menu + field, 0);
  }
  const uint64_t r3 = calls.Call(kVirtualAlloc, frame, 0x8225A86C, 0, 68, 0x1000, 4);
  const uint32_t data = uint32_t(r3);
  Store(base, kResultData, data);
  Store(base, data + 0, 0);
  Store(base, machine + kMachineState, Load(base, machine + kMachineReenter) != 0 ? 1 : 9);
  return r3;
}

uint64_t ResultOpenMovie(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225A510 TodaysResult::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, kResultMovie, kResultAsset);
}

uint64_t ResultWaitForMovie(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8225A930 TodaysResult::WaitForMovie");
  const uint32_t machine = uint32_t(r3);
  Frame(base, sp, 96);
  if (Load(base, machine + kMachineLoaded) == 0) {
    r3 = ui_movies::IsLoaded(base, kResultMovie);
    if (uint32_t(r3) != 0) {
      Store(base, machine + kMachineLoaded, 1);
    }
  }
  if (Load(base, machine + kMachineLoaded) != 0) {
    Store(base, machine + kMachineState, Load(base, machine + kMachineReenter) != 0 ? 9 : 4);
  }
  return r3;
}

uint64_t ReadDay(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225A520 TodaysResult::ReadDay");
  const uint32_t frame = Frame(base, sp, kReadDayFrame);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  int32_t day = int32_t(Load(base, save + 6100));
  // The stats record (6,044 bytes from the save's +48) at the frame's +4944,
  // and its day block (4,840 bytes from its +24) at +96.
  const uint32_t stats = frame + 4944, block = frame + 96;
  calls.Call(kMemcpy, frame, 0x8225A55C, stats, save + 48, 6044);
  calls.Call(kMemcpy, frame, 0x8225A56C, block, stats + 24, 4840);
  Store(base, Data(base) + 56, Load(base, save + 6104));
  Store(base, Data(base) + 60, uint32_t(day - (day / 7) * 7));
  if (day > 999) {
    day = 999;
  }
  Store(base, Data(base) + 64, uint32_t(day));
  // The day's wins, losses and outcome 3.
  const int32_t wins = Half(base, block + 12);
  Store(base, Data(base) + 8, uint32_t(wins));
  Store(base, Data(base) + 12, uint32_t(Half(base, block + 14)));
  Store(base, Data(base) + 16, uint32_t(Half(base, block + 16)));
  const float zero = guest::LoadFloat(base, kZero);
  const double hundred = double(guest::LoadFloat(base, kHundred));
  if (wins == 0) {
    guest::StoreFloat(base, Data(base) + 20, float(double(zero)));
  } else {
    const int32_t races = Half(base, block + 22);
    guest::StoreFloat(base, Data(base) + 20,
                      Percent(base, frame + 88, frame + 80, wins, races, hundred));
  }
  const uint64_t streak = calls.Call(kWinStreak, frame, 0x8225A630, block);
  Store(base, Data(base) + 24, uint32_t(int32_t(int16_t(uint16_t(streak)))));
  const uint64_t word = calls.Call(kWord4832, frame, 0x8225A648, block);
  Store(base, Data(base) + 28, uint32_t(word));
  // The career's.
  const int32_t career_wins = Half(base, stats + 12);
  Store(base, Data(base) + 32, uint32_t(career_wins));
  Store(base, Data(base) + 36, uint32_t(Half(base, stats + 14)));
  Store(base, Data(base) + 40, uint32_t(Half(base, stats + 16)));
  if (career_wins == 0) {
    guest::StoreFloat(base, Data(base) + 44, float(double(zero)));
  } else {
    const int32_t races = Half(base, stats + 22);
    guest::StoreFloat(base, Data(base) + 44,
                      Percent(base, frame + 80, frame + 88, career_wins, races, hundred));
  }
  Store(base, Data(base) + 48, uint32_t(Half(base, stats + 5968)));
  Store(base, Data(base) + 52, Load(base, save + 6116));
  const uint64_t r3 =
      calls.Call(kCareerNextDay, frame, 0x8225A6E4, vehicle_accessors::SaveDataOf(base, 0));
  const uint32_t data = Data(base);
  for (uint32_t k = 0; k < 5; k++) {
    Store(base, data + 68 + 4 * k, Load(base, save + 8 + 4 * k));
  }
  return r3;
}

uint64_t SetTexts(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225ADE0 TodaysResult::SetTexts");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80, kResultMovie);
  // The labels.
  struct Label {
    uint32_t node, string, lr;
  };
  static constexpr Label kLabels[] = {
      {29, 284, 0x8225AE24}, {30, 285, 0x8225AE38}, {31, 286, 0x8225AE4C},
      {32, 287, 0x8225AE60}, {33, 288, 0x8225AE74}, {34, 292, 0x8225AE88},
      {35, 295, 0x8225AE9C}, {16, 296, 0x8225AEB0}, {43, 435, 0x8225AEC4},
      {51, 435, 0x8225AED8}, {40, 297, 0x8225AEEC}, {48, 297, 0x8225AF00},
  };
  for (const Label& label : kLabels) {
    SetText(calls, frame, label.lr, movie, label.node, label.string, 0);
  }
  auto data = [&](uint32_t offset) { return Load(base, Data(base) + offset); };
  SetText(calls, frame, 0x8225AF20, movie, 15, 8, data(64));
  SetText(calls, frame, 0x8225AF3C, movie, 14, uint64_t(data(60)) + 464, 0);
  SetText(calls, frame, 0x8225AF58, movie, 36, 8, data(8));
  SetText(calls, frame, 0x8225AF74, movie, 37, 8, data(12));
  SetText(calls, frame, 0x8225AF90, movie, 38, 8, data(16));
  SetTextDouble(base, calls, frame, 0x8225AFB4, movie, 39, 436,
                guest::LoadFloat(base, Data(base) + 20));
  SetText(calls, frame, 0x8225AFD0, movie, 41, 8, data(24));
  calls.Call(kSetNumberText, frame, 0x8225AFE8, movie, kAll, 42, data(28));
  SetText(calls, frame, 0x8225B004, movie, 44, 8, data(32));
  SetText(calls, frame, 0x8225B020, movie, 45, 8, data(36));
  SetText(calls, frame, 0x8225B03C, movie, 46, 8, data(40));
  SetTextDouble(base, calls, frame, 0x8225B060, movie, 47, 436,
                guest::LoadFloat(base, Data(base) + 44));
  SetText(calls, frame, 0x8225B07C, movie, 49, 8, data(48));
  calls.Call(kSetNumberText, frame, 0x8225B094, movie, kAll, 50, data(52));
  return calls.Call(kSetChildNameText, frame, 0x8225B0AC, movie, kAll, 7, Data(base) + 68);
}

uint64_t SetUpMovie(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225B258 TodaysResult::SetUpMovie");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, frame + 80, kResultMovie);
  SetTexts(base, frame, calls);
  Store(base, Data(base) + 0, 1);
  Store(base, ui_movies::FindNode(base, movie, 1, 1) + 80, 0);
  Store(base, ui_movies::FindNode(base, movie, 1, 2) + 80, 0);
  const uint32_t fourth = ui_movies::FindNode(base, movie, 1, 4);
  const uint32_t fifth = ui_movies::FindNode(base, movie, 1, 5);
  Store(base, fourth + 108, 0);
  Store(base, fifth + 108, 0);
  return fifth;
}

uint64_t ResultStart(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225B300 TodaysResult::Start");
  const uint32_t frame = Frame(base, sp, 96);
  ReadDay(base, frame, calls);
  const uint64_t r3 = SetUpMovie(base, frame, calls);
  const uint32_t pages = Pages(base);
  const uint32_t page = Load(base, pages + 0);
  if (page != 0) {
    Store(base, pages + 8, page);
  }
  Store(base, pages + 12, 0);
  Store(base, pages + 16, 1);
  Store(base, pages + 4, 0);
  Store(base, pages + 0, kNone);
  return r3;
}

uint64_t ResultDraw(uint8_t* base, uint64_t r3, uint64_t dispatcher, uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225B0C8 TodaysResult::Draw");
  const uint32_t frame = Frame(base, sp, 96);
  Store(base, frame + 80, kPageTable[0]);
  Store(base, frame + 84, kPageTable[1]);
  const uint32_t page = Load(base, Pages(base) + 0);
  if (page == kNone) {
    return r3;
  }
  // No bounds check, as the original: the word at the table's index.
  return calls.Call(Load(base, frame + 80 + (page << 2)), frame, 0x8225B118, dispatcher,
                    dispatcher);
}

uint64_t ResultUpdate(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225B128 TodaysResult::Update");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, frame + 80, kResultMovie);
  const uint32_t pages = Pages(base);
  for (uint32_t k = 0; k < 3; k++) {
    Store(base, frame + 88 + 4 * k, kTransitionTable[k]);
  }
  const uint32_t running = Load(base, pages + 12);
  if (running == kNone) {
    if (Load(base, pages + 0) == 1) {
      return movie;
    }
    return RunMovie(calls, frame, 0x8225B240, movie);
  }
  uint64_t r3 = movie;
  uint32_t p = pages;
  if (int32_t(running) >= 0) {
    // No bounds check: the word at the table's index.
    r3 = calls.Call(Load(base, frame + 88 + (running << 2)), frame, 0x8225B1C4, pages + 24);
    p = Pages(base);
    if (uint32_t(r3) == 0) {
      return r3;
    }
  }
  const uint32_t queued = Load(base, p + 16);
  if (queued == kNone) {
    if (int32_t(running) >= 0) {
      Store(base, p + 0, Load(base, p + 4));
      Store(base, p + 12, queued);
    } else {
      const uint32_t next = Load(base, p + 4);
      Store(base, p + 12, kNone);
      Store(base, p + 0, next);
    }
  } else {
    Store(base, p + 16, kNone);
    Store(base, p + 20, 0);
    Store(base, p + 12, queued);
  }
  Store(base, p + 24, 0);
  return r3;
}

uint64_t ResultWaitPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225A9B0 TodaysResult::WaitPage");
  const uint32_t frame = Frame(base, sp, 96);
  const uint64_t pressed =
      calls.Call(kMenuPressed, frame, 0x8225A9CC, Load(base, kResultMenu), kAccept);
  if (uint32_t(pressed) == 0) {
    return pressed;
  }
  const uint32_t pages = Pages(base);
  KeepPage(base, pages, 1);
  Store(base, pages + 16, kNone);
  Store(base, pages + 12, 2);
  Store(base, pages + 4, 1);
  Store(base, pages + 0, kNone);
  return calls.Call(kMenuSoundPlay, frame, 0x8225AA10, 0);
}

uint64_t ResultQueueMovie(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225A720 TodaysResult::QueueMovie");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t movie = save + kQueuedMovie;
  for (uint32_t k = 0; k < 5; k++) {
    Store(base, kBootMovies + 4 * k, 11);
  }
  Store(base, kBootMovies + 20, 13);
  Store(base, kBootMovies + 28, 0);
  Store(base, kBootMovies + 24, 1);
  Store(base, kBootMovies + 0, Load(base, movie));
  Store(base, movie, kNone);
  Store(base, kBootMovies + 20, 2);
  // Progress::CheckUnlocks: its plain C++ where Condition::Test's copies fit,
  // else the original through the guest.
  if (career::CheckUnlocksNative(base, frame)) {
    return career::ProgressCheckUnlocks(base, save, frame);
  }
  return calls.Call(kCheckUnlocks, frame, 0x8225A790, save);
}

uint64_t ResultLeavePage(uint8_t* base, uint32_t dispatcher, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225AB30 TodaysResult::LeavePage");
  const uint32_t frame = Frame(base, sp, 96);
  Post(base, dispatcher, 5);
  uint64_t r3 = vehicle_accessors::SaveDataOf(base, 0);
  uint32_t queued = 0;
  if (Load(base, uint32_t(r3) + kQueuedMovie) != kNone) {
    r3 = ResultQueueMovie(base, frame, calls);
    queued = 1;
  }
  Store(base, Data(base) + 4, queued);
  // RaceEvent::Destroy reads no argument (r3 as left).
  return calls.Call(kRaceEventDestroy, frame, 0x8225ABC4, r3);
}

uint64_t ResultIntro(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225ABD8 TodaysResult::Intro");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80, kResultMovie);
  if (Load(base, started) == 0) {
    calls.Call(kPlayNamed, frame, 0x8225AC34, movie, 5);
    calls.Call(kFadeIn, frame, 0x8225AC50, Load(base, kFadesIn), 0, 0, 0);
    Store(base, started, 1);
  }
  return uint32_t(RunMovie(calls, frame, 0x8225AC64, movie)) == 0 ? 1 : 0;
}

uint64_t ResultShowLoop(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225AC88 TodaysResult::ShowLoop");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80, kResultMovie);
  if (Load(base, started) == 0) {
    calls.Call(kPlayNamed, frame, 0x8225ACE4, movie, 4);
    calls.Call(kLoopNamed, frame, 0x8225ACF0, movie, 4);
    Store(base, started, 1);
  }
  RunMovie(calls, frame, 0x8225AD04, movie);
  return 1;
}

uint64_t ResultFadeOut(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225AD20 TodaysResult::FadeOut");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80, kResultMovie);
  if (Load(base, started) == 0) {
    calls.Call(kFinishNamed, frame, 0x8225AD6C, movie, 4);
    calls.Call(kFadeOut, frame, 0x8225AD88, Load(base, kFadesOut), 0, 0, 0);
    Store(base, started, 1);
  }
  RunMovie(calls, frame, 0x8225AD9C, movie);
  return (Load(base, kFadeCount0) | Load(base, kFadeCount1)) == 0 ? 1 : 0;
}

uint64_t ResultUnload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225AA20 TodaysResult::Unload");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80, kResultMovie);
  uint64_t r3 = calls.Call(kFinishAll, frame, 0x8225AA54, movie);
  if (Load(base, machine + kMachineDelete) != 0) {
    r3 = calls.Call(kMoviesDelete, frame, 0x8225AA68, kResultMovie);
    Store(base, machine + kMachineLoaded, 0);
  }
  return r3;
}

void ResultUnloaded(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_8225A9A0 TodaysResult::Unloaded");
  Store(base, machine + kMachineState, 10);
}

uint64_t ResultLeave(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225AA88 TodaysResult::Leave");
  const uint32_t frame = Frame(base, sp, 112);
  Store(base, machine + kMachineState, 7);
  const uint32_t movie = FindMovie(base, frame + 80, kResultMovie);
  calls.Call(kFinishAll, frame, 0x8225AAC8, movie);
  const uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x8225AAD0, kResultMovie);
  Post(base, dispatcher, 4);
  return r3;
}

uint64_t ResultDestroy(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225A8A8 TodaysResult::Destroy");
  const uint32_t frame = Frame(base, sp, 128);
  Store(base, machine + kMachineState, Load(base, Data(base) + 4) != 0 ? 20 : 13);
  calls.Call(kVirtualFree, frame, 0x8225A8F0, Load(base, kResultPages), 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x8225A904, Load(base, kResultMenu), 0, kMemRelease);
  const uint64_t r3 =
      calls.Call(kVirtualFree, frame, 0x8225A914, Load(base, kResultData), 0, kMemRelease);
  Store(base, kResultPages, 0);
  Store(base, kResultMenu, 0);
  Store(base, kResultData, 0);
  return r3;
}

}  // namespace todays_result
