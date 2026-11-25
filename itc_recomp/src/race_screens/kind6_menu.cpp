// Kind 6's screen after the race (Kind6Menu::, docs/functions/
// race-screens-unseen.md): its opener, draw and close with the helpers and
// transitions they reach; see unseen.h. Read from the generated code; never
// seen in play (ported by the user's call, 2026-11-12).

#include "race_screens/unseen.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "schema/network_mode.h"
#include "schema/race_screens.h"
#include "schema/race_screens_unseen.h"
#include "schema/save_interface.h"
#include "schema/tuning_screen.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace race_screens {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint64_t kMinusOne = ~uint64_t(0);

// The UI movie calls the originals make, through their overrides.
constexpr uint32_t kFinishAll = 0x8211DFF8;
constexpr uint32_t kShowNode = 0x8211AC00;
constexpr uint32_t kHideNode = 0x8211AC48;
constexpr uint32_t kScrollSpeed = 0x8211C4F0;  // a text's scroll delay and speed
constexpr uint32_t kScrollText = 0x8211C570;
constexpr uint32_t kScrollReset = 0x8211C5F8;  // the scroll counter 0
constexpr uint32_t kClearNodeText = 0x8211CC50;
// The float ShowHelp's scroll speed loads.
constexpr uint32_t kHelpSpeed = 0x82000A30;

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  guest::StoreU32(sp - bytes, sp);
  return sp - bytes;
}

void StoreU64(uint32_t address, uint64_t value) {
  guest::StoreU32(address, uint32_t(value >> 32));
  guest::StoreU32(address + 4, uint32_t(value));
}

// A lis and addi pair's value as the register holds it (sign-extended).
uint64_t Register(uint32_t address) { return uint64_t(int64_t(int32_t(address))); }

using Globals = schema::RaceScreensUnseenGlobals;

// The screen [0x82814E10], read again at each use.
schema::Kind6Menu Screen() { return Globals::kind6_menu(); }
// Movie 17 (the menu), the screen's +24.
uint32_t Menu() { return Screen().menu_movie().address; }

uint64_t PlayNamed(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie,
                   uint64_t named) {
  const uint64_t args[8] = {movie, named, 0, 0, 0, 0, 0, 0};
  return calls.CallArgs(kPlayNamed, frame, lr, args);
}

uint64_t FinishNamed(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie,
                     uint64_t named) {
  return calls.Call(kFinishNamed, frame, lr, movie, named);
}

uint64_t LoopNamed(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie,
                   uint64_t named) {
  return calls.Call(kLoopNamed, frame, lr, movie, named);
}

uint64_t LoopLast(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie) {
  return calls.Call(kLoopLast, frame, lr, movie);
}

uint64_t Run(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie) {
  return calls.Call(kRun, frame, lr, movie, 0);
}

// UiMovie::Run(movie, 0) answered 0 (cntlzw's bit).
uint64_t RunDone(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie) {
  return uint32_t(Run(calls, frame, lr, movie)) == 0 ? 1 : 0;
}

// UiMovie::SetText(movie, sub_scene, child, id, r7 to r10), the unset
// varargs 0 (the user's call for the parking area, 2026-10-30).
uint64_t SetText(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie,
                 uint64_t sub_scene, uint64_t child, uint64_t id, uint64_t r7 = 0,
                 uint64_t r8 = 0, uint64_t r9 = 0, uint64_t r10 = 0) {
  const uint64_t args[8] = {movie, sub_scene, child, id, r7, r8, r9, r10};
  return calls.CallArgs(kSetText, frame, lr, args);
}

// UiMovie::SetNodeText(movie, node, id, r6), r7 to r10 0.
uint64_t SetNodeText(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie,
                     uint32_t node, uint64_t id, uint64_t r6 = 0) {
  const uint64_t args[8] = {movie, node, id, r6, 0, 0, 0, 0};
  return calls.CallArgs(kSetNodeText, frame, lr, args);
}

uint64_t Node(const Calls& calls, uint32_t function, uint32_t frame, uint32_t lr, uint32_t movie,
              uint64_t sub_scene, uint64_t child) {
  return calls.Call(function, frame, lr, movie, sub_scene, child);
}

uint64_t MenuSound(const Calls& calls, uint32_t frame, uint32_t lr) {
  return calls.Call(kMenuSoundPlay, frame, lr, 5);
}

// A value's float as the originals make it (std, lfd, fcfid, frsp): its
// doubleword left at `slot`.
float ToFloat(uint32_t slot, uint64_t value) {
  StoreU64(slot, value);
  return float(double(int64_t(value)));
}

}  // namespace

// --- The helpers ---

uint64_t Kind6Close(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221C168 Kind6Menu::Close");
  const uint32_t frame = Frame(sp, 96);
  constexpr uint32_t kLr[4] = {0x8221C190, 0x8221C1A4, 0x8221C1B8, 0x8221C1CC};
  uint64_t r3 = 0;
  // The menu, question, message and waiting movies (+24 to +36), the screen
  // read again for each.
  auto movie_at = [&](uint32_t i) {
    const schema::Kind6Menu k = Screen();
    const uint32_t at[4] = {k.menu_movie_address(), k.question_movie_address(),
                            k.message_movie_address(), k.waiting_movie_address()};
    return guest::LoadU32(at[i]);
  };
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t movie = movie_at(i);
    r3 = movie;
    if (movie != 0) r3 = calls.Call(kFinishAll, frame, kLr[i], movie);
  }
  return r3;
}

