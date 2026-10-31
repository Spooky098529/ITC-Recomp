// The menu after the race's pages and transitions
// (docs/functions/race-screens.md): see race_screens.h.

#include "race_screens/race_screens.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace race_screens {

namespace {

constexpr uint64_t kNone = ~uint64_t(0);

uint32_t Get(uint8_t* base, uint32_t global) { return guest::LoadU32(base, global); }

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  guest::StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// The menu's movie: the screen's +0, read again at each use.
uint32_t Movie(uint8_t* base) { return Get(base, Get(base, kAfterScreen)); }

uint32_t Selection(uint8_t* base) { return ListSelection(base, Get(base, kAfterList)); }

uint64_t Play(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t sub_scene,
              uint64_t index, uint64_t chained = 0) {
  const uint64_t args[8] = {movie, sub_scene, index, chained, 0, 0, 0, 0};
  return calls.CallArgs(kPlay, frame, lr, args);
}

uint64_t Run(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie) {
  return calls.Call(kRun, frame, lr, movie, 0);
}

// The item of `kind` at `i` (the table's words; any index read as the
// original reads it).
uint32_t Item(uint8_t* base, uint32_t kind, uint32_t i) {
  return guest::LoadU32(base, kAfterItems + (((kind << 2) + i) << 2));
}

}  // namespace

uint64_t AfterRaceMenuConfirmPage(uint8_t* base, uint64_t r4, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221A6C8 AfterRaceMenu::ConfirmPage");
  const uint32_t frame = Frame(base, sp, 96);
  const int32_t answer = int32_t(uint32_t(RaceDialogUpdate(base, r4, frame, calls)));
  if (answer == 1) {
    calls.Call(kPagesGo, frame, 0x8221A70C, Get(base, kAfterPages), 7, 4, kNone);
  } else if (answer == 2) {
    calls.Call(kPagesGo, frame, 0x8221A70C, Get(base, kAfterPages), 1, kNone, kNone);
  }
  return 0;
}

uint64_t AfterRaceMenuIntro(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221A730 AfterRaceMenu::Intro");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t movie = Movie(base);
  if (guest::LoadU32(base, step) == 0) {
    guest::StoreU32(base, step, 1);
    Play(calls, frame, 0x8221A780, Movie(base), kNone, 0);
  }
  return uint32_t(Run(calls, frame, 0x8221A78C, movie)) == 0 ? 1 : 0;
}

uint64_t AfterRaceMenuShow(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221A7A8 AfterRaceMenu::Show");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = Movie(base);
  if (guest::LoadU32(base, step) == 0) {
    guest::StoreU32(base, step, 1);
    Play(calls, frame, 0x8221A7FC, movie, kNone, 2);
    calls.Call(kLoop, frame, 0x8221A80C, movie, kNone, 2);
    Play(calls, frame, 0x8221A83C, movie, 4, uint64_t(Selection(base)) + 1, 1);
    calls.Call(kLoop, frame, 0x8221A858, movie, 4, uint64_t(Selection(base)) + 1);
    const uint64_t named[8] = {movie, 40, 0, 0, 0, 0, 0, 0};
    calls.CallArgs(kPlayNamed, frame, 0x8221A874, named);
    calls.Call(kLoopNamed, frame, 0x8221A880, movie, 40);
  }
  Run(calls, frame, 0x8221A88C, movie);
  return 1;
}

uint64_t AfterRaceMenuCloseMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221A950 AfterRaceMenu::CloseMain");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = Movie(base);
  if (guest::LoadU32(base, step) == 0) {
    guest::StoreU32(base, step, 1);
    const uint64_t item = uint64_t(Selection(base)) + 1;
    calls.Call(kFinish, frame, 0x8221A9A8, Movie(base), 4, item);
    calls.Call(kFinish, frame, 0x8221A9BC, Movie(base), kNone, 2);
    calls.Call(kFinishNamed, frame, 0x8221A9CC, Movie(base), 40);
    Play(calls, frame, 0x8221A9F0, Movie(base), kNone, 1);
  }
  return uint32_t(Run(calls, frame, 0x8221A9FC, movie)) == 0 ? 1 : 0;
}

uint64_t AfterRaceMenuMove(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221AA20 AfterRaceMenu::Move");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = Movie(base);
  if (guest::LoadU32(base, step) == 0) {
    guest::StoreU32(base, step, 1);
    // MenuList::GetPreviousSelection (sub_8220BC28): the list's +20.
    const uint32_t previous = guest::LoadU32(base, Get(base, kAfterList) + kListSelectionBefore);
    calls.Call(kFinish, frame, 0x8221AA70, Movie(base), 4, uint64_t(previous) + 1);
    const uint64_t item = uint64_t(Selection(base)) + 1;
    Play(calls, frame, 0x8221AAA0, Movie(base), 4, item, 1);
    calls.Call(kLoop, frame, 0x8221AAC0, Movie(base), 4, uint64_t(Selection(base)) + 1);
  }
  Run(calls, frame, 0x8221AACC, movie);
  return 1;
}

