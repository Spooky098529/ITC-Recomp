// The pause menu's transitions and item helpers
// (docs/functions/race-screens.md): see race_screens.h.

#include "race_screens/race_screens.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <bit>

namespace race_screens {

namespace {

constexpr uint64_t kNone = ~uint64_t(0);

uint32_t Get(uint8_t* base, uint32_t global) { return guest::LoadU32(base, global); }

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  guest::StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// The pause menu's movie: the screen's +0.
uint32_t PauseMovie(uint8_t* base) { return Get(base, Get(base, kPauseScreen)); }

// UiMovie::Play(movie, sub_scene, index, chained, 0, 0, 0) through its
// override.
uint64_t Play(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t sub_scene,
              uint64_t index, uint64_t chained = 0) {
  const uint64_t args[8] = {movie, sub_scene, index, chained, 0, 0, 0, 0};
  return calls.CallArgs(kPlay, frame, lr, args);
}

// UiMovie::PlayNamed(movie, named, 0, 0, 0, 0) through its override.
uint64_t PlayNamed(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie,
                   uint64_t named) {
  const uint64_t args[8] = {movie, named, 0, 0, 0, 0, 0, 0};
  return calls.CallArgs(kPlayNamed, frame, lr, args);
}

uint64_t Run(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie) {
  return calls.Call(kRun, frame, lr, movie, 0);
}

uint32_t Selection(uint8_t* base) { return ListSelection(base, Get(base, kPauseList)); }

// A transition's frame: its first call's work when the step word is 0 (then
// the word 1), the movie run once; Run's r3.
template <typename Work>
uint64_t Transition(uint8_t* base, uint32_t step, uint32_t frame, uint32_t lr,
                    const Calls& calls, Work&& work) {
  const uint32_t movie = PauseMovie(base);
  if (guest::LoadU32(base, step) == 0) {
    work(movie);
    guest::StoreU32(base, step, 1);
  }
  return Run(calls, frame, lr, movie);
}

}  // namespace

uint64_t PauseMenuRunTen(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220C5E0 PauseMenu::RunTen");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t movie = PauseMovie(base);
  if (guest::LoadU32(base, step) == 0) {
    guest::StoreU32(base, Get(base, kPauseData) + 152, 0);
    guest::StoreU32(base, step, 1);
  }
  Run(calls, frame, 0x8220C62C, movie);
  const uint32_t data = Get(base, kPauseData);
  guest::StoreU32(base, data + 152, guest::LoadU32(base, data + 152) + 1);
  return int32_t(guest::LoadU32(base, Get(base, kPauseData) + 152)) >= 10 ? 1 : 0;
}

uint64_t PauseMenuBackToMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220C668 PauseMenu::BackToMain");
  const uint32_t frame = Frame(base, sp, 112);
  Transition(base, step, frame, 0x8220C6F0, calls, [&](uint32_t movie) {
    Play(calls, frame, 0x8220C6C0, movie, 7, uint64_t(Selection(base)) + 1, 1);
    calls.Call(kLoop, frame, 0x8220C6DC, movie, 7,
               uint64_t(Selection(base)) + 1);
  });
  return 1;
}

uint64_t PauseMenuCloseMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220C700 PauseMenu::CloseMain");
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t run = Transition(base, step, frame, 0x8220C794, calls, [&](uint32_t movie) {
    calls.Call(kFinish, frame, 0x8220C750, movie, 7,
               uint64_t(Selection(base)) + 1);
    calls.Call(kFinish, frame, 0x8220C760, movie, kNone, 0);
    Play(calls, frame, 0x8220C780, movie, kNone, 2);
  });
  return uint32_t(run) == 0 ? 1 : 0;
}

uint64_t PauseMenuMoveMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220C7B8 PauseMenu::MoveMain");
  const uint32_t frame = Frame(base, sp, 112);
  Transition(base, step, frame, 0x8220C85C, calls, [&](uint32_t movie) {
    // MenuList::GetPreviousSelection (sub_8220BC28): the list's +20.
    const uint32_t previous = guest::LoadU32(base, Get(base, kPauseList) + kListSelectionBefore);
    calls.Call(kFinish, frame, 0x8220C800, movie, 7, uint64_t(previous) + 1);
    Play(calls, frame, 0x8220C82C, movie, 7, uint64_t(Selection(base)) + 1, 1);
    calls.Call(kLoop, frame, 0x8220C848, movie, 7,
               uint64_t(Selection(base)) + 1);
  });
  return 1;
}

uint64_t PauseMenuShowOptionsRow(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220C868 PauseMenu::ShowOptionsRow");
  const uint32_t frame = Frame(base, sp, 112);
  Transition(base, step, frame, 0x8220C924, calls, [&](uint32_t movie) {
    const int32_t selection = int32_t(Selection(base));
    uint64_t row;
    if (selection < 4) {
      row = uint64_t(uint32_t(selection)) + 4;
      Play(calls, frame, 0x8220C8C8, movie, 8, row);
    } else if (int32_t(guest::LoadU32(base, Get(base, kPauseData) + 144)) == 1) {
      row = 2;
      Play(calls, frame, 0x8220C8F0, movie, 8, row);
    } else {
      row = 1;
      Play(calls, frame, 0x8220C900, movie, 8, row);
    }
    calls.Call(kLoop, frame, 0x8220C910, movie, 8, row);
  });
  return 1;
}