uint64_t Kind6HidePanels(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221CFC0 Kind6Menu::HidePanels");
  const uint32_t frame = Frame(sp, 96);
  const uint32_t movie = Menu();
  constexpr uint32_t kLr[6] = {0x8221CFEC, 0x8221CFFC, 0x8221D00C,
                               0x8221D01C, 0x8221D02C, 0x8221D03C};
  uint64_t r3 = 0;
  for (uint32_t i = 0; i < 6; i++) r3 = Node(calls, kHideNode, frame, kLr[i], movie, 1, 4 + i);
  return r3;
}

uint64_t Kind6ShowPanelA(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221D050 Kind6Menu::ShowPanelA");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = Menu();
  Node(calls, kShowNode, frame, 0x8221D080, movie, 1, 4);
  Node(calls, kShowNode, frame, 0x8221D090, movie, 1, 5);
  const uint32_t a = ui_movies::FindNode(movie, 1, 4);
  const uint32_t b = ui_movies::FindNode(movie, 1, 5);
  if (a == 0 || b == 0) return b;
  schema::UiMovieNode(a).set_texture_entry(0);
  schema::UiMovieNode(b).set_texture_entry(0);
  constexpr uint32_t kLr[4] = {0x8221D0E4, 0x8221D0F4, 0x8221D104, 0x8221D114};
  uint64_t r3 = 0;
  for (uint32_t i = 0; i < 4; i++) r3 = Node(calls, kHideNode, frame, kLr[i], movie, 1, 6 + i);
  return r3;
}

uint64_t Kind6ShowHelp(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221D130 Kind6Menu::ShowHelp");
  const uint32_t frame = Frame(sp, 96);
  const uint32_t movie = Menu();
  SetText(calls, frame, 0x8221D160, movie, 2, 0, 967);
  const double speed = double(guest::LoadFloat(kHelpSpeed));
  calls.Call(kScrollSpeed, frame, 0x8221D17C, movie, 2, 0, 300, &speed);
  return calls.Call(kScrollText, frame, 0x8221D190, movie, 2, 0, 1);
}

uint64_t Kind6ShowCourse(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221D1A8 Kind6Menu::ShowCourse");
  const uint32_t frame = Frame(sp, 240);
  const uint32_t movie = Menu();
  SetText(calls, frame, 0x8221D1D4, movie, 9, 55, 402);
  SetText(calls, frame, 0x8221D1E8, movie, 9, 56, 401);
  SetText(calls, frame, 0x8221D1FC, movie, 9, 57, 407);
  SetText(calls, frame, 0x8221D210, movie, 9, 58, 408);
  const uint32_t course = Globals::kind6_course();
  // The two tables (the course's marks) in the frame, stored as the
  // original's.
  guest::StoreU32(frame + 128, 25);
  constexpr uint32_t kFirst[8] = {0, 0, 2, 2, 5, 6, 3, 4};
  for (uint32_t i = 0; i < 8; i++) guest::StoreU32(frame + 80 + 4 * i, kFirst[i]);
  for (uint32_t i = 0; i < 4; i++) guest::StoreU32(frame + 112 + 4 * i, kNone);
  guest::StoreU32(frame + 132, 26);
  guest::StoreU32(frame + 136, kNone);
  guest::StoreU32(frame + 140, kNone);
  // Case 0 (sub_8221D2B4) and a course past 7 both take row 0.
  const uint32_t row = course <= 7 ? course : 0;
  for (uint32_t i = 0; i < 27; i++) {
    schema::UiMovieNode(ui_movies::FindNode(movie, 9, i + 6)).set_visible(0);
  }
  const uint32_t a = guest::LoadU32(frame + 80 + 4 * row);
  const uint32_t first = ui_movies::FindNode(movie, 9, a + 6);
  const uint32_t b = guest::LoadU32(frame + 112 + 4 * row);
  schema::UiMovieNode(first).set_visible(1);
  schema::UiMovieNode(ui_movies::FindNode(movie, 9, b + 6)).set_visible(1);
  for (uint32_t i = 0; i < 19; i++) FinishNamed(calls, frame, 0x8221D364, movie, i + 47);
  constexpr uint32_t kRows[8] = {1, 0, 2, 3, 4, 7, 5, 6};
  for (uint32_t i = 0; i < 5; i++) guest::StoreU32(frame + 112 + 4 * i, kRows[i]);
  guest::StoreU32(frame + 140, 7);
  guest::StoreU32(frame + 132, 5);
  guest::StoreU32(frame + 136, 6);
  const uint64_t named = uint64_t(guest::LoadU32(frame + 112 + 4 * row)) + 47;
  FinishNamed(calls, frame, 0x8221D3AC, movie, named);
  return PlayNamed(calls, frame, 0x8221D3C8, movie, named);
}

uint64_t Kind6SetUpTexts(uint32_t screen_address, uint32_t sp,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221E038 Kind6Menu::SetUpTexts");
  const schema::Kind6Menu screen(screen_address);
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = screen.menu_movie().address;
  constexpr uint32_t kItems[4] = {1965, 1961, 1962, 115};
  for (uint32_t i = 0; i < 4; i++) guest::StoreU32(frame + 80 + 4 * i, kItems[i]);
  for (uint32_t i = 0; i < 4; i++) {
    SetText(calls, frame, 0x8221E084, movie, 3, i + 5, guest::LoadU32(frame + 80 + 4 * i));
  }
  for (uint32_t i = 0; i < 5; i++) {
    schema::UiMovieNode(ui_movies::FindNode(movie, kNone, i + 23)).set_visible(0);
  }
  Kind6ShowPanelA(frame, calls);
  // The course's name (case 0, sub_8221E0FC, merged: 395).
  constexpr uint32_t kNames[8] = {395, 396, 397, 398, 401, 402, 404, 403};
  const uint32_t course = Globals::kind6_course();
  SetText(calls, frame, 0x8221E148, movie, kMinusOne, 21, course <= 7 ? kNames[course] : 0);
  SetText(calls, frame, 0x8221E170, movie, kMinusOne, 22,
          int32_t(Globals::kind6_course_flag()) == 0 ? 1 : 0);
  Node(calls, kHideNode, frame, 0x8221E180, movie, kMinusOne, 17);
  Node(calls, kHideNode, frame, 0x8221E190, movie, kMinusOne, 16);
  Kind6ShowCourse(frame, calls);
  return Kind6ShowHelp(frame, calls);
}

