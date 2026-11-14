// Kind 6's screen after the race (Kind6Menu::, docs/functions/
// race-screens-unseen.md): its opener, draw and close with the helpers and
// transitions they reach; see unseen.h. Read from the generated code; never
// seen in play (ported by the user's call, 2026-11-12).

#include "race_screens/unseen.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
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

uint32_t Get(uint8_t* base, uint32_t global) { return guest::LoadU32(base, global); }

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  guest::StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

void StoreU64(uint8_t* base, uint32_t address, uint64_t value) {
  guest::StoreU32(base, address, uint32_t(value >> 32));
  guest::StoreU32(base, address + 4, uint32_t(value));
}

// A lis and addi pair's value as the register holds it (sign-extended).
uint64_t Register(uint32_t address) { return uint64_t(int64_t(int32_t(address))); }

uint32_t Screen(uint8_t* base) { return Get(base, kKind6Menu); }
uint32_t Field(uint8_t* base, uint32_t field) { return guest::LoadU32(base, Screen(base) + field); }
void SetField(uint8_t* base, uint32_t field, uint32_t value) {
  guest::StoreU32(base, Screen(base) + field, value);
}
// Movie 17 (the menu), the screen's +24.
uint32_t Menu(uint8_t* base) { return Field(base, 24); }

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
float ToFloat(uint8_t* base, uint32_t slot, uint64_t value) {
  StoreU64(base, slot, value);
  return float(double(int64_t(value)));
}

}  // namespace

// --- The helpers ---

uint64_t Kind6Close(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221C168 Kind6Menu::Close");
  const uint32_t frame = Frame(base, sp, 96);
  constexpr uint32_t kLr[4] = {0x8221C190, 0x8221C1A4, 0x8221C1B8, 0x8221C1CC};
  uint64_t r3 = 0;
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t movie = Field(base, 24 + 4 * i);
    r3 = movie;
    if (movie != 0) r3 = calls.Call(kFinishAll, frame, kLr[i], movie);
  }
  return r3;
}

uint64_t Kind6HidePanels(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221CFC0 Kind6Menu::HidePanels");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t movie = Menu(base);
  constexpr uint32_t kLr[6] = {0x8221CFEC, 0x8221CFFC, 0x8221D00C,
                               0x8221D01C, 0x8221D02C, 0x8221D03C};
  uint64_t r3 = 0;
  for (uint32_t i = 0; i < 6; i++) r3 = Node(calls, kHideNode, frame, kLr[i], movie, 1, 4 + i);
  return r3;
}

uint64_t Kind6ShowPanelA(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221D050 Kind6Menu::ShowPanelA");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = Menu(base);
  Node(calls, kShowNode, frame, 0x8221D080, movie, 1, 4);
  Node(calls, kShowNode, frame, 0x8221D090, movie, 1, 5);
  const uint32_t a = ui_movies::FindNode(base, movie, 1, 4);
  const uint32_t b = ui_movies::FindNode(base, movie, 1, 5);
  if (a == 0 || b == 0) return b;
  guest::StoreU32(base, a + 108, 0);
  guest::StoreU32(base, b + 108, 0);
  constexpr uint32_t kLr[4] = {0x8221D0E4, 0x8221D0F4, 0x8221D104, 0x8221D114};
  uint64_t r3 = 0;
  for (uint32_t i = 0; i < 4; i++) r3 = Node(calls, kHideNode, frame, kLr[i], movie, 1, 6 + i);
  return r3;
}

uint64_t Kind6ShowHelp(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221D130 Kind6Menu::ShowHelp");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t movie = Menu(base);
  SetText(calls, frame, 0x8221D160, movie, 2, 0, 967);
  const double speed = double(guest::LoadFloat(base, kHelpSpeed));
  calls.Call(kScrollSpeed, frame, 0x8221D17C, movie, 2, 0, 300, &speed);
  return calls.Call(kScrollText, frame, 0x8221D190, movie, 2, 0, 1);
}

