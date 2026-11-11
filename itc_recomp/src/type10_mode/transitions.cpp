// The replay theater's transitions and the replay list's helpers
// (docs/functions/type10-mode.md): see type10_mode.h. Each transition finds
// movie 45 (its id at its frame's +80), sets itself up once (its started
// word, the page object's +24, 0), then runs the movie. The varargs UI movie
// setters get 0 for the arguments their callers leave unset (the user's
// call of 2026-10-30).

#include "base/log.h"
#include "race_screens/race_screens.h"
#include "type10_mode/internal.h"
#include "ui_movies/ui_movies.h"

namespace type10_mode {

namespace {

uint64_t Play(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t id) {
  return calls.Call(kPlayNamed, frame, lr, movie, id);
}
uint64_t Loop(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t id) {
  return calls.Call(kLoopNamed, frame, lr, movie, id);
}
uint64_t Finish(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t id) {
  return calls.Call(kFinishNamed, frame, lr, movie, id);
}
uint64_t RunMovie(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie) {
  return calls.Call(kRun, frame, lr, movie, 0);
}
// 1 when Run answered 0.
uint64_t Done(uint64_t run) { return uint32_t(run) == 0 ? 1 : 0; }

// The list's selection's row (its animation: selection + 5).
uint64_t Row(uint8_t* base) {
  return uint64_t(race_screens::ListSelection(base, Load(base, kList))) + 5;
}

// The replays' 36-byte records (the save layer's header +12) and their count
// (+20).
uint32_t Records(uint8_t* base) { return Load(base, Load(base, kSaveBuffers) + 12); }

// The cursor node (sub-scene 3, child 2, its child 0)'s +16 = n × 40 as a
// float, through the original's frame doubleword at `slot` (the integer
// stored there for the conversion).
void StoreCursor(uint8_t* base, uint32_t slot, uint32_t movie, uint32_t n) {
  const uint32_t list_node = ui_movies::FindNode(base, movie, 3, 2);
  const int32_t value = int32_t(n * 40u);
  const uint64_t wide = uint64_t(int64_t(value));
  Store(base, slot, uint32_t(wide >> 32));
  Store(base, slot + 4, uint32_t(wide));
  const uint32_t cursor = ui_movies::Child(base, list_node, 0);
  guest::StoreFloat(base, cursor + 16, float(double(int64_t(value))));
}

}  // namespace

uint64_t Intro(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82289AC0 ReplayTheater::Intro");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80);
  if (Load(base, started) == 0) {
    Play(calls, frame, 0x82289B1C, movie, 52);
    Store(base, started, 1);
  }
  if (uint32_t(RunMovie(calls, frame, 0x82289B30, movie)) == 0) {
    Play(calls, frame, 0x82289B54, movie, 49);
    Loop(calls, frame, 0x82289B60, movie, 49);
    Play(calls, frame, 0x82289B7C, movie, 43);
    Loop(calls, frame, 0x82289B88, movie, 43);
    Play(calls, frame, 0x82289BA4, movie, 0);
    Loop(calls, frame, 0x82289BB0, movie, 0);
  }
  return 1;
}

uint64_t FadeOut(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82289BD0 ReplayTheater::FadeOut");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80);
  if (Load(base, started) == 0) {
    Finish(calls, frame, 0x82289C2C, movie, Row(base));
    Finish(calls, frame, 0x82289C38, movie, 49);
    Finish(calls, frame, 0x82289C44, movie, 43);
    Finish(calls, frame, 0x82289C50, movie, 0);
    calls.Call(kFadeOut, frame, 0x82289C6C, Load(base, kFadesOut), 0, 0, 0);
    Store(base, started, 1);
  }
  RunMovie(calls, frame, 0x82289C80, movie);
  return (Load(base, kFadeCount0) | Load(base, kFadeCount1)) == 0 ? 1 : 0;
}

uint64_t FadeInList(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82289CC0 ReplayTheater::FadeInList");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80);
  if (Load(base, started) == 0) {
    Play(calls, frame, 0x82289D1C, movie, 50);
    calls.Call(kFadeIn, frame, 0x82289D38, Load(base, kFadesIn), 0, 0, 0);
    Store(base, started, 1);
  }
  return Done(RunMovie(calls, frame, 0x82289D4C, movie));
}

uint64_t ShowCursor(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82289D70 ReplayTheater::ShowCursor");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, frame + 80);
  if (Load(base, started) == 0) {
    const uint64_t row = Row(base);
    Play(calls, frame, 0x82289DD8, movie, row);
    Loop(calls, frame, 0x82289DE4, movie, row);
    Store(base, started, 1);
  }
  RunMovie(calls, frame, 0x82289DF8, movie);
  return 1;
}

uint64_t HideCursor(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82289E08 ReplayTheater::HideCursor");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80);
  if (Load(base, started) == 0) {
    Finish(calls, frame, 0x82289E64, movie, Row(base));
    Play(calls, frame, 0x82289E80, movie, 51);
    Store(base, started, 1);
  }
  return Done(RunMovie(calls, frame, 0x82289E94, movie));
}

uint64_t PickReplay(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82289EB8 ReplayTheater::PickReplay");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80);
  if (Load(base, started) == 0) {
    Finish(calls, frame, 0x82289F14, movie, Row(base));
    Store(base, started, 1);
  }
  RunMovie(calls, frame, 0x82289F28, movie);
  return 1;
}

uint64_t MoveCursor(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8228A658 ReplayTheater::MoveCursor");
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t movie = FindMovie(base, frame + 80);
  if (Load(base, started) == 0) {
    const uint32_t selection = race_screens::ListSelection(base, Load(base, kList));
    const uint32_t before = Load(base, Load(base, kList) + 20);  // MenuList's previous selection
    const uint32_t list = Load(base, kList);
    const uint32_t scroll = Load(base, list + 4);
    const uint32_t scrolled = scroll - Load(base, list + 8);
    const uint32_t wrapped = Load(base, list + 32);
    const uint32_t records = Records(base);
    Finish(calls, frame, 0x8228A6DC, movie, uint64_t(before) + 5);
    const uint64_t row = uint64_t(selection) + 5;
    Play(calls, frame, 0x8228A6FC, movie, row);
    Loop(calls, frame, 0x8228A708, movie, row);
    ShowRecord(base, (scroll + selection) * 36 + records, frame, calls);
    Finish(calls, frame, 0x8228A724, movie, 15);
    Finish(calls, frame, 0x8228A730, movie, 16);
    if (wrapped != 0) {
      StoreCursor(base, frame + 80, movie, selection);
    } else {
      const int32_t direction = int32_t(Load(base, Load(base, kList) + 24));
      const uint32_t list_node = ui_movies::FindNode(base, movie, 3, 2);
      if (direction < 0) {
        calls.Call(kSetNodeText, frame, 0x8228A7A8, movie, ui_movies::Child(base, list_node, 15),
                   471, Load(base, scroll * 36 + records + 28));
        Play(calls, frame, 0x8228A7F4, movie, 15);
      } else if (direction > 0) {
        calls.Call(kSetNodeText, frame, 0x8228A7D8, movie, ui_movies::Child(base, list_node, 14),
                   471, Load(base, scroll * 36 + records + 352));
        Play(calls, frame, 0x8228A7F4, movie, 16);
      }
    }
    if (scrolled != 0) {
      Finish(calls, frame, 0x8228A808, movie, 28);
      Finish(calls, frame, 0x8228A814, movie, 27);
      ShowListPage(base, frame, calls);
      Play(calls, frame, 0x8228A840, movie, int32_t(scrolled) > 0 ? 28 : 27);
    }
    Store(base, started, 1);
  }
  RunMovie(calls, frame, 0x8228A854, movie);
  return 1;
}

uint64_t BuildList(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8228A560 ReplayTheater::BuildList");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, frame + 96);
  const uint32_t header = Load(base, kSaveBuffers);
  const uint32_t count = Load(base, header + 20);
  const uint32_t records = Load(base, header + 12);
  const uint32_t shown = int32_t(count) < 9 ? count : 9;
  // The accept and back actions, the stack arguments.
  Store(base, frame + 84, 159);
  Store(base, frame + 92, 158);
  calls.CallArgs(kListInit, frame, 0x8228A5DC,
                 {Load(base, kList), count, shown, 1, 154, 153, 155, 156});
  const uint32_t selection = race_screens::ListSelection(base, Load(base, kList));
  StoreCursor(base, frame + 96, movie, selection);
  ShowListPage(base, frame, calls);
  ShowRecord(base, selection * 36 + records, frame, calls);
  static constexpr uint32_t kPrompts[3] = {0, 1, kNone};
  return SetPrompts(base, kPrompts, kPrompts, frame, calls);
}

uint64_t ShowListPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82289F48 ReplayTheater::ShowListPage");
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t header = Load(base, kSaveBuffers);
  const uint32_t records = Load(base, header + 12);
  const uint32_t movie = FindMovie(base, frame + 80);
  const int32_t count = int32_t(Load(base, header + 20));
  const uint32_t scroll = Load(base, Load(base, kList) + 4);
  const int32_t shown = count < 9 ? count : 9;
  const uint32_t list_node = ui_movies::FindNode(base, movie, 3, 2);
  for (uint32_t i = 0; i < 9; i++) {
    calls.Call(kClearNodeText, frame, 0x82289FD0, movie, ui_movies::Child(base, list_node, i + 5));
  }
  for (int32_t i = 0; i < shown; i++) {
    calls.Call(kSetNodeText, frame, 0x8228A014, movie,
               ui_movies::Child(base, list_node, uint32_t(i) + 5), 471,
               Load(base, scroll * 36 + records + 36 * uint32_t(i) + 28));
  }
  // The scroll arrows (children 7 and 8 of sub-scene 3).
  uint32_t up = 0, down = 0;
  if (count > 9) {
    if (scroll == 0) {
      down = 1;
    } else if (scroll == uint32_t(count - 9)) {
      up = 1;
    } else {
      up = 1;
      down = 1;
    }
  }
  Store(base, ui_movies::FindNode(base, movie, 3, 7) + 80, up);
  const uint32_t last = ui_movies::FindNode(base, movie, 3, 8);
  Store(base, last + 80, down);
  return last;
}