uint32_t Kind6SetCursorX(uint64_t value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8221D7B0 Kind6Menu::SetCursorX");
  const uint32_t frame = Frame(sp, 112);
  const float x = ToFloat(frame + 80, value & 0xFFFFFFFFu);
  const uint32_t node = ui_movies::FindNode(Menu(), 3, 0);
  schema::UiMovieNode(node).set_y(x);
  return node;
}

uint64_t Kind6ShowReplay(uint32_t record, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221CBD8 Kind6Menu::ShowReplay");
  const uint32_t frame = Frame(sp, 176);
  const uint32_t movie = Menu();
  // A replay record (save-interface.toml's SaveReplayRecord).
  const schema::SaveReplayRecord replay(record);
  if (record == 0 || replay.number() == 0) {
    schema::UiMovieNode(ui_movies::FindNode(movie, 14, 0)).set_visible(0);
    constexpr uint32_t kTexts[7][2] = {{9, 0x8221CE38},  {10, 0x8221CE48}, {19, 0x8221CE58},
                                       {20, 0x8221CE68}, {21, 0x8221CE78}, {12, 0x8221CE88},
                                       {13, 0x8221CE98}};
    uint64_t r3 = 0;
    for (const auto& t : kTexts) r3 = Node(calls, kClearText, frame, t[1], movie, 14, t[0]);
    return r3;
  }
  // The date and time: string 530 with the halfwords +0, +2, +6 and +8 in
  // r7 to r10 and +10 on the stack (the frame's +84).
  guest::StoreU32(frame + 84, replay.minute());
  SetText(calls, frame, 0x8221CC34, movie, 14, 9, 530, replay.year(), replay.month(), replay.day(),
          replay.hour());
  SetText(calls, frame, 0x8221CC4C, movie, 14, 10, uint64_t(replay.mode()) + 473);
  SetText(calls, frame, 0x8221CC64, movie, 14, 19, uint64_t(replay.course()) + 1646);
  SetText(calls, frame, 0x8221CC7C, movie, 14, 20, uint64_t(replay.course()) + 1790);
  SetText(calls, frame, 0x8221CC94, movie, 14, 21, uint64_t(replay.course()) + 1718);
  const uint32_t time = replay.time();
  SetText(calls, frame, 0x8221CCB8, movie, 14, 13, 528, (time >> 16) & 0x7FFF, (time >> 10) & 0x3F,
          time & 0x3FF);
  // The result by the kind (+20): 0 string 4650 + +32; 1 and 3 the place
  // (a table in the frame by +32, unchecked); 2 string 477; others none.
  const uint32_t kind = replay.mode();
  if (kind <= 3) {
    uint64_t id;
    if (kind == 1 || kind == 3) {
      constexpr uint32_t kPlaces[8] = {395, 396, 397, 398, 401, 402, 404, 403};
      for (uint32_t i = 0; i < 8; i++) guest::StoreU32(frame + 96 + 4 * i, kPlaces[i]);
      schema::UiMovieNode(ui_movies::FindNode(movie, 14, 17)).set_texture_entry(1);
      const uint32_t index = replay.result();
      id = guest::LoadU32((index << 2) + frame + 96);
    } else if (kind == 2) {
      schema::UiMovieNode(ui_movies::FindNode(movie, 14, 17)).set_texture_entry(0);
      id = 477;
    } else {
      schema::UiMovieNode(ui_movies::FindNode(movie, 14, 17)).set_texture_entry(0);
      id = uint64_t(replay.result()) + 4650;
    }
    SetText(calls, frame, 0x8221CD98, movie, 14, 12, id);
  }
  schema::UiMovieNode(ui_movies::FindNode(movie, 14, 0)).set_visible(1);
  const uint32_t tens = ui_movies::FindNode(movie, 14, 5);
  const uint32_t ones = ui_movies::FindNode(movie, 14, 6);
  const int32_t count = int32_t(replay.number());
  schema::UiMovieNode(tens).set_texture_entry(count < 10 ? 0 : uint32_t(count / 10));
  const int32_t again = int32_t(replay.number());
  schema::UiMovieNode(ones).set_texture_entry(uint32_t(again - (again / 10) * 10));
  return ones;
}