uint64_t Kind6ShowCourse(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221D1A8 Kind6Menu::ShowCourse");
  const uint32_t frame = Frame(base, sp, 240);
  const uint32_t movie = Menu(base);
  SetText(calls, frame, 0x8221D1D4, movie, 9, 55, 402);
  SetText(calls, frame, 0x8221D1E8, movie, 9, 56, 401);
  SetText(calls, frame, 0x8221D1FC, movie, 9, 57, 407);
  SetText(calls, frame, 0x8221D210, movie, 9, 58, 408);
  const uint32_t course = Get(base, kKind6Course);
  // The two tables (the course's marks) in the frame, stored as the
  // original's.
  guest::StoreU32(base, frame + 128, 25);
  constexpr uint32_t kFirst[8] = {0, 0, 2, 2, 5, 6, 3, 4};
  for (uint32_t i = 0; i < 8; i++) guest::StoreU32(base, frame + 80 + 4 * i, kFirst[i]);
  for (uint32_t i = 0; i < 4; i++) guest::StoreU32(base, frame + 112 + 4 * i, kNone);
  guest::StoreU32(base, frame + 132, 26);
  guest::StoreU32(base, frame + 136, kNone);
  guest::StoreU32(base, frame + 140, kNone);
  // Case 0 (sub_8221D2B4) and a course past 7 both take row 0.
  const uint32_t row = course <= 7 ? course : 0;
  for (uint32_t i = 0; i < 27; i++) {
    guest::StoreU32(base, ui_movies::FindNode(base, movie, 9, i + 6) + 80, 0);
  }
  const uint32_t a = guest::LoadU32(base, frame + 80 + 4 * row);
  const uint32_t first = ui_movies::FindNode(base, movie, 9, a + 6);
  const uint32_t b = guest::LoadU32(base, frame + 112 + 4 * row);
  guest::StoreU32(base, first + 80, 1);
  guest::StoreU32(base, ui_movies::FindNode(base, movie, 9, b + 6) + 80, 1);
  for (uint32_t i = 0; i < 19; i++) FinishNamed(calls, frame, 0x8221D364, movie, i + 47);
  constexpr uint32_t kRows[8] = {1, 0, 2, 3, 4, 7, 5, 6};
  for (uint32_t i = 0; i < 5; i++) guest::StoreU32(base, frame + 112 + 4 * i, kRows[i]);
  guest::StoreU32(base, frame + 140, 7);
  guest::StoreU32(base, frame + 132, 5);
  guest::StoreU32(base, frame + 136, 6);
  const uint64_t named = uint64_t(guest::LoadU32(base, frame + 112 + 4 * row)) + 47;
  FinishNamed(calls, frame, 0x8221D3AC, movie, named);
  return PlayNamed(calls, frame, 0x8221D3C8, movie, named);
}

uint64_t Kind6SetUpTexts(uint8_t* base, uint32_t screen, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221E038 Kind6Menu::SetUpTexts");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = guest::LoadU32(base, screen + 24);
  constexpr uint32_t kItems[4] = {1965, 1961, 1962, 115};
  for (uint32_t i = 0; i < 4; i++) guest::StoreU32(base, frame + 80 + 4 * i, kItems[i]);
  for (uint32_t i = 0; i < 4; i++) {
    SetText(calls, frame, 0x8221E084, movie, 3, i + 5, guest::LoadU32(base, frame + 80 + 4 * i));
  }
  for (uint32_t i = 0; i < 5; i++) {
    guest::StoreU32(base, ui_movies::FindNode(base, movie, kNone, i + 23) + 80, 0);
  }
  Kind6ShowPanelA(base, frame, calls);
  // The course's name (case 0, sub_8221E0FC, merged: 395).
  constexpr uint32_t kNames[8] = {395, 396, 397, 398, 401, 402, 404, 403};
  const uint32_t course = Get(base, kKind6Course);
  SetText(calls, frame, 0x8221E148, movie, kMinusOne, 21, course <= 7 ? kNames[course] : 0);
  SetText(calls, frame, 0x8221E170, movie, kMinusOne, 22,
          int32_t(Get(base, kKind6CourseFlag)) == 0 ? 1 : 0);
  Node(calls, kHideNode, frame, 0x8221E180, movie, kMinusOne, 17);
  Node(calls, kHideNode, frame, 0x8221E190, movie, kMinusOne, 16);
  Kind6ShowCourse(base, frame, calls);
  return Kind6ShowHelp(base, frame, calls);
}

uint32_t Kind6SetCursorX(uint8_t* base, uint64_t value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8221D7B0 Kind6Menu::SetCursorX");
  const uint32_t frame = Frame(base, sp, 112);
  const float x = ToFloat(base, frame + 80, value & 0xFFFFFFFFu);
  const uint32_t node = ui_movies::FindNode(base, Menu(base), 3, 0);
  guest::StoreFloat(base, node + 16, x);
  return node;
}