uint64_t AfterRaceMenuShowItems(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221AC00 AfterRaceMenu::ShowItems");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = Movie(base);
  const uint32_t node = ui_movies::FindNode(base, movie, 4, 0);
  uint64_t r3 = node;
  for (int32_t i = 0; i < int32_t(guest::LoadU32(base, node + 104)); i++) {
    const uint32_t item =
        Item(base, guest::LoadU32(base, Get(base, kAfterData)), uint32_t(i));
    const uint64_t child = uint64_t(guest::LoadU32(base, node + 100)) + uint64_t(i);
    if (int32_t(item) != -1 && i < 4) {
      r3 = calls.Call(kSetText, frame, 0x8221AC88, movie, 4, child, uint64_t(item) + 1960);
    } else {
      const uint32_t hidden = ui_movies::FindNode(base, movie, 4, uint32_t(child));
      guest::StoreU32(base, hidden + 80, 0);
      r3 = hidden;
    }
  }
  return r3;
}

uint64_t AfterRaceMenuSetUpItems(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221B5C8 AfterRaceMenu::SetUpItems");
  const uint32_t frame = Frame(base, sp, 144);
  guest::StoreU32(base, frame + 84, 159);
  const uint32_t count = guest::LoadU32(base, Get(base, kAfterData) + 4);
  guest::StoreU32(base, frame + 92, 158);
  const uint64_t init[8] = {Get(base, kAfterList), count, count, 1, 154, 153, 155, 156};
  calls.CallArgs(kListInit, frame, 0x8221B614, init);
  ListSetSelection(base, Get(base, kAfterList), guest::LoadU32(base, Get(base, kAfterData) + 16));
  const uint32_t data = Get(base, kAfterData);
  if (guest::LoadU32(base, data + 8) != 0) {
    for (int32_t i = 0; i < int32_t(guest::LoadU32(base, Get(base, kAfterData) + 4)); i++) {
      const int32_t item = int32_t(Item(base, guest::LoadU32(base, Get(base, kAfterData)),
                                        uint32_t(i)));
      if (item == 1 || item == 2) {
        ListSetEnabled(base, Get(base, kAfterList), uint32_t(i), 0);
      }
    }
  }
  return AfterRaceMenuShowItems(base, frame, calls);
}

uint32_t AfterRaceMenuColorItems(uint8_t* base, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8221B768 AfterRaceMenu::ColorItems");
  Frame(base, sp, 128);
  const uint32_t node = ui_movies::FindNode(base, guest::LoadU32(base, screen), 4, 0);
  uint32_t r3 = node;
  for (int32_t i = 0; i < int32_t(guest::LoadU32(base, node + 104)); i++) {
    const bool enabled = ListIsEnabled(base, Get(base, kAfterList), uint32_t(i)) != 0;
    const uint32_t color[3] = {enabled ? 205u : 255u, enabled ? 235u : 255u,
                               enabled ? 67u : 255u};
    for (uint32_t k = 0; k < 3; k++) {
      r3 = ui_movies::FindNode(base, guest::LoadU32(base, screen), 4,
                               guest::LoadU32(base, node + 100) + uint32_t(i));
      guest::StoreU32(base, r3 + 56 + 4 * k, color[k]);
    }
  }
  return r3;
}

uint64_t AfterRaceMenuStart(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221B860 AfterRaceMenu::Start");
  const uint32_t frame = Frame(base, sp, 96);
  AfterRaceMenuColorItems(base, Get(base, kAfterScreen), frame);
  calls.Call(kPagesGo, frame, 0x8221B890, Get(base, kAfterPages), 1, 1, kNone);
  return 0;
}

uint64_t AfterRaceMenuOpen(uint8_t* base, uint32_t screen, uint64_t kind, uint64_t flag,
                           uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221BA68 AfterRaceMenu::Open");
  const uint32_t frame = Frame(base, sp, 128);
  guest::StoreU32(base, frame + 80, 9);
  guest::StoreU32(base, Get(base, kAfterData) + 0, uint32_t(kind));
  const uint32_t movie =
      guest::LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
  guest::StoreU32(base, screen, movie);
  calls.Call(kResetPose, frame, 0x8221BAB0, movie);
  guest::StoreU32(base, Get(base, kAfterData) + 4, 4);
  uint32_t data = Get(base, kAfterData);
  uint32_t at = kAfterItems + (guest::LoadU32(base, data + 0) << 4);
  for (uint32_t n = 0; n < 4; n++, at += 4) {
    if (int32_t(guest::LoadU32(base, at)) == -1) {
      guest::StoreU32(base, data + 4, n);
      data = Get(base, kAfterData);
      break;
    }
  }
  guest::StoreU32(base, data + 8, uint32_t(flag));
  guest::StoreU32(base, Get(base, kAfterData) + 16, 0);
  AfterRaceMenuSetUpItems(base, frame, calls);
  PagesInit(base, Get(base, kAfterPages), 1);
  const uint64_t r3 = calls.Call(kPagesGo, frame, 0x8221BB38, Get(base, kAfterPages), 0, 0, kNone);
  guest::StoreU32(base, Get(base, kAfterData) + 20, 0);
  return r3;
}

}  // namespace race_screens