uint32_t Kind6ShowList(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221D4E8 Kind6Menu::ShowList");
  const uint32_t frame = Frame(sp, 160);
  const schema::SaveBuffers replays = Globals::kind6_replays();
  const int32_t count = int32_t(replays.count());
  const uint32_t records = replays.records().address;
  const uint32_t movie = Menu();
  const uint32_t scroll = Screen().slot_list_scroll();
  const int32_t shown = count < 9 ? count : 9;
  const uint32_t node = ui_movies::FindNode(movie, 14, 2);
  for (uint32_t i = 0; i < 9; i++) {
    calls.Call(kClearNodeText, frame, 0x8221D558, movie, ui_movies::Child(node, i + 5));
  }
  uint32_t at = records + scroll * 36;
  for (int32_t i = 0; i < shown; i++, at += 36) {
    const uint32_t child = ui_movies::Child(node, uint32_t(i) + 5);
    const uint32_t word = schema::SaveReplayRecord(at).number();
    SetNodeText(calls, frame, word == 0 ? 0x8221D5A4 : 0x8221D5B0, movie, child,
                word == 0 ? 472 : 471, word);
  }
  // The scroll marks: none with 9 records or fewer; at the top only the
  // lower one, at the bottom only the upper one, else both.
  const uint32_t up = ui_movies::FindNode(movie, 14, 7);
  uint32_t up_shown = 0, down_shown = 0;
  if (count > 9) {
    if (int32_t(scroll) == 0) {
      down_shown = 1;
    } else if (int32_t(scroll) == count - 9) {
      up_shown = 1;
    } else {
      up_shown = down_shown = 1;
    }
  }
  schema::UiMovieNode(up).set_visible(up_shown);
  const uint32_t down = ui_movies::FindNode(movie, 14, 8);
  schema::UiMovieNode(down).set_visible(down_shown);
  return down;
}

uint64_t Kind6SetUpList(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221DF70 Kind6Menu::SetUpList");
  const uint32_t frame = Frame(sp, 160);
  const schema::SaveBuffers replays = Globals::kind6_replays();
  const schema::Kind6Menu screen = Screen();
  const uint32_t count = replays.count();
  const uint32_t movie = screen.menu_movie().address;
  const uint32_t records = replays.records().address;
  const uint32_t shown = int32_t(count) < 9 ? count : 9;
  guest::StoreU32(frame + 92, 158);
  guest::StoreU32(frame + 84, 159);
  const uint64_t init[8] = {uint64_t(screen.slot_list_count_address()), count, shown, 1, 154, 153, 155, 156};
  calls.CallArgs(kListInit, frame, 0x8221DFD8, init);
  const uint32_t selection = ListSelection(Screen().slot_list_count_address());
  const uint32_t node = ui_movies::FindNode(movie, 14, 2);
  const float y = ToFloat(frame + 96, uint64_t(int64_t(int32_t(selection * 40))));
  schema::UiMovieNode(ui_movies::Child(node, 0)).set_y(y);
  Kind6ShowList(frame, calls);
  return Kind6ShowReplay(records + selection * 36, frame, calls);
}

// --- Page object A's transitions ---

uint64_t Kind6Intro(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221C438 Kind6Menu::Intro");
  const uint32_t frame = Frame(sp, 112);
  if (guest::LoadU32(step) == 0) {
    guest::StoreU32(step, 1);
    const uint32_t movie = Menu();
    calls.Call(kFinishAll, frame, 0x8221C46C, movie);
    PlayNamed(calls, frame, 0x8221C488, movie, 123);
    Screen().set_menu_movie_running(1);
  }
  const uint64_t done = RunDone(calls, frame, 0x8221C4A0, Menu());
  if (done != 0) {
    const uint32_t movie = Menu();
    constexpr uint32_t kNamed[4][3] = {{124, 0x8221C4D0, 0x8221C4D8},
                                       {0, 0x8221C4F4, 0x8221C4FC},
                                       {68, 0x8221C518, 0x8221C520},
                                       {47, 0x8221C53C, 0x8221C544}};
    for (const auto& n : kNamed) {
      PlayNamed(calls, frame, n[1], movie, n[0]);
      LoopLast(calls, frame, n[2], movie);
    }
  }
  return done;
}

uint64_t Kind6Show(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221C550 Kind6Menu::Show");
  const uint32_t frame = Frame(sp, 112);
  if (guest::LoadU32(step) == 0) {
    guest::StoreU32(step, 1);
    const uint32_t movie = Menu();
    PlayNamed(calls, frame, 0x8221C5A0, movie, 7);
    LoopLast(calls, frame, 0x8221C5A8, movie);
  }
  Run(calls, frame, 0x8221C5B8, Menu());
  return 1;
}

uint64_t Kind6RunToEnd(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221C5D8 Kind6Menu::RunToEnd");
  const uint32_t frame = Frame(sp, 96);
  if (guest::LoadU32(step) == 0) guest::StoreU32(step, 1);
  return RunDone(calls, frame, 0x8221C60C, Menu());
}

uint64_t Kind6MoveMain(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221DB48 Kind6Menu::MoveMain");
  const uint32_t frame = Frame(sp, 128);
  if (guest::LoadU32(step) == 0) {
    guest::StoreU32(step, 1);
    const uint32_t movie = Menu();
    const uint32_t cursor = Screen().main_menu_cursor();
    const uint32_t before = Screen().main_menu_previous();
    FinishNamed(calls, frame, 0x8221DB88, movie, 17);
    FinishNamed(calls, frame, 0x8221DB94, movie, 18);
    FinishNamed(calls, frame, 0x8221DBA0, movie, uint64_t(before) + 7);
    PlayNamed(calls, frame, 0x8221DBBC, movie, uint64_t(cursor) + 7);
    LoopLast(calls, frame, 0x8221DBC4, movie);
    // A wrap moves the cursor's marker; else the slide by the last move
    // (+76: -1 named 17, 1 named 18).
    if (int32_t(cursor) == 0 && int32_t(before) == 3) {
      Kind6SetCursorX(0, frame);
    } else if (int32_t(before) == 0 && int32_t(cursor) == 3) {
      Kind6SetCursorX(120, frame);
    } else if (int32_t(Screen().main_menu_moved()) == -1) {
      PlayNamed(calls, frame, 0x8221DC3C, movie, 17);
    } else if (int32_t(Screen().main_menu_moved()) == 1) {
      PlayNamed(calls, frame, 0x8221DC3C, movie, 18);
    }
    SetText(calls, frame, 0x8221DC58, movie, 2, 0, uint64_t(Screen().main_menu_cursor()) + 967);
    Node(calls, kScrollReset, frame, 0x8221DC68, movie, 2, 0);
  }
  Run(calls, frame, 0x8221DC78, Menu());
  return 1;
}