uint64_t Kind6ShowReplay(uint8_t* base, uint32_t record, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221CBD8 Kind6Menu::ShowReplay");
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t movie = Menu(base);
  if (record == 0 || guest::LoadU32(base, record + 28) == 0) {
    guest::StoreU32(base, ui_movies::FindNode(base, movie, 14, 0) + 80, 0);
    constexpr uint32_t kTexts[7][2] = {{9, 0x8221CE38},  {10, 0x8221CE48}, {19, 0x8221CE58},
                                       {20, 0x8221CE68}, {21, 0x8221CE78}, {12, 0x8221CE88},
                                       {13, 0x8221CE98}};
    uint64_t r3 = 0;
    for (const auto& t : kTexts) r3 = Node(calls, kClearText, frame, t[1], movie, 14, t[0]);
    return r3;
  }
  // The date and time: string 530 with the halfwords +0, +2, +6 and +8 in
  // r7 to r10 and +10 on the stack (the frame's +84).
  guest::StoreU32(base, frame + 84, guest::LoadU16(base, record + 10));
  SetText(calls, frame, 0x8221CC34, movie, 14, 9, 530, guest::LoadU16(base, record + 0),
          guest::LoadU16(base, record + 2), guest::LoadU16(base, record + 6),
          guest::LoadU16(base, record + 8));
  SetText(calls, frame, 0x8221CC4C, movie, 14, 10,
          uint64_t(guest::LoadU32(base, record + 20)) + 473);
  SetText(calls, frame, 0x8221CC64, movie, 14, 19,
          uint64_t(guest::LoadU32(base, record + 24)) + 1646);
  SetText(calls, frame, 0x8221CC7C, movie, 14, 20,
          uint64_t(guest::LoadU32(base, record + 24)) + 1790);
  SetText(calls, frame, 0x8221CC94, movie, 14, 21,
          uint64_t(guest::LoadU32(base, record + 24)) + 1718);
  const uint32_t time = guest::LoadU32(base, record + 16);
  SetText(calls, frame, 0x8221CCB8, movie, 14, 13, 528, (time >> 16) & 0x7FFF, (time >> 10) & 0x3F,
          time & 0x3FF);
  // The result by the kind (+20): 0 string 4650 + +32; 1 and 3 the place
  // (a table in the frame by +32, unchecked); 2 string 477; others none.
  const uint32_t kind = guest::LoadU32(base, record + 20);
  if (kind <= 3) {
    uint64_t id;
    if (kind == 1 || kind == 3) {
      constexpr uint32_t kPlaces[8] = {395, 396, 397, 398, 401, 402, 404, 403};
      for (uint32_t i = 0; i < 8; i++) guest::StoreU32(base, frame + 96 + 4 * i, kPlaces[i]);
      guest::StoreU32(base, ui_movies::FindNode(base, movie, 14, 17) + 108, 1);
      const uint32_t index = guest::LoadU32(base, record + 32);
      id = guest::LoadU32(base, (index << 2) + frame + 96);
    } else if (kind == 2) {
      guest::StoreU32(base, ui_movies::FindNode(base, movie, 14, 17) + 108, 0);
      id = 477;
    } else {
      guest::StoreU32(base, ui_movies::FindNode(base, movie, 14, 17) + 108, 0);
      id = uint64_t(guest::LoadU32(base, record + 32)) + 4650;
    }
    SetText(calls, frame, 0x8221CD98, movie, 14, 12, id);
  }
  guest::StoreU32(base, ui_movies::FindNode(base, movie, 14, 0) + 80, 1);
  const uint32_t tens = ui_movies::FindNode(base, movie, 14, 5);
  const uint32_t ones = ui_movies::FindNode(base, movie, 14, 6);
  const int32_t count = int32_t(guest::LoadU32(base, record + 28));
  guest::StoreU32(base, tens + 108, count < 10 ? 0 : uint32_t(count / 10));
  const int32_t again = int32_t(guest::LoadU32(base, record + 28));
  guest::StoreU32(base, ones + 108, uint32_t(again - (again / 10) * 10));
  return ones;
}

uint32_t Kind6ShowList(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221D4E8 Kind6Menu::ShowList");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t replays = Get(base, kKind6Replays);
  const int32_t count = int32_t(guest::LoadU32(base, replays + 20));
  const uint32_t records = guest::LoadU32(base, replays + 12);
  const uint32_t movie = Menu(base);
  const uint32_t scroll = Field(base, 140);
  const int32_t shown = count < 9 ? count : 9;
  const uint32_t node = ui_movies::FindNode(base, movie, 14, 2);
  for (uint32_t i = 0; i < 9; i++) {
    calls.Call(kClearNodeText, frame, 0x8221D558, movie, ui_movies::Child(base, node, i + 5));
  }
  uint32_t used = records + scroll * 36 + 28;
  for (int32_t i = 0; i < shown; i++, used += 36) {
    const uint32_t child = ui_movies::Child(base, node, uint32_t(i) + 5);
    const uint32_t word = guest::LoadU32(base, used);
    SetNodeText(calls, frame, word == 0 ? 0x8221D5A4 : 0x8221D5B0, movie, child,
                word == 0 ? 472 : 471, word);
  }
  // The scroll marks: none with 9 records or fewer; at the top only the
  // lower one, at the bottom only the upper one, else both.
  const uint32_t up = ui_movies::FindNode(base, movie, 14, 7);
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
  guest::StoreU32(base, up + 80, up_shown);
  const uint32_t down = ui_movies::FindNode(base, movie, 14, 8);
  guest::StoreU32(base, down + 80, down_shown);
  return down;
}

