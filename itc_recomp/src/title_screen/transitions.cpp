// The title screen's transitions and the menus' set-ups
// (docs/functions/title-screen.md): see title_screen.h. Each transition
// finds movie 0 (its id at its frame's +80), sets itself up once (its
// started word, the page object's +24, 0), then runs the movie.

#include "base/log.h"
#include "race_screens/race_screens.h"
#include "title_screen/internal.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace title_screen {

namespace {

constexpr uint32_t kLoopNamed = 0x8211DDE0;
constexpr uint32_t kLoopLast = 0x8211DDE8;
constexpr uint32_t kFinishNamed = 0x8211DF20;
constexpr uint32_t kFindNode = 0x8211A578;
constexpr uint32_t kSetNodeColor = 0x8211A5B8;
constexpr uint32_t kListInit = 0x8220BAC0;
constexpr uint32_t kSlotsState = 0x8228F9E8;
constexpr uint32_t kPadIsConnected = 0x8209F340;
// The players' mapper entries (8 bytes each) and player 1's controller.
constexpr uint32_t kMapperEntries = 0x826BAB80;
constexpr uint32_t kPlayer1Controller = 0x826BABA8;
// The cursor's positions MoveMenu4Cursor copies when the menu wraps.
constexpr uint32_t kMenu4WrapUp = 0x82001FB0;
constexpr uint32_t kMenu4WrapDown = 0x82000A34;

uint64_t Play(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t id,
              uint32_t r5 = 0, uint32_t r6 = 0) {
  return calls.CallArgs(kPlayNamed, frame, lr, {movie, id, r5, r6, 0, 0, 0, 0});
}
uint64_t Finish(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t id) {
  return calls.Call(kFinishNamed, frame, lr, movie, id);
}
uint64_t RunMovie(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie) {
  return calls.Call(kRun, frame, lr, movie, 0);
}
// 1 when Run answered 0.
uint64_t Done(uint64_t run) { return uint32_t(run) == 0 ? 1 : 0; }

uint32_t List(uint8_t* base) { return Load(base, kList); }

// The cursor node's +16 = n × 40 as a float, through the original's frame
// doubleword at `slot` (the integer stored there for the conversion).
void StoreCursor(uint8_t* base, uint32_t slot, uint32_t node, uint32_t n) {
  const int32_t value = int32_t(n * 40u);
  const uint64_t wide = uint64_t(int64_t(value));
  Store(base, slot, uint32_t(wide >> 32));
  Store(base, slot + 4, uint32_t(wide));
  guest::StoreFloat(base, node + 16, float(double(int64_t(value))));
}

// The page object to page 2 through transition 6 (BackFromCareer's and
// BackFromMenu4's end), +24 = 0.
void BackToMainMenu(uint8_t* base) {
  const uint32_t pages = Load(base, kPages);
  const uint32_t page = Load(base, pages + 0);
  if (page != 2) {
    Store(base, pages + 8, page);
  }
  Store(base, pages + 16, kNone);
  Store(base, pages + 12, 6);
  Store(base, pages + 4, 2);
  Store(base, pages + 0, kNone);
  Store(base, Load(base, kPages) + 24, 0);
}

// ShowMainMenu's and ShowMainMenuAtOnce's end: item 5 shown refused when
// SaveSlots::State(0) answers 0.
void MarkSaveItem(uint8_t* base, uint32_t frame, uint32_t movie, uint32_t lr_state,
                  uint32_t lr_color, const Calls& calls) {
  const uint64_t state = calls.Call(kSlotsState, frame, lr_state, 0);
  uint32_t color = 0;
  if (uint32_t(state) != 0) {
    Store(base, Words(base) + 28, 0);
  } else {
    color = 2;
    Store(base, Words(base) + 28, 1);
  }
  calls.Call(kSetNodeColor, frame, lr_color, movie, 4, 10, color);
}

}  // namespace