uint64_t Kind6BackToQuit(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221DC88 Kind6Menu::BackToQuit");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = Menu();
  if (guest::LoadU32(step) == 0) {
    const uint32_t cursor = Screen().main_menu_cursor();
    Screen().set_main_menu_cursor(3);
    Screen().set_main_menu_previous(cursor);
    const uint32_t now = Screen().main_menu_cursor();
    FinishNamed(calls, frame, 0x8221DCD4, movie, 17);
    FinishNamed(calls, frame, 0x8221DCE0, movie, 18);
    for (uint32_t i = 0; i < 4; i++) FinishNamed(calls, frame, 0x8221DCF0, movie, i + 7);
    PlayNamed(calls, frame, 0x8221DD18, movie, uint64_t(now) + 7);
    LoopLast(calls, frame, 0x8221DD20, movie);
    Kind6SetCursorX(120, frame);
    SetText(calls, frame, 0x8221DD44, movie, 2, 0, uint64_t(Screen().main_menu_cursor()) + 967);
    Node(calls, kScrollReset, frame, 0x8221DD54, movie, 2, 0);
    guest::StoreU32(step, 1);
  }
  Run(calls, frame, 0x8221DD6C, Menu());
  return 1;
}

uint64_t Kind6OpenList(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221EB98 Kind6Menu::OpenList");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = Menu();
  if (guest::LoadU32(step) == 0) {
    guest::StoreU32(step, 1);
    PlayNamed(calls, frame, 0x8221EBE0, movie, 126);
    Kind6SetUpList(frame, calls);
  }
  const uint64_t done = RunDone(calls, frame, 0x8221EBF4, Menu());
  if (done != 0) {
    const uint64_t row = uint64_t(ListSelection(Screen().slot_list_count_address())) + 72;
    PlayNamed(calls, frame, 0x8221EC30, movie, row);
    LoopNamed(calls, frame, 0x8221EC3C, movie, row);
  }
  return done;
}

uint64_t Kind6CloseList(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221C628 Kind6Menu::CloseList");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = Menu();
  if (guest::LoadU32(step) == 0) {
    guest::StoreU32(step, 1);
    const uint64_t row = uint64_t(ListSelection(Screen().slot_list_count_address())) + 72;
    FinishNamed(calls, frame, 0x8221C678, movie, row);
    PlayNamed(calls, frame, 0x8221C698, Menu(), 127);
  }
  return RunDone(calls, frame, 0x8221C6A8, Menu());
}

uint64_t Kind6MoveList(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221DD78 Kind6Menu::MoveList");
  const uint32_t frame = Frame(sp, 176);
  if (guest::LoadU32(step) == 0) {
    guest::StoreU32(step, 1);
    const uint32_t list = Screen().slot_list_count_address();
    const uint32_t movie = Menu();
    const uint32_t selection = ListSelection(list);
    // MenuList::GetPreviousSelection (sub_8220BC28): the list's +20.
    const schema::MenuList slots(list);
    const uint32_t previous = slots.selection_before();
    const uint32_t scroll = slots.scroll();
    const uint32_t before = slots.scroll_before();
    const uint32_t wrapped = slots.wrapped();
    const int32_t scrolled = int32_t(scroll - before);
    const uint32_t records = Globals::kind6_replays().records().address;
    FinishNamed(calls, frame, 0x8221DDEC, movie, uint64_t(previous) + 72);
    const uint64_t row = uint64_t(selection) + 72;
    PlayNamed(calls, frame, 0x8221DE0C, movie, row);
    LoopNamed(calls, frame, 0x8221DE18, movie, row);
    Kind6ShowReplay(records + (scroll + selection) * 36, frame, calls);
    FinishNamed(calls, frame, 0x8221DE34, movie, 82);
    FinishNamed(calls, frame, 0x8221DE40, movie, 83);
    const uint32_t node = ui_movies::FindNode(movie, 14, 2);
    if (int32_t(wrapped) != 0) {
      const float y = ToFloat(frame + 80, uint64_t(int64_t(int32_t(selection * 40))));
      schema::UiMovieNode(ui_movies::Child(node, 0)).set_y(y);
    } else {
      // A row scrolled in: the slot that comes in shown (its record's +28)
      // and the slide played (82 up, 83 down).
      const int32_t move = int32_t(slots.move());
      if (move < 0) {
        const uint32_t child = ui_movies::Child(node, 15);
        SetNodeText(calls, frame, 0x8221DEB4, movie, child, 471,
                    schema::SaveReplayRecord(records + scroll * 36).number());
        PlayNamed(calls, frame, 0x8221DF00, movie, 82);
      } else if (move > 0) {
        const uint32_t child = ui_movies::Child(node, 14);
        SetNodeText(calls, frame, 0x8221DEE4, movie, child, 471,
                    schema::SaveReplayRecord(records + (scroll + 9) * 36).number());
        PlayNamed(calls, frame, 0x8221DF00, movie, 83);
      }
    }
    if (scrolled != 0) {
      FinishNamed(calls, frame, 0x8221DF14, movie, 95);
      FinishNamed(calls, frame, 0x8221DF20, movie, 94);
      Kind6ShowList(frame, calls);
      PlayNamed(calls, frame, 0x8221DF4C, movie, scrolled > 0 ? 95 : 94);
    }
  }
  Run(calls, frame, 0x8221DF5C, Menu());
  return 1;
}

