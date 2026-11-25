// The results at +1028 and +1032 (RaceResults:: and SplitResults::,
// docs/functions/race-screens-unseen.md): see unseen.h. Read from the
// generated code; never seen in play.

#include "race_screens/unseen.h"

#include "base/log.h"
#include "guest.h"
#include "schema/race_hud_machine.h"
#include "schema/race_screens_unseen.h"
#include "schema/race_states.h"
#include "schema/tuning_screen.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_screens {

namespace {

constexpr uint64_t kNone = ~uint64_t(0);

using Globals = schema::RaceScreensUnseenGlobals;

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  guest::StoreU32(sp - bytes, sp);
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
uint32_t ResultsMovie() { return Globals::race_results().movie().address; }
uint32_t SplitMovie() { return Globals::split_results().movie().address; }

// An action mapper entry's (8 bytes at kMapperEntries) slot 10 on `action`:
// false when the entry's +4 is 1, else the slot's low byte.
bool Pressed(uint32_t entry, uint32_t action, uint32_t frame, uint32_t lr,
             bool native_slots, const Calls& calls) {
  if (int32_t(guest::LoadU32(entry + 4)) == 1) return false;
  return (MapperPressed(guest::LoadU32(entry), action, frame, lr, native_slots,
                        calls) &
          0xFF) != 0;
}

uint32_t Entry(uint32_t controller) {
  return guest::LoadU32(controller) * 8 + kMapperEntries;
}

// A transition's first call (the step word 0): the word 1, then `work()`.
template <typename Work>
void First(uint32_t step, Work&& work) {
  if (guest::LoadU32(step) == 0) {
    guest::StoreU32(step, 1);
    work();
  }
}

}  // namespace

// --- The results at +1028 ---

uint64_t RaceResultsSetTexts(uint32_t screen, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221F628 RaceResults::SetTexts");
  const uint32_t frame = Frame(sp, 160);
  // The places' strings (item 7), stored in the frame as the original's.
  constexpr uint32_t kPlaces[11] = {395, 396, 397, 398, 401, 402, 404, 403, 409, 410, 411};
  const schema::RaceResults results(screen);
  uint32_t time = results.time();
  const uint32_t movie = results.movie().address;
  for (uint32_t k = 0; k < 11; k++) guest::StoreU32(frame + 80 + 4 * k, kPlaces[k]);
  // Past 59 minutes: 59:59.999 (the sign bit kept).
  if ((time & 0x7FFF0000u) > 0x3B0000u) time = (time & 0x80000000u) | 0x3BEFE7u;
  SetText(calls, frame, 0x8221F6C8, movie, 2, 1976);
  SetText(calls, frame, 0x8221F6DC, movie, 3, 1977);
  SetText(calls, frame, 0x8221F6F0, movie, 4, 1978);
  SetText(calls, frame, 0x8221F704, movie, 5, 1979);
  SetText(calls, frame, 0x8221F724, movie, 6, 528, (time >> 16) & 0x7FFF, (time >> 10) & 0x3F,
          time & 0x3FF);
  const uint32_t place = guest::LoadU32((results.place() << 2) + frame + 80);
  SetText(calls, frame, 0x8221F744, movie, 7, place);
  uint64_t r3 = SetText(calls, frame, 0x8221F75C, movie, 8,
                        uint64_t(results.race_word_60()) + 1718);
  const uint32_t result = results.players_word_2196();
  if (result < 3) {
    r3 = SetText(calls, frame, 0x8221F7C8, movie, 9, 1985 + result);
  } else if (result < 10) {
    r3 = SetText(calls, frame, 0x8221F7A0, movie, 9, 1984, uint64_t(result) + 1);
  } else if (result == 10) {
    r3 = SetText(calls, frame, 0x8221F7C8, movie, 9, 1988);
  }
  return r3;
}

uint64_t RaceResultsOpen(uint32_t screen, uint32_t block, uint32_t sp,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221F990 RaceResults::Open");
  const uint32_t frame = Frame(sp, 112);
  const schema::RaceResults results(screen);
  // The block's five words (+0 to +16).
  for (uint32_t i = 0; i < 5; i++) {
    guest::StoreU32(results.shown_flag_address() + 4 * i, guest::LoadU32(block + 4 * i));
  }
  guest::StoreU32(frame + 80, 7);
  const uint32_t movie =
      guest::LoadU32(ui_movies::PairsFind(ui_movies::kMovieTable, frame + 80));
  results.set_movie(movie);
  calls.Call(kResetPose, frame, 0x8221F9EC, movie);
  PagesInit(results.pages_page_address(), 0);
  PagesGo(results.pages_page_address(), 0, 0, 1);
  return RaceResultsSetTexts(screen, frame, calls);
}