uint64_t ShowRecord(uint8_t* base, uint32_t record, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8228A0D8 ReplayTheater::ShowRecord");
  const uint32_t frame = Frame(base, sp, 192);
  const uint32_t movie = FindMovie(base, frame + 96);
  if (record == 0 || Load(base, record + 28) == 0) {
    static constexpr uint32_t kTexts[6] = {9, 10, 13, 19, 20, 21};
    static constexpr uint32_t kLr[6] = {0x8228A330, 0x8228A340, 0x8228A350,
                                        0x8228A360, 0x8228A370, 0x8228A380};
    for (uint32_t k = 0; k < 6; k++) {
      calls.Call(kClearText, frame, kLr[k], movie, 3, kTexts[k]);
    }
    Store(base, ui_movies::FindNode(base, movie, 3, 17) + 108, 0);
    return calls.Call(kClearText, frame, 0x8228A3AC, movie, 3, 12);
  }
  uint32_t time = Load(base, record + 16);
  if ((time & 0x7FFF0000u) > 0x3B0000u) {
    time = 0x3BEFE7u | (time & 0x80000000u);  // 59:59.999
  }
  auto half = [&](uint32_t offset) { return uint64_t(guest::LoadU16(base, record + offset)); };
  // The date: its fifth argument on the stack (+84).
  Store(base, frame + 84, uint32_t(half(10)));
  calls.CallArgs(kSetText, frame, 0x8228A164,
                 {movie, 3, 9, 530, half(0), half(2), half(6), half(8)});
  calls.Call(kSetText, frame, 0x8228A17C, movie, 3, 10, uint64_t(Load(base, record + 20)) + 473);
  calls.CallArgs(kSetText, frame, 0x8228A19C,
                 {movie, 3, 13, 528, (time >> 16) & 0x7FFF, (time >> 10) & 0x3F, time & 0x3FF, 0});
  calls.Call(kSetText, frame, 0x8228A1B4, movie, 3, 19, uint64_t(Load(base, record + 24)) + 1646);
  calls.Call(kSetText, frame, 0x8228A1CC, movie, 3, 20, uint64_t(Load(base, record + 24)) + 1790);
  calls.Call(kSetText, frame, 0x8228A1E4, movie, 3, 21, uint64_t(Load(base, record + 24)) + 1718);
  const uint32_t kind = Load(base, record + 20);
  if (kind < 4) {
    uint64_t text;
    if (kind == 1 || kind == 3) {
      static constexpr uint32_t kModes[8] = {395, 396, 397, 398, 401, 402, 404, 403};
      for (uint32_t k = 0; k < 8; k++) {
        Store(base, frame + 112 + 4 * k, kModes[k]);
      }
      Store(base, ui_movies::FindNode(base, movie, 3, 17) + 108, 1);
      text = Load(base, frame + 112 + (Load(base, record + 32) << 2));
    } else if (kind == 2) {
      Store(base, ui_movies::FindNode(base, movie, 3, 17) + 108, 0);
      text = 477;
    } else {
      Store(base, ui_movies::FindNode(base, movie, 3, 17) + 108, 0);
      text = uint64_t(Load(base, record + 32)) + 4650;
    }
    calls.Call(kSetText, frame, 0x8228A2C4, movie, 3, 12, text);
  }
  // The record's +28 in two digits (children 5 and 6).
  const uint32_t tens = ui_movies::FindNode(base, movie, 3, 5);
  const uint32_t units = ui_movies::FindNode(base, movie, 3, 6);
  const int32_t number = int32_t(Load(base, record + 28));
  Store(base, tens + 108, number < 10 ? 0 : uint32_t(number / 10));
  Store(base, units + 108, uint32_t(number % 10));
  return units;
}

}  // namespace type10_mode