// --- Page object B's transitions ---

uint64_t Kind6OpenQuestion(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221C6C8 Kind6Menu::OpenQuestion");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = Screen().question_movie().address;
  if (guest::LoadU32(step) == 0) {
    SetText(calls, frame, 0x8221C704, movie, 1, 3, 2);
    SetText(calls, frame, 0x8221C718, movie, 1, 4, 3);
    SetText(calls, frame, 0x8221C730, movie, kMinusOne, 0, Screen().box_message());
    PlayNamed(calls, frame, 0x8221C74C, movie, 5);
    Screen().set_input_on(0);
    Screen().set_box_shown(0);
    MenuSound(calls, frame, 0x8221C768);
    guest::StoreU32(step, 1);
  }
  const uint64_t done = RunDone(calls, frame, 0x8221C77C, movie);
  if (done != 0) {
    FinishNamed(calls, frame, 0x8221C794, movie, 1);
    FinishNamed(calls, frame, 0x8221C7A0, movie, 2);
    PlayNamed(calls, frame, 0x8221C7C4, movie, uint64_t(Screen().question_menu_cursor()) + 1);
    LoopLast(calls, frame, 0x8221C7CC, movie);
  }
  return done;
}

uint64_t Kind6CloseQuestion(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221C7D8 Kind6Menu::CloseQuestion");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = Screen().question_movie().address;
  if (guest::LoadU32(step) == 0) {
    FinishNamed(calls, frame, 0x8221C810, movie, 1);
    FinishNamed(calls, frame, 0x8221C81C, movie, 2);
    PlayNamed(calls, frame, 0x8221C838, movie, 6);
    MenuSound(calls, frame, 0x8221C840);
    guest::StoreU32(step, 1);
  }
  const uint64_t done = RunDone(calls, frame, 0x8221C850, movie);
  if (done != 0) {
    Screen().set_box_shown(3);
    Screen().set_input_on(1);
    Screen().set_ranking_skipped(0);
  }
  return done;
}

uint64_t Kind6MoveQuestion(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221C888 Kind6Menu::MoveQuestion");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = Screen().question_movie().address;
  if (guest::LoadU32(step) == 0) {
    FinishNamed(calls, frame, 0x8221C8BC, movie, 1);
    FinishNamed(calls, frame, 0x8221C8C8, movie, 2);
    PlayNamed(calls, frame, 0x8221C8EC, movie, uint64_t(Screen().question_menu_cursor()) + 1);
    LoopNamed(calls, frame, 0x8221C900, movie, uint64_t(Screen().question_menu_cursor()) + 1);
    guest::StoreU32(step, 1);
  }
  Run(calls, frame, 0x8221C914, movie);
  return 1;
}

uint64_t Kind6OpenMessage(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221CAB8 Kind6Menu::OpenMessage");
  const uint32_t frame = Frame(sp, 128);
  const schema::Kind6Menu screen = Screen();
  const uint32_t movie = screen.message_movie().address;
  if (guest::LoadU32(step) == 0) {
    SetText(calls, frame, 0x8221CAF4, movie, kMinusOne, 1, screen.box_message());
    PlayNamed(calls, frame, 0x8221CB10, movie, 2);
    Screen().set_input_on(0);
    Screen().set_box_shown(1);
    MenuSound(calls, frame, 0x8221CB30);
    guest::StoreU32(step, 1);
  }
  return RunDone(calls, frame, 0x8221CB40, movie);
}

uint64_t Kind6CloseMessage(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221CB50 Kind6Menu::CloseMessage");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = Screen().message_movie().address;
  if (guest::LoadU32(step) == 0) {
    PlayNamed(calls, frame, 0x8221CB98, movie, 3);
    MenuSound(calls, frame, 0x8221CBA0);
    guest::StoreU32(step, 1);
  }
  const uint64_t done = RunDone(calls, frame, 0x8221CBB0, movie);
  if (done != 0) {
    Screen().set_box_shown(3);
    Screen().set_input_on(1);
  }
  return done;
}

uint64_t Kind6OpenWaiting(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221C920 Kind6Menu::OpenWaiting");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = Screen().waiting_movie().address;
  if (guest::LoadU32(step) == 0) {
    const uint32_t node = ui_movies::FindNode(movie, 0, 2);
    const uint32_t text = ui_movies::Child(node, 1);
    SetNodeText(calls, frame, 0x8221C978, movie, text, Screen().box_message());
    schema::UiMovieNode(ui_movies::Child(node, 0)).set_visible(0);
    PlayNamed(calls, frame, 0x8221C9A8, movie, 13);
    FinishNamed(calls, frame, 0x8221C9B4, movie, 0);
    PlayNamed(calls, frame, 0x8221C9D0, movie, 0);
    LoopNamed(calls, frame, 0x8221C9DC, movie, 0);
    Screen().set_box_shown(2);
    Screen().set_input_on(0);
    MenuSound(calls, frame, 0x8221C9F8);
    guest::StoreU32(step, 1);
  }
  return RunDone(calls, frame, 0x8221CA0C, movie);
}

