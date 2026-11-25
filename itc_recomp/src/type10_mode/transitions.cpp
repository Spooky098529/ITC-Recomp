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
uint64_t Row() {
  return uint64_t(race_screens::ListSelection(List().address)) + 5;
}

// The replays' 36-byte records (the save buffers' +12).
uint32_t Records() { return SaveBuffers().records().address; }

// The cursor node (sub-scene 3, child 2, its child 0)'s +16 = n × 40 as a
// float, through the original's frame doubleword at `slot` (the integer
// stored there for the conversion).
void StoreCursor(uint32_t slot, uint32_t movie, uint32_t n) {
  const uint32_t list_node = ui_movies::FindNode(movie, 3, 2);
  const int32_t value = int32_t(n * 40u);
  const uint64_t wide = uint64_t(int64_t(value));
  Store(slot, uint32_t(wide >> 32));
  Store(slot + 4, uint32_t(wide));
  const Node cursor(ui_movies::Child(list_node, 0));
  cursor.set_y(float(double(int64_t(value))));
}

}  // namespace

uint64_t Intro(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82289AC0 ReplayTheater::Intro");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = FindMovie(frame + 80);
  if (Load(started) == 0) {
    Play(calls, frame, 0x82289B1C, movie, 52);
    Store(started, 1);
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

uint64_t FadeOut(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82289BD0 ReplayTheater::FadeOut");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = FindMovie(frame + 80);
  if (Load(started) == 0) {
    Finish(calls, frame, 0x82289C2C, movie, Row());
    Finish(calls, frame, 0x82289C38, movie, 49);
    Finish(calls, frame, 0x82289C44, movie, 43);
    Finish(calls, frame, 0x82289C50, movie, 0);
    calls.Call(kFadeOut, frame, 0x82289C6C, G::fades_out(), 0, 0, 0);
    Store(started, 1);
  }
  RunMovie(calls, frame, 0x82289C80, movie);
  return (G::fade_count0() | G::fade_count1()) == 0 ? 1 : 0;
}

uint64_t FadeInList(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82289CC0 ReplayTheater::FadeInList");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = FindMovie(frame + 80);
  if (Load(started) == 0) {
    Play(calls, frame, 0x82289D1C, movie, 50);
    calls.Call(kFadeIn, frame, 0x82289D38, G::fades_in(), 0, 0, 0);
    Store(started, 1);
  }
  return Done(RunMovie(calls, frame, 0x82289D4C, movie));
}

uint64_t ShowCursor(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82289D70 ReplayTheater::ShowCursor");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = FindMovie(frame + 80);
  if (Load(started) == 0) {
    const uint64_t row = Row();
    Play(calls, frame, 0x82289DD8, movie, row);
    Loop(calls, frame, 0x82289DE4, movie, row);
    Store(started, 1);
  }
  RunMovie(calls, frame, 0x82289DF8, movie);
  return 1;
}

uint64_t HideCursor(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82289E08 ReplayTheater::HideCursor");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = FindMovie(frame + 80);
  if (Load(started) == 0) {
    Finish(calls, frame, 0x82289E64, movie, Row());
    Play(calls, frame, 0x82289E80, movie, 51);
    Store(started, 1);
  }
  return Done(RunMovie(calls, frame, 0x82289E94, movie));
}

uint64_t PickReplay(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82289EB8 ReplayTheater::PickReplay");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = FindMovie(frame + 80);
  if (Load(started) == 0) {
    Finish(calls, frame, 0x82289F14, movie, Row());
    Store(started, 1);
  }
  RunMovie(calls, frame, 0x82289F28, movie);
  return 1;
}

uint64_t MoveCursor(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8228A658 ReplayTheater::MoveCursor");
  const uint32_t frame = Frame(sp, 176);
  const uint32_t movie = FindMovie(frame + 80);
  if (Load(started) == 0) {
    const uint32_t selection = race_screens::ListSelection(List().address);
    const uint32_t before = List().selection_before();
    const schema::MenuList list = List();
    const uint32_t scroll = list.scroll();
    const uint32_t scrolled = scroll - list.scroll_before();
    const uint32_t wrapped = list.wrapped();
    const uint32_t records = Records();
    Finish(calls, frame, 0x8228A6DC, movie, uint64_t(before) + 5);
    const uint64_t row = uint64_t(selection) + 5;
    Play(calls, frame, 0x8228A6FC, movie, row);
    Loop(calls, frame, 0x8228A708, movie, row);
    ShowRecord((scroll + selection) * 36 + records, frame, calls);
    Finish(calls, frame, 0x8228A724, movie, 15);
    Finish(calls, frame, 0x8228A730, movie, 16);
    if (wrapped != 0) {
      StoreCursor(frame + 80, movie, selection);
    } else {
      const int32_t direction = int32_t(List().move());
      const uint32_t list_node = ui_movies::FindNode(movie, 3, 2);
      if (direction < 0) {
        calls.Call(kSetNodeText, frame, 0x8228A7A8, movie, ui_movies::Child(list_node, 15),
                   471, Record(records, scroll).number());
        Play(calls, frame, 0x8228A7F4, movie, 15);
      } else if (direction > 0) {
        calls.Call(kSetNodeText, frame, 0x8228A7D8, movie, ui_movies::Child(list_node, 14),
                   471, Record(records, scroll + 9).number());
        Play(calls, frame, 0x8228A7F4, movie, 16);
      }
    }
    if (scrolled != 0) {
      Finish(calls, frame, 0x8228A808, movie, 28);
      Finish(calls, frame, 0x8228A814, movie, 27);
      ShowListPage(frame, calls);
      Play(calls, frame, 0x8228A840, movie, int32_t(scrolled) > 0 ? 28 : 27);
    }
    Store(started, 1);
  }
  RunMovie(calls, frame, 0x8228A854, movie);
  return 1;
}

uint64_t BuildList(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8228A560 ReplayTheater::BuildList");
  const uint32_t frame = Frame(sp, 144);
  const uint32_t movie = FindMovie(frame + 96);
  const schema::SaveBuffers header = SaveBuffers();
  const uint32_t count = header.count();
  const uint32_t records = header.records().address;
  const uint32_t shown = int32_t(count) < 9 ? count : 9;
  // The accept and back actions, the stack arguments.
  Store(frame + 84, 159);
  Store(frame + 92, 158);
  calls.CallArgs(kListInit, frame, 0x8228A5DC,
                 {List().address, count, shown, 1, 154, 153, 155, 156});
  const uint32_t selection = race_screens::ListSelection(List().address);
  StoreCursor(frame + 96, movie, selection);
  ShowListPage(frame, calls);
  ShowRecord(selection * 36 + records, frame, calls);
  static constexpr uint32_t kPrompts[3] = {0, 1, kNone};
  return SetPrompts(kPrompts, kPrompts, frame, calls);
}

uint64_t ShowListPage(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82289F48 ReplayTheater::ShowListPage");
  const uint32_t frame = Frame(sp, 176);
  const schema::SaveBuffers header = SaveBuffers();
  const uint32_t records = header.records().address;
  const uint32_t movie = FindMovie(frame + 80);
  const int32_t count = int32_t(header.count());
  const uint32_t scroll = List().scroll();
  const int32_t shown = count < 9 ? count : 9;
  const uint32_t list_node = ui_movies::FindNode(movie, 3, 2);
  for (uint32_t i = 0; i < 9; i++) {
    calls.Call(kClearNodeText, frame, 0x82289FD0, movie, ui_movies::Child(list_node, i + 5));
  }
  for (int32_t i = 0; i < shown; i++) {
    calls.Call(kSetNodeText, frame, 0x8228A014, movie,
               ui_movies::Child(list_node, uint32_t(i) + 5), 471,
               Record(records, scroll + uint32_t(i)).number());
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
  Node(ui_movies::FindNode(movie, 3, 7)).set_visible(up);
  const Node last(ui_movies::FindNode(movie, 3, 8));
  last.set_visible(down);
  return last.address;
}

uint64_t ShowRecord(uint32_t record, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8228A0D8 ReplayTheater::ShowRecord");
  const uint32_t frame = Frame(sp, 192);
  const uint32_t movie = FindMovie(frame + 96);
  const schema::SaveReplayRecord r(record);
  if (record == 0 || r.number() == 0) {
    static constexpr uint32_t kTexts[6] = {9, 10, 13, 19, 20, 21};
    static constexpr uint32_t kLr[6] = {0x8228A330, 0x8228A340, 0x8228A350,
                                        0x8228A360, 0x8228A370, 0x8228A380};
    for (uint32_t k = 0; k < 6; k++) {
      calls.Call(kClearText, frame, kLr[k], movie, 3, kTexts[k]);
    }
    Node(ui_movies::FindNode(movie, 3, 17)).set_texture_entry(0);
    return calls.Call(kClearText, frame, 0x8228A3AC, movie, 3, 12);
  }
  uint32_t time = r.time();
  if ((time & 0x7FFF0000u) > 0x3B0000u) {
    time = 0x3BEFE7u | (time & 0x80000000u);  // 59:59.999
  }
  // The date: its fifth argument on the stack (+84).
  Store(frame + 84, uint32_t(r.minute()));
  calls.CallArgs(kSetText, frame, 0x8228A164,
                 {movie, 3, 9, 530, uint64_t(r.year()), uint64_t(r.month()), uint64_t(r.day()),
                  uint64_t(r.hour())});
  calls.Call(kSetText, frame, 0x8228A17C, movie, 3, 10, uint64_t(r.mode()) + 473);
  calls.CallArgs(kSetText, frame, 0x8228A19C,
                 {movie, 3, 13, 528, (time >> 16) & 0x7FFF, (time >> 10) & 0x3F, time & 0x3FF, 0});
  calls.Call(kSetText, frame, 0x8228A1B4, movie, 3, 19, uint64_t(r.course()) + 1646);
  calls.Call(kSetText, frame, 0x8228A1CC, movie, 3, 20, uint64_t(r.course()) + 1790);
  calls.Call(kSetText, frame, 0x8228A1E4, movie, 3, 21, uint64_t(r.course()) + 1718);
  const uint32_t kind = r.mode();
  if (kind < 4) {
    uint64_t text;
    if (kind == 1 || kind == 3) {
      static constexpr uint32_t kModes[8] = {395, 396, 397, 398, 401, 402, 404, 403};
      for (uint32_t k = 0; k < 8; k++) {
        Store(frame + 112 + 4 * k, kModes[k]);
      }
      Node(ui_movies::FindNode(movie, 3, 17)).set_texture_entry(1);
      text = Load(frame + 112 + (r.result() << 2));
    } else if (kind == 2) {
      Node(ui_movies::FindNode(movie, 3, 17)).set_texture_entry(0);
      text = 477;
    } else {
      Node(ui_movies::FindNode(movie, 3, 17)).set_texture_entry(0);
      text = uint64_t(r.result()) + 4650;
    }
    calls.Call(kSetText, frame, 0x8228A2C4, movie, 3, 12, text);
  }
  // The record's +28 in two digits (children 5 and 6).
  const uint32_t tens = ui_movies::FindNode(movie, 3, 5);
  const uint32_t units = ui_movies::FindNode(movie, 3, 6);
  const int32_t number = int32_t(r.number());
  Node(tens).set_texture_entry(number < 10 ? 0 : uint32_t(number / 10));
  Node(units).set_texture_entry(uint32_t(number % 10));
  return units;
}

}  // namespace type10_mode