uint64_t Kind6SetUpList(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221DF70 Kind6Menu::SetUpList");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t replays = Get(base, kKind6Replays);
  const uint32_t screen = Screen(base);
  const uint32_t count = guest::LoadU32(base, replays + 20);
  const uint32_t movie = guest::LoadU32(base, screen + 24);
  const uint32_t records = guest::LoadU32(base, replays + 12);
  const uint32_t shown = int32_t(count) < 9 ? count : 9;
  guest::StoreU32(base, frame + 92, 158);
  guest::StoreU32(base, frame + 84, 159);
  const uint64_t init[8] = {uint64_t(screen) + 136, count, shown, 1, 154, 153, 155, 156};
  calls.CallArgs(kListInit, frame, 0x8221DFD8, init);
  const uint32_t selection = ListSelection(base, Screen(base) + 136);
  const uint32_t node = ui_movies::FindNode(base, movie, 14, 2);
  const float y = ToFloat(base, frame + 96, uint64_t(int64_t(int32_t(selection * 40))));
  guest::StoreFloat(base, ui_movies::Child(base, node, 0) + 16, y);
  Kind6ShowList(base, frame, calls);
  return Kind6ShowReplay(base, records + selection * 36, frame, calls);
}

// --- Page object A's transitions ---

uint64_t Kind6Intro(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221C438 Kind6Menu::Intro");
  const uint32_t frame = Frame(base, sp, 112);
  if (guest::LoadU32(base, step) == 0) {
    guest::StoreU32(base, step, 1);
    const uint32_t movie = Menu(base);
    calls.Call(kFinishAll, frame, 0x8221C46C, movie);
    PlayNamed(calls, frame, 0x8221C488, movie, 123);
    SetField(base, 336, 1);
  }
  const uint64_t done = RunDone(calls, frame, 0x8221C4A0, Menu(base));
  if (done != 0) {
    const uint32_t movie = Menu(base);
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

uint64_t Kind6Show(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221C550 Kind6Menu::Show");
  const uint32_t frame = Frame(base, sp, 112);
  if (guest::LoadU32(base, step) == 0) {
    guest::StoreU32(base, step, 1);
    const uint32_t movie = Menu(base);
    PlayNamed(calls, frame, 0x8221C5A0, movie, 7);
    LoopLast(calls, frame, 0x8221C5A8, movie);
  }
  Run(calls, frame, 0x8221C5B8, Menu(base));
  return 1;
}

uint64_t Kind6RunToEnd(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221C5D8 Kind6Menu::RunToEnd");
  const uint32_t frame = Frame(base, sp, 96);
  if (guest::LoadU32(base, step) == 0) guest::StoreU32(base, step, 1);
  return RunDone(calls, frame, 0x8221C60C, Menu(base));
}

uint64_t Kind6MoveMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221DB48 Kind6Menu::MoveMain");
  const uint32_t frame = Frame(base, sp, 128);
  if (guest::LoadU32(base, step) == 0) {
    guest::StoreU32(base, step, 1);
    const uint32_t movie = Menu(base);
    const uint32_t cursor = Field(base, 68);
    const uint32_t before = Field(base, 72);
    FinishNamed(calls, frame, 0x8221DB88, movie, 17);
    FinishNamed(calls, frame, 0x8221DB94, movie, 18);
    FinishNamed(calls, frame, 0x8221DBA0, movie, uint64_t(before) + 7);
    PlayNamed(calls, frame, 0x8221DBBC, movie, uint64_t(cursor) + 7);
    LoopLast(calls, frame, 0x8221DBC4, movie);
    // A wrap moves the cursor's marker; else the slide by the last move
    // (+76: -1 named 17, 1 named 18).
    if (int32_t(cursor) == 0 && int32_t(before) == 3) {
      Kind6SetCursorX(base, 0, frame);
    } else if (int32_t(before) == 0 && int32_t(cursor) == 3) {
      Kind6SetCursorX(base, 120, frame);
    } else if (int32_t(Field(base, 76)) == -1) {
      PlayNamed(calls, frame, 0x8221DC3C, movie, 17);
    } else if (int32_t(Field(base, 76)) == 1) {
      PlayNamed(calls, frame, 0x8221DC3C, movie, 18);
    }
    SetText(calls, frame, 0x8221DC58, movie, 2, 0, uint64_t(Field(base, 68)) + 967);
    Node(calls, kScrollReset, frame, 0x8221DC68, movie, 2, 0);
  }
  Run(calls, frame, 0x8221DC78, Menu(base));
  return 1;
}

uint64_t Kind6BackToQuit(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221DC88 Kind6Menu::BackToQuit");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = Menu(base);
  if (guest::LoadU32(base, step) == 0) {
    const uint32_t cursor = Field(base, 68);
    SetField(base, 68, 3);
    SetField(base, 72, cursor);
    const uint32_t now = Field(base, 68);
    FinishNamed(calls, frame, 0x8221DCD4, movie, 17);
    FinishNamed(calls, frame, 0x8221DCE0, movie, 18);
    for (uint32_t i = 0; i < 4; i++) FinishNamed(calls, frame, 0x8221DCF0, movie, i + 7);
    PlayNamed(calls, frame, 0x8221DD18, movie, uint64_t(now) + 7);
    LoopLast(calls, frame, 0x8221DD20, movie);
    Kind6SetCursorX(base, 120, frame);
    SetText(calls, frame, 0x8221DD44, movie, 2, 0, uint64_t(Field(base, 68)) + 967);
    Node(calls, kScrollReset, frame, 0x8221DD54, movie, 2, 0);
    guest::StoreU32(base, step, 1);
  }
  Run(calls, frame, 0x8221DD6C, Menu(base));
  return 1;
}

uint64_t Kind6OpenList(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221EB98 Kind6Menu::OpenList");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = Menu(base);
  if (guest::LoadU32(base, step) == 0) {
    guest::StoreU32(base, step, 1);
    PlayNamed(calls, frame, 0x8221EBE0, movie, 126);
    Kind6SetUpList(base, frame, calls);
  }
  const uint64_t done = RunDone(calls, frame, 0x8221EBF4, Menu(base));
  if (done != 0) {
    const uint64_t row = uint64_t(ListSelection(base, Screen(base) + 136)) + 72;
    PlayNamed(calls, frame, 0x8221EC30, movie, row);
    LoopNamed(calls, frame, 0x8221EC3C, movie, row);
  }
  return done;
}

uint64_t Kind6CloseList(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221C628 Kind6Menu::CloseList");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = Menu(base);
  if (guest::LoadU32(base, step) == 0) {
    guest::StoreU32(base, step, 1);
    const uint64_t row = uint64_t(ListSelection(base, Screen(base) + 136)) + 72;
    FinishNamed(calls, frame, 0x8221C678, movie, row);
    PlayNamed(calls, frame, 0x8221C698, Menu(base), 127);
  }
  return RunDone(calls, frame, 0x8221C6A8, Menu(base));
}

uint64_t Kind6MoveList(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221DD78 Kind6Menu::MoveList");
  const uint32_t frame = Frame(base, sp, 176);
  if (guest::LoadU32(base, step) == 0) {
    guest::StoreU32(base, step, 1);
    const uint32_t list = Screen(base) + 136;
    const uint32_t movie = Menu(base);
    const uint32_t selection = ListSelection(base, list);
    // MenuList::GetPreviousSelection (sub_8220BC28): the list's +20.
    const uint32_t previous = guest::LoadU32(base, list + kListSelectionBefore);
    const uint32_t scroll = guest::LoadU32(base, list + kListScroll);
    const uint32_t before = guest::LoadU32(base, list + kListScrollBefore);
    const uint32_t wrapped = guest::LoadU32(base, list + kListWrapped);
    const int32_t scrolled = int32_t(scroll - before);
    const uint32_t records = guest::LoadU32(base, Get(base, kKind6Replays) + 12);
    FinishNamed(calls, frame, 0x8221DDEC, movie, uint64_t(previous) + 72);
    const uint64_t row = uint64_t(selection) + 72;
    PlayNamed(calls, frame, 0x8221DE0C, movie, row);
    LoopNamed(calls, frame, 0x8221DE18, movie, row);
    Kind6ShowReplay(base, records + (scroll + selection) * 36, frame, calls);
    FinishNamed(calls, frame, 0x8221DE34, movie, 82);
    FinishNamed(calls, frame, 0x8221DE40, movie, 83);
    const uint32_t node = ui_movies::FindNode(base, movie, 14, 2);
    if (int32_t(wrapped) != 0) {
      const float y = ToFloat(base, frame + 80, uint64_t(int64_t(int32_t(selection * 40))));
      guest::StoreFloat(base, ui_movies::Child(base, node, 0) + 16, y);
    } else {
      // A row scrolled in: the slot that comes in shown (its record's +28)
      // and the slide played (82 up, 83 down).
      const int32_t move = int32_t(guest::LoadU32(base, list + kListMove));
      if (move < 0) {
        const uint32_t child = ui_movies::Child(base, node, 15);
        SetNodeText(calls, frame, 0x8221DEB4, movie, child, 471,
                    guest::LoadU32(base, records + scroll * 36 + 28));
        PlayNamed(calls, frame, 0x8221DF00, movie, 82);
      } else if (move > 0) {
        const uint32_t child = ui_movies::Child(base, node, 14);
        SetNodeText(calls, frame, 0x8221DEE4, movie, child, 471,
                    guest::LoadU32(base, records + scroll * 36 + 352));
        PlayNamed(calls, frame, 0x8221DF00, movie, 83);
      }
    }
    if (scrolled != 0) {
      FinishNamed(calls, frame, 0x8221DF14, movie, 95);
      FinishNamed(calls, frame, 0x8221DF20, movie, 94);
      Kind6ShowList(base, frame, calls);
      PlayNamed(calls, frame, 0x8221DF4C, movie, scrolled > 0 ? 95 : 94);
    }
  }
  Run(calls, frame, 0x8221DF5C, Menu(base));
  return 1;
}

// --- Page object B's transitions ---

uint64_t Kind6OpenQuestion(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221C6C8 Kind6Menu::OpenQuestion");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = Field(base, 28);
  if (guest::LoadU32(base, step) == 0) {
    SetText(calls, frame, 0x8221C704, movie, 1, 3, 2);
    SetText(calls, frame, 0x8221C718, movie, 1, 4, 3);
    SetText(calls, frame, 0x8221C730, movie, kMinusOne, 0, Field(base, 324));
    PlayNamed(calls, frame, 0x8221C74C, movie, 5);
    SetField(base, 340, 0);
    SetField(base, 348, 0);
    MenuSound(calls, frame, 0x8221C768);
    guest::StoreU32(base, step, 1);
  }
  const uint64_t done = RunDone(calls, frame, 0x8221C77C, movie);
  if (done != 0) {
    FinishNamed(calls, frame, 0x8221C794, movie, 1);
    FinishNamed(calls, frame, 0x8221C7A0, movie, 2);
    PlayNamed(calls, frame, 0x8221C7C4, movie, uint64_t(Field(base, 236)) + 1);
    LoopLast(calls, frame, 0x8221C7CC, movie);
  }
  return done;
}

uint64_t Kind6CloseQuestion(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221C7D8 Kind6Menu::CloseQuestion");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = Field(base, 28);
  if (guest::LoadU32(base, step) == 0) {
    FinishNamed(calls, frame, 0x8221C810, movie, 1);
    FinishNamed(calls, frame, 0x8221C81C, movie, 2);
    PlayNamed(calls, frame, 0x8221C838, movie, 6);
    MenuSound(calls, frame, 0x8221C840);
    guest::StoreU32(base, step, 1);
  }
  const uint64_t done = RunDone(calls, frame, 0x8221C850, movie);
  if (done != 0) {
    SetField(base, 348, 3);
    SetField(base, 340, 1);
    SetField(base, 356, 0);
  }
  return done;
}

uint64_t Kind6MoveQuestion(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221C888 Kind6Menu::MoveQuestion");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = Field(base, 28);
  if (guest::LoadU32(base, step) == 0) {
    FinishNamed(calls, frame, 0x8221C8BC, movie, 1);
    FinishNamed(calls, frame, 0x8221C8C8, movie, 2);
    PlayNamed(calls, frame, 0x8221C8EC, movie, uint64_t(Field(base, 236)) + 1);
    LoopNamed(calls, frame, 0x8221C900, movie, uint64_t(Field(base, 236)) + 1);
    guest::StoreU32(base, step, 1);
  }
  Run(calls, frame, 0x8221C914, movie);
  return 1;
}

uint64_t Kind6OpenMessage(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221CAB8 Kind6Menu::OpenMessage");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t screen = Screen(base);
  const uint32_t movie = guest::LoadU32(base, screen + 32);
  if (guest::LoadU32(base, step) == 0) {
    SetText(calls, frame, 0x8221CAF4, movie, kMinusOne, 1, guest::LoadU32(base, screen + 324));
    PlayNamed(calls, frame, 0x8221CB10, movie, 2);
    SetField(base, 340, 0);
    SetField(base, 348, 1);
    MenuSound(calls, frame, 0x8221CB30);
    guest::StoreU32(base, step, 1);
  }
  return RunDone(calls, frame, 0x8221CB40, movie);
}

uint64_t Kind6CloseMessage(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221CB50 Kind6Menu::CloseMessage");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = Field(base, 32);
  if (guest::LoadU32(base, step) == 0) {
    PlayNamed(calls, frame, 0x8221CB98, movie, 3);
    MenuSound(calls, frame, 0x8221CBA0);
    guest::StoreU32(base, step, 1);
  }
  const uint64_t done = RunDone(calls, frame, 0x8221CBB0, movie);
  if (done != 0) {
    SetField(base, 348, 3);
    SetField(base, 340, 1);
  }
  return done;
}

uint64_t Kind6OpenWaiting(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221C920 Kind6Menu::OpenWaiting");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = Field(base, 36);
  if (guest::LoadU32(base, step) == 0) {
    const uint32_t node = ui_movies::FindNode(base, movie, 0, 2);
    const uint32_t text = ui_movies::Child(base, node, 1);
    SetNodeText(calls, frame, 0x8221C978, movie, text, Field(base, 324));
    guest::StoreU32(base, ui_movies::Child(base, node, 0) + 80, 0);
    PlayNamed(calls, frame, 0x8221C9A8, movie, 13);
    FinishNamed(calls, frame, 0x8221C9B4, movie, 0);
    PlayNamed(calls, frame, 0x8221C9D0, movie, 0);
    LoopNamed(calls, frame, 0x8221C9DC, movie, 0);
    SetField(base, 348, 2);
    SetField(base, 340, 0);
    MenuSound(calls, frame, 0x8221C9F8);
    guest::StoreU32(base, step, 1);
  }
  return RunDone(calls, frame, 0x8221CA0C, movie);
}

uint64_t Kind6CloseWaiting(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221CA20 Kind6Menu::CloseWaiting");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = Field(base, 36);
  if (guest::LoadU32(base, step) == 0) {
    PlayNamed(calls, frame, 0x8221CA68, movie, 15);
    MenuSound(calls, frame, 0x8221CA70);
    guest::StoreU32(base, step, 1);
  }
  const uint64_t done = RunDone(calls, frame, 0x8221CA80, movie);
  if (done != 0) {
    SetField(base, 348, 3);
    SetField(base, 340, 1);
    FinishNamed(calls, frame, 0x8221CAAC, movie, 0);
  }
  return done;
}

// --- The opener, draw and close ---

uint64_t Kind6Open(uint8_t* base, uint32_t screen, uint32_t block, uint32_t sp,
                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221EC48 Kind6Menu::Open");
  const uint32_t frame = Frame(base, sp, 160);
  if (!c_runtime::Memcpy(base, screen, block, 24)) {
    calls.Call(kMemcpy, frame, 0x8221EC60, screen, block, 24);
  }
  // Past 59 minutes: 59:59.999 (the sign bit kept).
  const uint32_t time = guest::LoadU32(base, screen + 4);
  if ((time & 0x7FFF0000u) >= 0x3C0000u) {
    guest::StoreU32(base, screen + 4, (time & 0x80000000u) | 0x3BEFE7u);
  }
  constexpr uint32_t kMovies[4][2] = {{17, 24}, {20, 28}, {21, 32}, {14, 36}};
  for (const auto& m : kMovies) {
    guest::StoreU32(base, frame + 96, m[0]);
    const uint32_t found = ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 96);
    guest::StoreU32(base, screen + m[1], guest::LoadU32(base, found));
  }
  auto answered = [&] { return Get(base, kKind6Word) <= 2; };
  if (answered()) {
    constexpr uint32_t kLr[4] = {0x8221ED30, 0x8221ED38, 0x8221ED40, 0x8221ED48};
    for (uint32_t i = 0; i < 4; i++) {
      calls.Call(kResetPose, frame, kLr[i], guest::LoadU32(base, screen + 24 + 4 * i));
    }
  }
  const uint32_t a = screen + 40, b = screen + 208;
  PagesInit(base, a, 0);
  PagesInit(base, b, 0);
  SetField(base, 336, 0);
  if (answered()) {
    guest::StoreU32(base, screen + 324, 44);
    PagesGo(base, a, 12, 0, 1);
    PagesGo(base, b, 5, 5, kNone);
  } else {
    PagesGo(base, a, 0, kNone, kNone);
  }
  for (const uint32_t field : {80u, 68u, 72u, 76u}) guest::StoreU32(base, screen + field, 0);
  guest::StoreU32(base, frame + 84, 159);
  guest::StoreU32(base, frame + 92, 158);
  guest::StoreU32(base, screen + 84, 3);
  guest::StoreU32(base, screen + 88, 1);
  for (const uint32_t field : {108u, 116u, 120u, 104u, 132u, 92u, 96u, 100u}) {
    guest::StoreU32(base, screen + field, 0);
  }
  const uint64_t init[8] = {uint64_t(screen) + 136, 4, 4, 1, 154, 153, 155, 156};
  calls.CallArgs(kListInit, frame, 0x8221EE48, init);
  guest::StoreU32(base, screen + 248, 0);
  guest::StoreU32(base, screen + 252, 1);
  guest::StoreU32(base, screen + 256, 1);
  for (const uint32_t field :
       {236u, 240u, 244u, 276u, 284u, 288u, 272u, 300u, 260u, 264u, 268u}) {
    guest::StoreU32(base, screen + field, 0);
  }
  if (answered()) {
    // The race's time in milliseconds for the ranking.
    const uint32_t t = guest::LoadU32(base, screen + 4);
    const uint64_t minutes =
        uint64_t(int64_t(int32_t((t >> 16) & 0x7FFF)) * 60000) & 0xFFFFFFFFu;
    const uint64_t seconds = (uint64_t((t >> 10) & 0x3F) * 1000) & 0xFFFFFFFFu;
    StoreU64(base, Get(base, kKind6Session) + 200, seconds + minutes + (t & 0x3FF));
  }
  if (answered()) {
    for (const uint32_t field : {304u, 308u, 312u, 316u, 320u, 344u, 356u, 328u, 332u}) {
      guest::StoreU32(base, screen + field, 0);
    }
    guest::StoreU32(base, screen + 348, 3);
    guest::StoreU32(base, screen + 352, 0);
  }
  guest::StoreU32(base, screen + 340, 1);
  uint64_t r3 = Kind6SetUpTexts(base, screen, frame, calls);
  if (answered()) r3 = Kind6HidePanels(base, frame, calls);
  return r3;
}