uint64_t Kind6CloseWaiting(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221CA20 Kind6Menu::CloseWaiting");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = Screen().waiting_movie().address;
  if (guest::LoadU32(step) == 0) {
    PlayNamed(calls, frame, 0x8221CA68, movie, 15);
    MenuSound(calls, frame, 0x8221CA70);
    guest::StoreU32(step, 1);
  }
  const uint64_t done = RunDone(calls, frame, 0x8221CA80, movie);
  if (done != 0) {
    Screen().set_box_shown(3);
    Screen().set_input_on(1);
    FinishNamed(calls, frame, 0x8221CAAC, movie, 0);
  }
  return done;
}

// --- The opener, draw and close ---

uint64_t Kind6Open(uint32_t screen_address, uint32_t block, uint32_t sp,
                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221EC48 Kind6Menu::Open");
  const schema::Kind6Menu screen(screen_address);
  const uint32_t frame = Frame(sp, 160);
  // The block (+0 to +20).
  if (!c_runtime::Memcpy(screen.address, block, 24)) {
    calls.Call(kMemcpy, frame, 0x8221EC60, screen.address, block, 24);
  }
  // Past 59 minutes: 59:59.999 (the sign bit kept).
  const uint32_t time = screen.time();
  if ((time & 0x7FFF0000u) >= 0x3C0000u) {
    screen.set_time((time & 0x80000000u) | 0x3BEFE7u);
  }
  // UI movies 17, 20, 21 and 14 into the menu, question, message and
  // waiting movies (+24 to +36).
  const uint32_t movies[4] = {screen.menu_movie_address(), screen.question_movie_address(),
                              screen.message_movie_address(), screen.waiting_movie_address()};
  constexpr uint32_t kMovieIds[4] = {17, 20, 21, 14};
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(frame + 96, kMovieIds[i]);
    const uint32_t found = ui_movies::PairsFind(ui_movies::kMovieTable, frame + 96);
    guest::StoreU32(movies[i], guest::LoadU32(found));
  }
  auto answered = [&] { return Globals::kind6_answer() <= 2; };
  if (answered()) {
    constexpr uint32_t kLr[4] = {0x8221ED30, 0x8221ED38, 0x8221ED40, 0x8221ED48};
    for (uint32_t i = 0; i < 4; i++) {
      calls.Call(kResetPose, frame, kLr[i], guest::LoadU32(movies[i]));
    }
  }
  const uint32_t a = screen.pages_a_page_address(), b = screen.pages_b_page_address();
  PagesInit(a, 0);
  PagesInit(b, 0);
  Screen().set_menu_movie_running(0);
  if (answered()) {
    screen.set_box_message(44);
    PagesGo(a, 12, 0, 1);
    PagesGo(b, 5, 5, kNone);
  } else {
    PagesGo(a, 0, kNone, kNone);
  }
  screen.set_main_menu_first_item(0);
  screen.set_main_menu_cursor(0);
  screen.set_main_menu_previous(0);
  screen.set_main_menu_moved(0);
  guest::StoreU32(frame + 84, 159);
  guest::StoreU32(frame + 92, 158);
  screen.set_main_menu_last_item(3);
  screen.set_main_menu_wraps(1);
  screen.set_main_menu_wrapped(0);
  screen.set_main_menu_visible_row(0);
  screen.set_main_menu_previous_row(0);
  screen.set_main_menu_side(0);
  screen.set_main_menu_player(0);
  screen.set_main_menu_up_down_locked(0);
  screen.set_main_menu_left_right_steps(0);
  screen.set_main_menu_shoulders_step(0);
  const uint64_t init[8] = {uint64_t(screen.slot_list_count_address()), 4, 4, 1, 154, 153, 155, 156};
  calls.CallArgs(kListInit, frame, 0x8221EE48, init);
  screen.set_question_menu_first_item(0);
  screen.set_question_menu_last_item(1);
  screen.set_question_menu_wraps(1);
  screen.set_question_menu_cursor(0);
  screen.set_question_menu_previous(0);
  screen.set_question_menu_moved(0);
  screen.set_question_menu_wrapped(0);
  screen.set_question_menu_visible_row(0);
  screen.set_question_menu_previous_row(0);
  screen.set_question_menu_side(0);
  screen.set_question_menu_player(0);
  screen.set_question_menu_up_down_locked(0);
  screen.set_question_menu_left_right_steps(0);
  screen.set_question_menu_shoulders_step(0);
  if (answered()) {
    // The race's time in milliseconds for the ranking.
    const uint32_t t = screen.time();
    const uint64_t minutes =
        uint64_t(int64_t(int32_t((t >> 16) & 0x7FFF)) * 60000) & 0xFFFFFFFFu;
    const uint64_t seconds = (uint64_t((t >> 10) & 0x3F) * 1000) & 0xFFFFFFFFu;
    StoreU64(Globals::kind6_session().address + 200, seconds + minutes + (t & 0x3FF));
  }
  if (answered()) {
    screen.set_unknown_304(0);
    screen.set_rank(0);
    screen.set_rows_read(0);
    screen.set_rows_shown(0);
    screen.set_read_started(0);
    screen.set_upload_request(0);
    screen.set_ranking_skipped(0);
    screen.set_message_which(0);
    screen.set_question_which(0);
    screen.set_box_shown(3);
    screen.set_reset_request(0);
  }
  screen.set_input_on(1);
  uint64_t r3 = Kind6SetUpTexts(screen.address, frame, calls);
  if (answered()) r3 = Kind6HidePanels(frame, calls);
  return r3;
}