uint64_t SetUpMainMenu(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223700 TitleScreen::SetUpMainMenu");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 96);
  Store(base, frame + 92, 158);
  Store(base, frame + 84, 159);
  calls.CallArgs(kListInit, frame, 0x82223764, {List(base), 7, 7, 1, 154, 153, 155, 156});
  race_screens::ListSetSelection(base, List(base), Load(base, Words(base) + 4));
  const uint32_t node = uint32_t(calls.Call(kFindNode, frame, 0x82223788, movie, 4, 0));
  StoreCursor(base, frame + 96, node, race_screens::ListSelection(base, List(base)));
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  Store(base, Words(base) + 12, (Load(base, save + 6092) >> 5) & 1);
  int32_t day = int32_t(Load(base, save + 6100));
  if (day > 999) {
    day = 999;
  }
  const bool can_continue = day != 0 && guest::LoadU16(base, save + 6328) != 0;
  Store(base, Words(base) + 8, can_continue ? 1 : 0);
  if (Load(base, Words(base) + 40) == 0) {
    for (uint32_t k = 0; k < 4; k++) {
      Store(base, kSystemChecks + 4 * k, 0);
    }
  }
  return save;
}

uint64_t SetUpCareerMenu(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223840 TitleScreen::SetUpCareerMenu");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 96);
  const uint32_t third = Load(base, Words(base) + 12);
  Store(base, frame + 92, 158);
  Store(base, frame + 84, 159);
  const uint32_t n = third != 0 ? 3 : 2;
  calls.CallArgs(kListInit, frame, 0x822238C0, {List(base), n, n, 1, 154, 153, 155, 156});
  race_screens::ListSetSelection(base, List(base), Load(base, Words(base) + 20));
  const uint32_t node = uint32_t(calls.Call(kFindNode, frame, 0x822238E0, movie, 5, 0));
  StoreCursor(base, frame + 96, node, race_screens::ListSelection(base, List(base)));
  uint64_t r3 = 0;
  for (uint32_t k = 5; k < 5 + n; k++) {
    // r7 to r10 unset in the original: 0 (the user's call, 2026-10-30).
    r3 = calls.Call(kSetText, frame, 0x82223928, movie, 5, k, k + 249);
  }
  return r3;
}

uint64_t SetUpMenu4(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223940 TitleScreen::SetUpMenu4");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 80);
  const uint32_t menu = Load(base, kMenu);
  Store(base, menu + 12, 0);
  Store(base, menu + 16, 1);
  Store(base, menu + 20, 1);
  for (const uint32_t field : {0u, 4u, 8u, 40u, 48u, 52u, 36u, 64u, 24u, 28u, 32u}) {
    Store(base, menu + field, 0);
  }
  const uint32_t node = uint32_t(calls.Call(kFindNode, frame, 0x822239BC, movie, 5, 0));
  StoreCursor(base, frame + 80, node, Load(base, Load(base, kMenu) + 0));
  calls.Call(kSetText, frame, 0x822239F8, movie, 5, 5, 935);
  return calls.Call(kSetText, frame, 0x82223A0C, movie, 5, 6, 936);
}

uint64_t Intro(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82222B60 TitleScreen::Intro");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 80);
  if (Load(base, started) == 0) {
    calls.Call(kResetPlayer, frame, 0x82222BA4);
    Play(calls, frame, 0x82222BC0, movie, 82);
    Store(base, started, 1);
  }
  return Done(RunMovie(calls, frame, 0x82222BD4, movie));
}

uint64_t PressStartLoop(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82222BF8 TitleScreen::PressStartLoop");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 80);
  if (Load(base, started) == 0) {
    Play(calls, frame, 0x82222C54, movie, 81);
    calls.Call(kLoopNamed, frame, 0x82222C60, movie, 81);
    Store(base, started, 1);
  }
  RunMovie(calls, frame, 0x82222C74, movie);
  return 1;
}

uint64_t BackToPressStart(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82222C90 TitleScreen::Transition2");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 80);
  if (Load(base, started) == 0) {
    const uint32_t s = race_screens::ListSelection(base, List(base));
    Finish(calls, frame, 0x82222CEC, movie, uint64_t(s) + 2);
    calls.Call(kResetPlayer, frame, 0x82222CF0);
    Play(calls, frame, 0x82222D0C, movie, 85);
    Store(base, started, 1);
  }
  return Done(RunMovie(calls, frame, 0x82222D20, movie));
}

uint64_t LeavePressStart(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82222D40 TitleScreen::LeavePressStart");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 80);
  if (Load(base, started) == 0) {
    Play(calls, frame, 0x82222D9C, movie, 83);
    Store(base, started, 1);
  }
  return Done(RunMovie(calls, frame, 0x82222DB0, movie));
}

uint64_t ShowMainMenu(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223B78 TitleScreen::ShowMainMenu");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 80);
  if (Load(base, started) == 0) {
    SetUpMainMenu(base, frame, calls);
    Play(calls, frame, 0x82223BD4, movie, 84);
    Store(base, started, 1);
  }
  const uint64_t done = Done(RunMovie(calls, frame, 0x82223BE4, movie));
  if (done != 0) {
    MarkSaveItem(base, frame, movie, 0x82223BF8, 0x82223C2C, calls);
  }
  return done;
}