uint64_t RaceResultsMainPage(uint32_t sp, bool native_slots, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221F870 RaceResults::MainPage");
  const uint32_t frame = Frame(sp, 96);
  const uint32_t entry = Entry(kControllerEntries);
  if (Pressed(entry, 159, frame, 0x8221F8BC, native_slots, calls) ||
      Pressed(entry, 158, frame, 0x8221F8E8, native_slots, calls)) {
    calls.Call(kMenuSoundPlay, frame, 0x8221F8F8, 0);
    PagesGo(Globals::race_results().pages_page_address(), 1, 2, 0xFFFFFFFFu);
  }
  return 0;
}

uint64_t RaceResultsUpdate(uint64_t r3, uint64_t r4, uint32_t sp,
                           bool native_slots, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221F930 RaceResults::Update");
  const uint32_t frame = Frame(sp, 96);
  // MainPage, the shared "returns 1".
  guest::StoreU32(frame + 80, 0x8221F870);
  guest::StoreU32(frame + 84, 0x825C8390);
  const int32_t page = int32_t(schema::RaceResults(uint32_t(r3)).pages_page());
  switch (page) {
    case -1:
      return 0;
    case 0:
      return RaceResultsMainPage(frame, native_slots, calls);
    case 1:
      return vehicle_accessors::DebugSwitchOn();
    default: {
      // Past the table: the frame's word, as the original's bctrl.
      const uint32_t handler = guest::LoadU32((uint32_t(page) << 2) + frame + 80);
      const uint64_t regs[8] = {r3, r4, 0, 0, 0, 0, Register(0x8221F870), uint64_t(frame) + 80};
      return calls.CallArgs(handler, frame, 0x8221F974, regs);
    }
  }
}

uint64_t RaceResultsIntro(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221F428 RaceResults::Intro");
  const uint32_t frame = Frame(sp, 96);
  First(step, [&] {
    const schema::RaceResults results = Globals::race_results();
    const int32_t kind = int32_t(results.shown_flag());
    PlayNamed(calls, frame, 0x8221F480, results.movie().address, kind == 0 ? 3 : 2);
  });
  return uint32_t(Run(calls, frame, 0x8221F490, ResultsMovie())) == 0 ? 1 : 0;
}

uint64_t RaceResultsShow(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221F4B0 RaceResults::Show");
  const uint32_t frame = Frame(sp, 112);
  First(step, [&] {
    const uint32_t movie = ResultsMovie();
    PlayNamed(calls, frame, 0x8221F500, movie, 4);
    LoopNamed(calls, frame, 0x8221F50C, movie, 4);
    if (int32_t(Globals::race_results().shown_flag()) != 0) {
      PlayNamed(calls, frame, 0x8221F538, movie, 6);
      LoopNamed(calls, frame, 0x8221F554, movie, 6);
    } else {
      PlayNamed(calls, frame, 0x8221F548, movie, 7);
      LoopNamed(calls, frame, 0x8221F554, movie, 7);
    }
  });
  Run(calls, frame, 0x8221F564, ResultsMovie());
  return 1;
}

uint64_t RaceResultsClose(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221F580 RaceResults::Close");
  const uint32_t frame = Frame(sp, 112);
  First(step, [&] {
    const uint32_t movie = ResultsMovie();
    FinishNamed(calls, frame, 0x8221F5C0, movie, 6);
    FinishNamed(calls, frame, 0x8221F5CC, movie, 7);
    FinishNamed(calls, frame, 0x8221F5D8, movie, 4);
    PlayNamed(calls, frame, 0x8221F5F4, movie, 5);
  });
  return uint32_t(Run(calls, frame, 0x8221F604, ResultsMovie())) == 0 ? 1 : 0;
}