uint64_t Kind6Draw(uint8_t* base, uint32_t screen, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221EF60 Kind6Menu::Draw");
  const uint32_t frame = Frame(base, sp, 144);
  // A's: Intro, Show, RunToEnd, MoveMain, OpenList, CloseList, MoveList,
  // BackToQuit.
  constexpr uint32_t kA[8] = {0x8221C438, 0x8221C550, 0x8221C5D8, 0x8221DB48,
                              0x8221EB98, 0x8221C628, 0x8221DD78, 0x8221DC88};
  for (uint32_t i = 0; i < 8; i++) guest::StoreU32(base, frame + 80 + 4 * i, kA[i]);
  const int32_t transition = int32_t(guest::LoadU32(base, screen + 52));
  uint64_t r3;
  if (transition >= 0) {
    const uint32_t step = screen + 64;
    uint64_t result;
    switch (transition) {
      case 0: result = Kind6Intro(base, step, frame, calls); break;
      case 1: result = Kind6Show(base, step, frame, calls); break;
      case 2: result = Kind6RunToEnd(base, step, frame, calls); break;
      case 3: result = Kind6MoveMain(base, step, frame, calls); break;
      case 4: result = Kind6OpenList(base, step, frame, calls); break;
      case 5: result = Kind6CloseList(base, step, frame, calls); break;
      case 6: result = Kind6MoveList(base, step, frame, calls); break;
      case 7: result = Kind6BackToQuit(base, step, frame, calls); break;
      default: {
        // Past the table: the frame's word, as the original's bctrl.
        const uint32_t handler =
            guest::LoadU32(base, (uint32_t(transition) << 2) + frame + 80);
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
    PagesFinish(base, screen + 40, uint32_t(result));
    r3 = uint64_t(screen) + 40;
  } else if (Field(base, 336) != 0) {
    r3 = Run(calls, frame, 0x8221F03C, guest::LoadU32(base, screen + 24));
  } else {
    r3 = Register(kA[0]);
  }
  const int32_t page = int32_t(guest::LoadU32(base, screen + 40));
  if ((page >= 3 && page <= 5) || page == 13) r3 = RaceDialogDraw(base, frame, calls);
  // B's (the eighth word, BackToQuit, left from A's table): OpenQuestion,
  // CloseQuestion, OpenMessage, CloseMessage, MoveQuestion, OpenWaiting,
  // CloseWaiting.
  constexpr uint32_t kB[7] = {0x8221C6C8, 0x8221C7D8, 0x8221CAB8, 0x8221CB50,
                              0x8221C888, 0x8221C920, 0x8221CA20};
  for (uint32_t i = 0; i < 7; i++) guest::StoreU32(base, frame + 80 + 4 * i, kB[i]);
  const int32_t b_transition = int32_t(guest::LoadU32(base, screen + 220));
  if (b_transition != -1) {
    uint64_t result = 1;
    if (b_transition >= 0) {
      const uint32_t step = screen + 232;
      switch (b_transition) {
        case 0: result = Kind6OpenQuestion(base, step, frame, calls); break;
        case 1: result = Kind6CloseQuestion(base, step, frame, calls); break;
        case 2: result = Kind6OpenMessage(base, step, frame, calls); break;
        case 3: result = Kind6CloseMessage(base, step, frame, calls); break;
        case 4: result = Kind6MoveQuestion(base, step, frame, calls); break;
        case 5: result = Kind6OpenWaiting(base, step, frame, calls); break;
        case 6: result = Kind6CloseWaiting(base, step, frame, calls); break;
        case 7: result = Kind6BackToQuit(base, step, frame, calls); break;
        default: {
          const uint32_t handler =
              guest::LoadU32(base, (uint32_t(b_transition) << 2) + frame + 80);
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
    PagesFinish(base, screen + 208, uint32_t(result));
    return uint64_t(screen) + 208;
  }
  if (Field(base, 336) != 0) {
    const int32_t b_page = int32_t(guest::LoadU32(base, screen + 208));
    uint32_t field = 0;
    if (b_page > 0 && b_page <= 2) {
      field = 28;
    } else if (b_page > 2 && b_page <= 4) {
      field = 32;
    } else if (b_page == 5) {
      field = 36;
    }
    if (field != 0) r3 = Run(calls, frame, 0x8221F144, guest::LoadU32(base, screen + field));
  }
  if (Field(base, 344) != 0) {
    guest::StoreU32(base, Get(base, kKind6Session) + 236, 3);
    SetField(base, 344, 0);
    const uint32_t k = Screen(base);
    const uint32_t state = guest::LoadU32(base, k + 348);
    if (state <= 2) {
      constexpr uint32_t kTransitions[3] = {1, 3, 6};
      PagesGo(base, k + 208, 0, kTransitions[state], kNone);
      r3 = uint64_t(k) + 208;
    }
    if (state <= 3) {
      const uint32_t again = Screen(base);
      PagesGo(base, again + 40, 11, kNone, kNone);
      r3 = uint64_t(again) + 40;
    }
  }
  return r3;
}

}  // namespace race_screens