uint64_t ShowMainMenuAtOnce(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223C38 TitleScreen::ShowMainMenuAtOnce");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 80);
  if (Load(base, started) == 0) {
    Play(calls, frame, 0x82223C90, movie, 82);
    Finish(calls, frame, 0x82223C9C, movie, 82);
    Play(calls, frame, 0x82223CB8, movie, 83);
    Finish(calls, frame, 0x82223CC4, movie, 83);
    Play(calls, frame, 0x82223CE0, movie, 81);
    calls.Call(kLoopNamed, frame, 0x82223CEC, movie, 81);
    SetUpMainMenu(base, frame, calls);
    Play(calls, frame, 0x82223D0C, movie, 84);
    Store(base, started, 1);
  }
  const uint64_t done = Done(RunMovie(calls, frame, 0x82223D1C, movie));
  if (done != 0) {
    MarkSaveItem(base, frame, movie, 0x82223D30, 0x82223D64, calls);
  }
  return done;
}

uint64_t HighlightMain(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82222DD0 TitleScreen::HighlightMain");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 80);
  if (Load(base, started) == 0) {
    const uint32_t words = Words(base);
    const uint32_t item = Load(base, words + 4);
    if (item != 5 || Load(base, words + 28) != 0) {
      Play(calls, frame, 0x82222E4C, movie, uint64_t(item) + 2);
      calls.Call(kLoopLast, frame, 0x82222E54, movie);
    }
    Store(base, started, 1);
  }
  RunMovie(calls, frame, 0x82222E68, movie);
  return 1;
}

uint64_t MoveMainCursor(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82222E88 TitleScreen::MoveMainCursor");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 80);
  if (Load(base, started) == 0) {
    const uint32_t s = race_screens::ListSelection(base, List(base));
    const uint32_t p = Load(base, List(base) + 20);
    Finish(calls, frame, 0x82222EEC, movie, uint64_t(p) + 2);
    if (s == 5 && Load(base, Words(base) + 28) != 0) {
      Finish(calls, frame, 0x82222F14, movie, 7);
      Play(calls, frame, 0x82222F30, movie, 26);
      Finish(calls, frame, 0x82222F3C, movie, 26);
      Play(calls, frame, 0x82222F58, movie, 27, 1, 1);
      Finish(calls, frame, 0x82222F64, movie, 27);
    } else {
      Play(calls, frame, 0x82222F88, movie, uint64_t(s) + 2);
      calls.Call(kLoopNamed, frame, 0x82222F94, movie, uint64_t(s) + 2);
    }
    Finish(calls, frame, 0x82222FA0, movie, 12);
    Finish(calls, frame, 0x82222FAC, movie, 13);
    if ((s == 0 && p == 6) || (p == 0 && s == 6)) {
      const uint32_t node = uint32_t(calls.Call(kFindNode, frame, 0x82222FF4, movie, 4, 0));
      StoreCursor(base, frame + 80, node, s);
    } else {
      const uint32_t up = int32_t(Load(base, List(base) + 24)) == -1 ? 12 : 13;
      Play(calls, frame, 0x8222302C, movie, up, 1);
    }
    Store(base, started, 1);
  }
  RunMovie(calls, frame, 0x82223040, movie);
  return 1;
}

uint64_t RefreshSaveItem(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223050 TitleScreen::Transition9");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 80);
  if (Load(base, started) == 0) {
    const uint32_t s = race_screens::ListSelection(base, List(base));
    const uint32_t p = Load(base, List(base) + 20);
    const uint32_t state = uint32_t(calls.Call(kSlotsState, frame, 0x822230AC, 0));
    Finish(calls, frame, 0x822230BC, movie, uint64_t(p) + 2);
    uint32_t color = 2;
    if (s == 5 && state == 0) {
      Finish(calls, frame, 0x822230DC, movie, 7);
      Play(calls, frame, 0x822230F8, movie, 26);
      Finish(calls, frame, 0x82223104, movie, 26);
      Play(calls, frame, 0x82223120, movie, 27, 1, 1);
      Finish(calls, frame, 0x8222312C, movie, 27);
      Store(base, Words(base) + 28, 1);
    } else {
      Play(calls, frame, 0x82223188, movie, uint64_t(s) + 2);
      calls.Call(kLoopNamed, frame, 0x82223194, movie, uint64_t(s) + 2);
      if (state == 0) {
        Store(base, Words(base) + 28, 1);
      } else {
        color = 0;
        Store(base, Words(base) + 28, 0);
      }
    }
    calls.Call(kSetNodeColor, frame, 0x8222314C, movie, 4, 10, color);
    Store(base, started, 1);
  }
  RunMovie(calls, frame, 0x8222315C, movie);
  return 1;
}