uint64_t PauseMenuToggleOptions(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220C930 PauseMenu::ToggleOptions");
  const uint32_t frame = Frame(base, sp, 112);
  Transition(base, step, frame, 0x8220CA0C, calls, [&](uint32_t movie) {
    if (int32_t(Selection(base)) != 4) {
      return;
    }
    const bool on = int32_t(guest::LoadU32(base, Get(base, kPauseData) + 144)) == 1;
    const uint64_t from = on ? 2 : 1, to = on ? 1 : 2;
    calls.Call(kFinish, frame, on ? 0x8220C998 : 0x8220C9C8, movie, 8, from);
    Play(calls, frame, on ? 0x8220C9B8 : 0x8220C9E8, movie, 8, to);
    calls.Call(kLoop, frame, 0x8220C9F8, movie, 8, to);
  });
  return 1;
}

uint64_t PauseMenuShowVolumeRow(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220CA28 PauseMenu::ShowVolumeRow");
  const uint32_t frame = Frame(base, sp, 112);
  Transition(base, step, frame, 0x8220CA94, calls, [&](uint32_t movie) {
    Play(calls, frame, 0x8220CA78, movie, 9, 1);
    calls.Call(kLoopLast, frame, 0x8220CA80, movie);
  });
  return 1;
}

uint64_t PauseMenuShowAdjustRow(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220CAB0 PauseMenu::ShowAdjustRow");
  const uint32_t frame = Frame(base, sp, 112);
  Transition(base, step, frame, 0x8220CB1C, calls, [&](uint32_t movie) {
    PlayNamed(calls, frame, 0x8220CAFC, movie, 192);
    calls.Call(kLoopNamed, frame, 0x8220CB08, movie, 192);
  });
  return 1;
}

uint64_t PauseMenuConfirmPage(uint8_t* base, uint64_t r4, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220C578 PauseMenu::ConfirmPage");
  const uint32_t frame = Frame(base, sp, 96);
  const int32_t answer = int32_t(uint32_t(RaceDialogUpdate(base, r4, frame, calls)));
  if (answer == 1) {
    calls.Call(kPagesGo, frame, 0x8220C5BC, Get(base, kPausePages), 5, 5, kNone);
  } else if (answer == 2) {
    calls.Call(kPagesGo, frame, 0x8220C5BC, Get(base, kPausePages), 0, kNone, kNone);
  }
  return 0;
}

uint64_t PauseMenuShowItems(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220CB38 PauseMenu::ShowItems");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = PauseMovie(base);
  const uint32_t node = ui_movies::FindNode(base, movie, 7, 0);
  uint64_t r3 = node;
  for (int32_t i = 0; i < int32_t(guest::LoadU32(base, node + 104)); i++) {
    const uint32_t kind = guest::LoadU32(base, Get(base, kPauseData) + 132);
    const uint32_t item = guest::LoadU32(base, kPauseItems + (kind * 6 + uint32_t(i)) * 4);
    const uint64_t child = uint64_t(guest::LoadU32(base, node + 100)) + uint64_t(i);
    if (int32_t(item) != -1 && i < 6) {
      r3 = calls.Call(kSetText, frame, 0x8220CBC0, movie, 7, child, uint64_t(item) + 1989);
    } else {
      const uint32_t hidden = ui_movies::FindNode(base, movie, 7, uint32_t(child));
      guest::StoreU32(base, hidden + 80, 0);
      r3 = hidden;
    }
  }
  return r3;
}

uint32_t PauseMenuSetArrowA(uint8_t* base, uint64_t value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220CBF8 PauseMenu::SetArrowA");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t node = ui_movies::FindNode(base, PauseMovie(base), 8, 0);
  const int64_t word = int64_t(int32_t(uint32_t(value)));
  guest::StoreU32(base, frame + 80, uint32_t(uint64_t(word) >> 32));
  guest::StoreU32(base, frame + 84, uint32_t(word));
  const uint32_t child = ui_movies::Child(base, node, 0);
  guest::StoreU32(base, child + 16, std::bit_cast<uint32_t>(float(double(word))));
  return child;
}

uint32_t PauseMenuShowOptions(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220CC60 PauseMenu::ShowOptions");
  const uint32_t frame = Frame(base, sp, 192);
  const uint32_t movie = PauseMovie(base);
  const uint32_t data = Get(base, kPauseData);
  for (const uint32_t field : {36u, 40u, 44u, 48u}) {
    guest::StoreU32(base, frame + 80 + (field - 36), guest::LoadU32(base, data + field));
  }
  guest::StoreU32(base, frame + 96, guest::LoadU32(base, data + 144));
  const uint32_t group = ui_movies::FindNode(base, movie, 8, 2);
  uint32_t r3 = group;
  for (int32_t i = 0; i < int32_t(guest::LoadU32(base, group + 104)); i++) {
    const uint32_t part = ui_movies::FindNode(base, movie, 8, guest::LoadU32(base, group + 100) + i);
    const uint32_t first = ui_movies::FindNode(base, movie, 8, guest::LoadU32(base, part + 100));
    const uint32_t first0 = ui_movies::Child(base, first, 0);
    const uint32_t first2 = ui_movies::Child(base, first, 2);
    const uint32_t second =
        ui_movies::FindNode(base, movie, 8, guest::LoadU32(base, part + 100) + 1);
    const uint32_t second0 = ui_movies::Child(base, second, 0);
    const uint32_t second2 = ui_movies::Child(base, second, 2);
    const bool on = int32_t(guest::LoadU32(base, frame + 80 + 4 * uint32_t(i))) == 1;
    guest::StoreU32(base, first0 + 108, on ? 0 : 1);
    guest::StoreU32(base, first2 + 80, on ? 1 : 0);
    guest::StoreU32(base, second0 + 108, on ? 1 : 0);
    guest::StoreU32(base, second2 + 80, on ? 0 : 1);
    r3 = second2;
  }
  return r3;
}

}  // namespace race_screens