uint64_t Kind6Draw(uint32_t screen_address, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221EF60 Kind6Menu::Draw");
  const schema::Kind6Menu screen(screen_address);
  const uint32_t frame = Frame(sp, 144);
  // A's: Intro, Show, RunToEnd, MoveMain, OpenList, CloseList, MoveList,
  // BackToQuit.
  constexpr uint32_t kA[8] = {0x8221C438, 0x8221C550, 0x8221C5D8, 0x8221DB48,
                              0x8221EB98, 0x8221C628, 0x8221DD78, 0x8221DC88};
  for (uint32_t i = 0; i < 8; i++) guest::StoreU32(frame + 80 + 4 * i, kA[i]);
  const int32_t transition = int32_t(screen.pages_a_step());
  uint64_t r3;
  if (transition >= 0) {
    const uint32_t step = screen.pages_a_step_started_address();
    uint64_t result;
    switch (transition) {
      case 0: result = Kind6Intro(step, frame, calls); break;
      case 1: result = Kind6Show(step, frame, calls); break;
      case 2: result = Kind6RunToEnd(step, frame, calls); break;
      case 3: result = Kind6MoveMain(step, frame, calls); break;
      case 4: result = Kind6OpenList(step, frame, calls); break;
      case 5: result = Kind6CloseList(step, frame, calls); break;
      case 6: result = Kind6MoveList(step, frame, calls); break;
      case 7: result = Kind6BackToQuit(step, frame, calls); break;
      default: {
        // Past the table: the frame's word, as the original's bctrl.
        const uint32_t handler =
            guest::LoadU32((uint32_t(transition) << 2) + frame + 80);
        const uint64_t regs[8] = {step,
                                  Register(kA[1]),
                                  Register(kA[2]),
                                  Register(kA[3]),
                                  Register(kA[4]),
                                  Register(kA[5]),
                                  Register(kA[6]),
                                  uint64_t(frame) + 80};
        result = calls.CallArgs(handler, frame, 0x8221F008, regs);
      }
    }
    PagesFinish(screen.pages_a_page_address(), uint32_t(result));
    r3 = uint64_t(screen.pages_a_page_address());
  } else if (Screen().menu_movie_running() != 0) {
    r3 = Run(calls, frame, 0x8221F03C, screen.menu_movie().address);
  } else {
    r3 = Register(kA[0]);
  }
  const int32_t page = int32_t(screen.pages_a_page());
  if ((page >= 3 && page <= 5) || page == 13) r3 = RaceDialogDraw(frame, calls);
  // B's (the eighth word, BackToQuit, left from A's table): OpenQuestion,
  // CloseQuestion, OpenMessage, CloseMessage, MoveQuestion, OpenWaiting,
  // CloseWaiting.
  constexpr uint32_t kB[7] = {0x8221C6C8, 0x8221C7D8, 0x8221CAB8, 0x8221CB50,
                              0x8221C888, 0x8221C920, 0x8221CA20};
  for (uint32_t i = 0; i < 7; i++) guest::StoreU32(frame + 80 + 4 * i, kB[i]);
  const int32_t b_transition = int32_t(screen.pages_b_step());
  if (b_transition != -1) {
    uint64_t result = 1;
    if (b_transition >= 0) {
      const uint32_t step = screen.pages_b_step_started_address();
      switch (b_transition) {
        case 0: result = Kind6OpenQuestion(step, frame, calls); break;
        case 1: result = Kind6CloseQuestion(step, frame, calls); break;
        case 2: result = Kind6OpenMessage(step, frame, calls); break;
        case 3: result = Kind6CloseMessage(step, frame, calls); break;
        case 4: result = Kind6MoveQuestion(step, frame, calls); break;
        case 5: result = Kind6OpenWaiting(step, frame, calls); break;
        case 6: result = Kind6CloseWaiting(step, frame, calls); break;
        case 7: result = Kind6BackToQuit(step, frame, calls); break;
        default: {
          const uint32_t handler =
              guest::LoadU32((uint32_t(b_transition) << 2) + frame + 80);
          const uint64_t regs[8] = {step,
                                    Register(kB[0]),
                                    Register(kB[1]),
                                    Register(kB[2]),
                                    Register(kB[3]),
                                    Register(kB[4]),
                                    Register(kB[5]),
                                    uint64_t(frame) + 80};
          result = calls.CallArgs(handler, frame, 0x8221F0DC, regs);
        }
      }
    }
    PagesFinish(screen.pages_b_page_address(), uint32_t(result));
    return uint64_t(screen.pages_b_page_address());
  }
  if (Screen().menu_movie_running() != 0) {
    const int32_t b_page = int32_t(screen.pages_b_page());
    // The box's movie: the question's, the message's or the waiting one's.
    uint32_t field = 0;
    if (b_page > 0 && b_page <= 2) {
      field = screen.question_movie_address();
    } else if (b_page > 2 && b_page <= 4) {
      field = screen.message_movie_address();
    } else if (b_page == 5) {
      field = screen.waiting_movie_address();
    }
    if (field != 0) r3 = Run(calls, frame, 0x8221F144, guest::LoadU32(field));
  }
  if (Screen().upload_request() != 0) {
    Globals::kind6_session().set_upload_state(3);
    Screen().set_upload_request(0);
    const schema::Kind6Menu k = Screen();
    const uint32_t state = k.box_shown();
    if (state <= 2) {
      constexpr uint32_t kTransitions[3] = {1, 3, 6};
      PagesGo(k.pages_b_page_address(), 0, kTransitions[state], kNone);
      r3 = uint64_t(k.pages_b_page_address());
    }
    if (state <= 3) {
      const schema::Kind6Menu again = Screen();
      PagesGo(again.pages_a_page_address(), 11, kNone, kNone);
      r3 = uint64_t(again.pages_a_page_address());
    }
  }
  return r3;
}

}  // namespace race_screens