uint64_t RefreshPadItem(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822231B0 TitleScreen::Transition10");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 80);
  if (Load(base, started) == 0) {
    const uint32_t s = race_screens::ListSelection(base, List(base));
    const uint32_t p = Load(base, List(base) + 20);
    const uint32_t entry = kMapperEntries + (Load(base, kPlayer1Controller) << 3);
    const uint32_t connected = uint32_t(calls.Call(kPadIsConnected, frame, 0x82223224, entry));
    Finish(calls, frame, 0x82223234, movie, uint64_t(p) + 2);
    uint32_t color = 2;
    if (s == 4 && connected == 0) {
      Finish(calls, frame, 0x82223254, movie, 6);
      Play(calls, frame, 0x82223270, movie, 26);
      Finish(calls, frame, 0x8222327C, movie, 26);
      Play(calls, frame, 0x82223298, movie, 27, 1, 1);
      Finish(calls, frame, 0x822232A4, movie, 27);
      Store(base, Words(base) + 32, 0);
    } else {
      Play(calls, frame, 0x82223300, movie, uint64_t(s) + 2);
      calls.Call(kLoopNamed, frame, 0x8222330C, movie, uint64_t(s) + 2);
      if (connected == 0) {
        Store(base, Words(base) + 32, 0);
      } else {
        color = 0;
        Store(base, Words(base) + 32, 1);
      }
    }
    calls.Call(kSetNodeColor, frame, 0x822232C4, movie, 4, 9, color);
    Store(base, started, 1);
  }
  RunMovie(calls, frame, 0x822232D4, movie);
  return 1;
}

uint64_t ShowCareerMenu(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223D70 TitleScreen::ShowCareerMenu");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 80);
  if (Load(base, started) == 0) {
    const uint32_t s = race_screens::ListSelection(base, List(base));
    Finish(calls, frame, 0x82223DC8, movie, uint64_t(s) + 2);
    Play(calls, frame, 0x82223DE4, movie, 86);
    const uint32_t words = Words(base);
    Store(base, words + 20, Load(base, words + 8) != 0 ? 1 : 0);
    SetUpCareerMenu(base, frame, calls);
    Store(base, started, 1);
  }
  const uint64_t done = Done(RunMovie(calls, frame, 0x82223E18, movie));
  if (done != 0 && Load(base, Words(base) + 8) == 0) {
    calls.Call(kSetNodeColor, frame, 0x82223E48, movie, 5, 6, 2);
  }
  return done;
}

namespace {

// HighlightCareer's and MoveCareerCursor's highlight of item `s` (continue,
// 1, refused when the title's +8 is 0).
void HighlightCareerItem(uint8_t* base, uint32_t frame, uint32_t movie, uint32_t s,
                         const uint32_t (&lrs)[6], const Calls& calls) {
  if (Load(base, Words(base) + 8) == 0 && s == 1) {
    Play(calls, frame, lrs[0], movie, 65);
    Finish(calls, frame, lrs[1], movie, 65);
    Play(calls, frame, lrs[2], movie, 66, 1, 1);
    Finish(calls, frame, lrs[3], movie, 66);
  } else {
    Play(calls, frame, lrs[4], movie, uint64_t(s) + 41);
    calls.Call(kLoopNamed, frame, lrs[5], movie, uint64_t(s) + 41);
  }
}

}  // namespace

uint64_t HighlightCareer(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223328 TitleScreen::HighlightCareer");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 80);
  if (Load(base, started) == 0) {
    const uint32_t s = race_screens::ListSelection(base, List(base));
    HighlightCareerItem(base, frame, movie, s,
                        {0x822233A4, 0x822233B0, 0x822233CC, 0x822233D8, 0x822233FC, 0x82223408},
                        calls);
    Store(base, started, 1);
  }
  RunMovie(calls, frame, 0x8222341C, movie);
  return 1;
}