uint64_t RaceResultsDraw(uint32_t screen, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221F7D0 RaceResults::Draw");
  const uint32_t frame = Frame(sp, 112);
  // Intro, Show, Close.
  constexpr uint32_t kHandlers[3] = {0x8221F428, 0x8221F4B0, 0x8221F580};
  const schema::RaceResults results(screen);
  const int32_t transition = int32_t(results.pages_step());
  for (uint32_t k = 0; k < 3; k++) guest::StoreU32(frame + 80 + 4 * k, kHandlers[k]);
  if (transition < 0) {
    return Run(calls, frame, 0x8221F858, results.movie().address);
  }
  const uint32_t step = results.pages_step_started_address();
  uint64_t result;
  switch (transition) {
    case 0: result = RaceResultsIntro(step, frame, calls); break;
    case 1: result = RaceResultsShow(step, frame, calls); break;
    case 2: result = RaceResultsClose(step, frame, calls); break;
    default: {
      // Past the table: the frame's word, as the original's bctrl.
      const uint32_t handler = guest::LoadU32((uint32_t(transition) << 2) + frame + 80);
      const uint64_t regs[8] = {step, 0, 0, 0, 0, Register(kHandlers[0]), Register(kHandlers[1]),
                                uint64_t(frame) + 80};
      result = calls.CallArgs(handler, frame, 0x8221F834, regs);
    }
  }
  PagesFinish(results.pages_page_address(), uint32_t(result));
  return results.pages_page_address();
}

// --- The results at +1032 ---

uint64_t SplitResultsOpen(uint32_t screen, uint64_t outcome, uint64_t mode,
                          uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82220050 SplitResults::Open");
  const uint32_t frame = Frame(sp, 112);
  const schema::SplitResults split(screen);
  split.set_mode(uint32_t(mode));
  split.set_outcome(uint32_t(outcome));
  guest::StoreU32(frame + 80, 8);
  const uint32_t movie =
      guest::LoadU32(ui_movies::PairsFind(ui_movies::kMovieTable, frame + 80));
  split.set_movie(movie);
  calls.Call(kResetPose, frame, 0x82220094, movie);
  PagesInit(split.pages_page_address(), 0);
  PagesGo(split.pages_page_address(), 0, 0, 1);
  Globals::set_split_ticks(0);
  Globals::set_split_exchanged(0);
  return split.pages_page_address();
}

uint64_t SplitResultsMainPage(uint32_t sp, bool native_slots, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221FE70 SplitResults::MainPage");
  const uint32_t frame = Frame(sp, 192);
  const uint32_t first = Entry(kControllerEntries);
  const uint32_t second = Entry(kControllerEntries + 4);
  // Stages::Copy of the current stage into the frame's +80 (its plain C++,
  // or the original through its adapter where it declines).
  const uint32_t stages = Globals::race_data().stages_address();
  if (!vehicle_accessors::StagesCopy(frame + 80, stages, 0xFFFFFFFFu)) {
    calls.Call(kStagesCopy, frame, 0x8221FEC0, frame + 80, stages, kNone);
  }
  if (int32_t(Globals::race_data().kind()) == 6 &&
      int32_t(guest::LoadU32(frame + 80)) != 1) {
    // The network battle: once the other console's result came, 210 ticks
    // counted, then the screen closed (no sound).
    if (int32_t(Globals::split_exchanged()) == 1) {
      const uint32_t ticks = Globals::split_ticks() + 1;
      Globals::set_split_ticks(ticks);
      if (ticks > 210) {
        PagesGo(Globals::split_results().pages_page_address(), 1, 2, 0xFFFFFFFFu);
      }
    }
    return 0;
  }
  if (Pressed(first, 159, frame, 0x8221FF34, native_slots, calls) ||
      Pressed(first, 158, frame, 0x8221FF60, native_slots, calls) ||
      Pressed(second, 159, frame, 0x8221FF8C, native_slots, calls) ||
      Pressed(second, 158, frame, 0x8221FFB8, native_slots, calls)) {
    calls.Call(kMenuSoundPlay, frame, 0x8221FFC8, 0);
    PagesGo(Globals::split_results().pages_page_address(), 1, 2, 0xFFFFFFFFu);
  }
  return 0;
}