uint64_t MoveCareerCursor(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223428 TitleScreen::MoveCareerCursor");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 80);
  if (Load(base, started) == 0) {
    const uint32_t s = race_screens::ListSelection(base, List(base));
    const uint32_t p = Load(base, List(base) + 20);
    Finish(calls, frame, 0x8222348C, movie, uint64_t(p) + 41);
    HighlightCareerItem(base, frame, movie, s,
                        {0x822234C4, 0x822234D0, 0x822234EC, 0x822234F8, 0x8222351C, 0x82223528},
                        calls);
    Finish(calls, frame, 0x82223534, movie, 51);
    Finish(calls, frame, 0x82223540, movie, 52);
    const uint32_t list = List(base);
    if (Load(base, list + 32) != 0) {
      const uint32_t node = uint32_t(calls.Call(kFindNode, frame, 0x82223578, movie, 5, 0));
      StoreCursor(base, frame + 80, node, s);
    } else {
      const uint32_t up = int32_t(Load(base, list + 24)) == -1 ? 51 : 52;
      Play(calls, frame, 0x822235A8, movie, up, 1);
    }
    Store(base, started, 1);
  }
  RunMovie(calls, frame, 0x822235BC, movie);
  return 1;
}

uint64_t BackFromCareer(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223E58 TitleScreen::BackFromCareer");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 80);
  if (Load(base, started) == 0) {
    const uint32_t s = race_screens::ListSelection(base, List(base));
    Finish(calls, frame, 0x82223EAC, movie, uint64_t(s) + 41);
    Play(calls, frame, 0x82223EC8, movie, 87);
    SetUpMainMenu(base, frame, calls);
    Store(base, started, 1);
  }
  if (uint32_t(RunMovie(calls, frame, 0x82223EE0, movie)) == 0) {
    BackToMainMenu(base);
  }
  return 0;
}

uint64_t ShowMenu4(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223F30 TitleScreen::ShowMenu4");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 80);
  if (Load(base, started) == 0) {
    const uint32_t s = race_screens::ListSelection(base, List(base));
    Finish(calls, frame, 0x82223F84, movie, uint64_t(s) + 2);
    Play(calls, frame, 0x82223FA0, movie, 86);
    Store(base, Words(base) + 24, 0);
    SetUpMenu4(base, frame, calls);
    Store(base, started, 1);
  }
  return Done(RunMovie(calls, frame, 0x82223FC4, movie));
}

uint64_t BackFromMenu4(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223FD8 TitleScreen::BackFromMenu4");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 80);
  if (Load(base, started) == 0) {
    Finish(calls, frame, 0x82224028, movie, uint64_t(Load(base, Load(base, kMenu) + 4)) + 41);
    Play(calls, frame, 0x82224044, movie, 87);
    SetUpMainMenu(base, frame, calls);
    Store(base, started, 1);
  }
  if (uint32_t(RunMovie(calls, frame, 0x8222405C, movie)) == 0) {
    BackToMainMenu(base);
  }
  return 0;
}

uint64_t MoveMenu4Cursor(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822235D0 TitleScreen::MoveMenu4Cursor");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 80);
  if (Load(base, started) == 0) {
    const uint32_t menu = Load(base, kMenu);
    const uint32_t before = Load(base, menu + 4);
    const uint32_t now = Load(base, menu + 0);
    Store(base, Words(base) + 24, now);
    Finish(calls, frame, 0x82223630, movie, uint64_t(before) + 41);
    Finish(calls, frame, 0x8222363C, movie, 51);
    Finish(calls, frame, 0x82223648, movie, 52);
    const int32_t moved = int32_t(Load(base, Load(base, kMenu) + 8));
    if (moved == -1) {
      if (before == 0 && now == 1) {
        const uint32_t node = uint32_t(calls.Call(kFindNode, frame, 0x82223678, movie, 5, 0));
        Store(base, node + 16, Load(base, kMenu4WrapUp));
      } else {
        Play(calls, frame, 0x822236E0, movie, 51, 1);
      }
    } else if (moved == 1) {
      if (before == 1 && now == 0) {
        const uint32_t node = uint32_t(calls.Call(kFindNode, frame, 0x822236B4, movie, 5, 0));
        Store(base, node + 16, Load(base, kMenu4WrapDown));
      } else {
        Play(calls, frame, 0x822236E0, movie, 52, 1);
      }
    }
    Store(base, started, 1);
  }
  RunMovie(calls, frame, 0x822236F4, movie);
  return 1;
}

}  // namespace title_screen