uint64_t SplitResultsUpdate(uint64_t r3, uint64_t r4, uint32_t sp,
                            bool native_slots, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221FFF0 SplitResults::Update");
  const uint32_t frame = Frame(sp, 96);
  // MainPage, the shared "returns 1".
  guest::StoreU32(frame + 80, 0x8221FE70);
  guest::StoreU32(frame + 84, 0x825C8390);
  const int32_t page = int32_t(schema::SplitResults(uint32_t(r3)).pages_page());
  switch (page) {
    case -1:
      return 0;
    case 0:
      return SplitResultsMainPage(frame, native_slots, calls);
    case 1:
      return vehicle_accessors::DebugSwitchOn();
    default: {
      const uint32_t handler = guest::LoadU32((uint32_t(page) << 2) + frame + 80);
      const uint64_t regs[8] = {r3, r4, 0, 0, 0, 0, Register(0x8221FE70), uint64_t(frame) + 80};
      return calls.CallArgs(handler, frame, 0x82220034, regs);
    }
  }
}

uint64_t SplitResultsIntro(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221FAC8 SplitResults::Intro");
  const uint32_t frame = Frame(sp, 112);
  First(step, [&] {
    const uint32_t mode = Globals::split_results().mode();
    const uint32_t movie = SplitMovie();
    if (mode > 2) return;
    // The group -1 node 0's +80: 1 for modes 0 and 2, 0 for mode 1.
    const uint32_t node = ui_movies::FindNode(movie, 0xFFFFFFFFu, 0);
    schema::UiMovieNode(node).set_visible(mode == 1 ? 0 : 1);
    uint64_t named;
    if (mode == 2) {
      named = 5;
    } else {
      const uint32_t outcome = Globals::split_results().outcome();
      if (outcome > 2) return;
      static constexpr uint64_t kNamed[2][3] = {{0, 1, 2}, {3, 4, 2}};
      named = kNamed[mode][outcome];
    }
    PlayNamed(calls, frame, 0x8221FBC8, movie, named);
  });
  return uint32_t(Run(calls, frame, 0x8221FBD8, SplitMovie())) == 0 ? 1 : 0;
}

uint64_t SplitResultsShow(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221FBE8 SplitResults::Show");
  const uint32_t frame = Frame(sp, 112);
  First(step, [&] {
    const uint32_t movie = SplitMovie();
    PlayNamed(calls, frame, 0x8221FC38, movie, 6);
    LoopNamed(calls, frame, 0x8221FC44, movie, 6);
    const uint32_t mode = Globals::split_results().mode();
    if (mode > 2) return;
    uint64_t named;
    uint32_t lr;
    if (mode == 2) {
      named = 13;
      lr = 0x8221FC7C;
    } else {
      const uint32_t outcome = Globals::split_results().outcome();
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
  Run(calls, frame, 0x8221FD7C, SplitMovie());
  return 1;
}

uint64_t SplitResultsClose(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221FD98 SplitResults::Close");
  const uint32_t frame = Frame(sp, 112);
  First(step, [&] {
    const uint32_t movie = SplitMovie();
    constexpr uint32_t kFinished[7][2] = {{8, 0x8221FDD8},  {9, 0x8221FDE4},  {10, 0x8221FDF0},
                                          {6, 0x8221FDFC},  {11, 0x8221FE08}, {12, 0x8221FE14},
                                          {13, 0x8221FE20}};
    for (const auto& f : kFinished) FinishNamed(calls, frame, f[1], movie, f[0]);
    PlayNamed(calls, frame, 0x8221FE3C, movie, 7);
  });
  return uint32_t(Run(calls, frame, 0x8221FE4C, SplitMovie())) == 0 ? 1 : 0;
}

uint64_t SplitResultsDraw(uint32_t screen, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822200E0 SplitResults::Draw");
  const uint32_t frame = Frame(sp, 2816);
  // Stages::Copy of the current stage into the frame's +96 (its plain C++,
  // or the original through its adapter where it declines); its r3 the copy.
  const uint32_t stages = Globals::race_data().stages_address();
  if (!vehicle_accessors::StagesCopy(frame + 96, stages, 0xFFFFFFFFu)) {
    calls.Call(kStagesCopy, frame, 0x8222010C, frame + 96, stages, kNone);
  }
  uint64_t r3 = frame + 96;
  if (int32_t(Globals::race_data().kind()) == 6 &&
      int32_t(guest::LoadU32(frame + 96)) != 1 &&
      int32_t(Globals::split_exchanged()) == 0) {
    // The network battle, its result not yet exchanged.
    const uint32_t ticks = Globals::split_ticks();
    if (ticks > 900) {
      // Probably the connection lost: the sign-in watch's state 1, reason 6.
      if (int32_t(Globals::sign_in_watch_state()) == 0) {
        Globals::set_sign_in_watch_state(1);
        Globals::set_sign_in_watch_reason(6);
      }
      return r3;
    }
    // The packet (type 23, the outcome, the id's bytes), in the frame as the
    // original builds it: +176 on the first tick, +1456 after.
    const uint32_t packet = frame + (ticks == 0 ? 176 : 1456);
    guest::StoreU8(packet + 2, 23);
    guest::StoreU8(packet + 3, uint8_t(Globals::split_results().outcome()));
    const uint32_t id = Globals::net_players().kind8_position();
    guest::StoreU8(packet + 8, uint8_t(id));
    guest::StoreU8(packet + 5, uint8_t(id >> 24));
    guest::StoreU16(packet + 6, uint16_t(id >> 8));
    // The sends (sub_8214AB48 on the first tick, sub_8214AA70 after) to each
    // peer in the list or to the one address: Dropped (the user's call,
    // 2026-11-14: sendto on the system link socket B5 no longer makes, the
    // results unread). Each would leave r3 the packet, as their drop
    // overrides do. The peer list's iterator traps (twi) on a list whose end
    // isn't reached in 384-byte steps; logged here instead.
    if (int32_t(Globals::net_peer_mode()) != 0) {
      const uint32_t begin = guest::LoadU32(Globals::net_peers_address() + 4);
      const uint32_t end = guest::LoadU32(Globals::net_peers_address() + 8);
      if (begin > end || (end - begin) % 384 != 0) {
        ITC_LOG_ERROR("SplitResults::Draw: the peer list {:08X} to {:08X} (the original traps)",
                      begin, end);
      }
      if (begin != end) r3 = packet;
    } else {
      r3 = packet;
    }
    Globals::set_split_ticks(Globals::split_ticks() + 1);
    if (Globals::net_answer() == 255) return r3;
    if (int32_t(Globals::net_peer_mode()) == 0) {
      // The other console's answer: the last racer record's result and the
      // screen's outcome.
      const schema::RaceData race = Globals::race_data();
      const uint32_t records = race.results_address();
      const uint32_t count = race.results_count();
      const uint32_t record = vehicle_accessors::RacerRecordsAt(records, count - 1);
      r3 = record;
      switch (Globals::net_answer()) {
        case 0:
          schema::RacerResultsRecord(record).set_finish(1);
          Globals::split_results().set_outcome(1);
          break;
        case 1:
          schema::RacerResultsRecord(record).set_finish(2);
          Globals::split_results().set_outcome(0);
          break;
        default:
          schema::RacerResultsRecord(record).set_outcome(0);
          Globals::split_results().set_outcome(2);
          break;
      }
    }
    Globals::set_split_exchanged(1);
    Globals::set_split_ticks(0);
    Globals::set_net_answer_word(0);
    return r3;
  }
  // Intro, Show, Close.
  constexpr uint32_t kHandlers[3] = {0x8221FAC8, 0x8221FBE8, 0x8221FD98};
  const schema::SplitResults split(screen);
  const int32_t transition = int32_t(split.pages_step());
  for (uint32_t k = 0; k < 3; k++) guest::StoreU32(frame + 80 + 4 * k, kHandlers[k]);
  if (transition < 0) {
    return Run(calls, frame, 0x82220460, split.movie().address);
  }
  const uint32_t step = split.pages_step_started_address();
  uint64_t result;
  switch (transition) {
    case 0: result = SplitResultsIntro(step, frame, calls); break;
    case 1: result = SplitResultsShow(step, frame, calls); break;
    case 2: result = SplitResultsClose(step, frame, calls); break;
    default: {
      // Past the table: the frame's word, as the original's bctrl.
      const uint32_t handler = guest::LoadU32((uint32_t(transition) << 2) + frame + 80);
      const uint64_t regs[8] = {step, 0, 0, 0, 0, Register(kHandlers[0]), Register(kHandlers[1]),
                                uint64_t(frame) + 80};
      result = calls.CallArgs(handler, frame, 0x8222043C, regs);
    }
  }
  PagesFinish(split.pages_page_address(), uint32_t(result));
  return split.pages_page_address();
}

}  // namespace race_screens
