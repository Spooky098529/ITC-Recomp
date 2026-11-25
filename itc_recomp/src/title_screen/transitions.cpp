// The title screen's transitions and the menus' set-ups
// (docs/functions/title-screen.md): see title_screen.h. Each transition
// finds movie 0 (its id at its frame's +80), sets itself up once (its
// started word, the page object's +24, 0), then runs the movie.

#include <bit>

#include "base/log.h"
#include "race_screens/race_screens.h"
#include "schema/career.h"
#include "schema/tuning_screen.h"
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

uint32_t List() { return ListAddress(); }

// The cursor node's +16 = n × 40 as a float, through the original's frame
// doubleword at `slot` (the integer stored there for the conversion).
void StoreCursor(uint32_t slot, uint32_t node, uint32_t n) {
  const int32_t value = int32_t(n * 40u);
  const uint64_t wide = uint64_t(int64_t(value));
  Store(slot, uint32_t(wide >> 32));
  Store(slot + 4, uint32_t(wide));
  schema::UiMovieNode(node).set_y(float(double(int64_t(value))));
}

// The page object to page 2 through transition 6 (BackFromCareer's and
// BackFromMenu4's end), +24 = 0.
void BackToMainMenu() {
  const auto pages = G::pages();
  const uint32_t page = pages.page();
  if (page != 2) {
    pages.set_previous_page(page);
  }
  pages.set_queued_step(kNone);
  pages.set_step(6);
  pages.set_next_page(2);
  pages.set_page(kNone);
  G::pages().set_step_started(0);
}

// ShowMainMenu's and ShowMainMenuAtOnce's end: item 5 shown refused when
// SaveSlots::State(0) answers 0.
void MarkSaveItem(uint32_t frame, uint32_t movie, uint32_t lr_state,
                  uint32_t lr_color, const Calls& calls) {
  const uint64_t state = calls.Call(kSlotsState, frame, lr_state, 0);
  uint32_t color = 0;
  if (uint32_t(state) != 0) {
    Words().set_save_item_refused(0);
  } else {
    color = 2;
    Words().set_save_item_refused(1);
  }
  calls.Call(kSetNodeColor, frame, lr_color, movie, 4, 10, color);
}

}  // namespace

uint64_t SetUpMainMenu(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223700 TitleScreen::SetUpMainMenu");
  const uint32_t frame = Frame(sp, 144);
  const uint32_t movie = FindMovie(kTitleMovie, frame + 96);
  Store(frame + 92, 158);
  Store(frame + 84, 159);
  calls.CallArgs(kListInit, frame, 0x82223764, {List(), 7, 7, 1, 154, 153, 155, 156});
  race_screens::ListSetSelection(List(), Words().main_item());
  const uint32_t node = uint32_t(calls.Call(kFindNode, frame, 0x82223788, movie, 4, 0));
  StoreCursor(frame + 96, node, race_screens::ListSelection(List()));
  // Player 0's save data (career's CareerSave).
  const schema::CareerSave save(vehicle_accessors::SaveDataOf(0));
  // The save's word first, then the title's pointer, as the original.
  const uint32_t third = (save.stages() >> 5) & 1;
  Words().set_career_third_item(third);
  int32_t day = int32_t(save.day());
  if (day > 999) {
    day = 999;
  }
  const bool can_continue = day != 0 && save.garage_count() != 0;
  Words().set_can_continue(can_continue ? 1 : 0);
  if (Words().interrupted() == 0) {
    for (uint32_t k = 0; k < 4; k++) {
      Store(G::system_checks_address() + 4 * k, 0);
    }
  }
  return save.address;
}

uint64_t SetUpCareerMenu(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223840 TitleScreen::SetUpCareerMenu");
  const uint32_t frame = Frame(sp, 144);
  const uint32_t movie = FindMovie(kTitleMovie, frame + 96);
  const uint32_t third = Words().career_third_item();
  Store(frame + 92, 158);
  Store(frame + 84, 159);
  const uint32_t n = third != 0 ? 3 : 2;
  calls.CallArgs(kListInit, frame, 0x822238C0, {List(), n, n, 1, 154, 153, 155, 156});
  race_screens::ListSetSelection(List(), Words().career_selection());
  const uint32_t node = uint32_t(calls.Call(kFindNode, frame, 0x822238E0, movie, 5, 0));
  StoreCursor(frame + 96, node, race_screens::ListSelection(List()));
  uint64_t r3 = 0;
  for (uint32_t k = 5; k < 5 + n; k++) {
    // r7 to r10 unset in the original: 0 (the user's call, 2026-10-30).
    r3 = calls.Call(kSetText, frame, 0x82223928, movie, 5, k, k + 249);
  }
  return r3;
}

uint64_t SetUpMenu4(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223940 TitleScreen::SetUpMenu4");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = FindMovie(kTitleMovie, frame + 80);
  ResetMenu(G::menu());
  const uint32_t node = uint32_t(calls.Call(kFindNode, frame, 0x822239BC, movie, 5, 0));
  StoreCursor(frame + 80, node, G::menu().cursor());
  calls.Call(kSetText, frame, 0x822239F8, movie, 5, 5, 935);
  return calls.Call(kSetText, frame, 0x82223A0C, movie, 5, 6, 936);
}

uint64_t Intro(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82222B60 TitleScreen::Intro");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = FindMovie(kTitleMovie, frame + 80);
  if (Started(started).started() == 0) {
    calls.Call(kResetPlayer, frame, 0x82222BA4);
    Play(calls, frame, 0x82222BC0, movie, 82);
    Started(started).set_started(1);
  }
  return Done(RunMovie(calls, frame, 0x82222BD4, movie));
}

uint64_t PressStartLoop(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82222BF8 TitleScreen::PressStartLoop");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = FindMovie(kTitleMovie, frame + 80);
  if (Started(started).started() == 0) {
    Play(calls, frame, 0x82222C54, movie, 81);
    calls.Call(kLoopNamed, frame, 0x82222C60, movie, 81);
    Started(started).set_started(1);
  }
  RunMovie(calls, frame, 0x82222C74, movie);
  return 1;
}

uint64_t BackToPressStart(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82222C90 TitleScreen::Transition2");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = FindMovie(kTitleMovie, frame + 80);
  if (Started(started).started() == 0) {
    const uint32_t s = race_screens::ListSelection(List());
    Finish(calls, frame, 0x82222CEC, movie, uint64_t(s) + 2);
    calls.Call(kResetPlayer, frame, 0x82222CF0);
    Play(calls, frame, 0x82222D0C, movie, 85);
    Started(started).set_started(1);
  }
  return Done(RunMovie(calls, frame, 0x82222D20, movie));
}

uint64_t LeavePressStart(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82222D40 TitleScreen::LeavePressStart");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = FindMovie(kTitleMovie, frame + 80);
  if (Started(started).started() == 0) {
    Play(calls, frame, 0x82222D9C, movie, 83);
    Started(started).set_started(1);
  }
  return Done(RunMovie(calls, frame, 0x82222DB0, movie));
}

uint64_t ShowMainMenu(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223B78 TitleScreen::ShowMainMenu");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = FindMovie(kTitleMovie, frame + 80);
  if (Started(started).started() == 0) {
    SetUpMainMenu(frame, calls);
    Play(calls, frame, 0x82223BD4, movie, 84);
    Started(started).set_started(1);
  }
  const uint64_t done = Done(RunMovie(calls, frame, 0x82223BE4, movie));
  if (done != 0) {
    MarkSaveItem(frame, movie, 0x82223BF8, 0x82223C2C, calls);
  }
  return done;
}

uint64_t ShowMainMenuAtOnce(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223C38 TitleScreen::ShowMainMenuAtOnce");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = FindMovie(kTitleMovie, frame + 80);
  if (Started(started).started() == 0) {
    Play(calls, frame, 0x82223C90, movie, 82);
    Finish(calls, frame, 0x82223C9C, movie, 82);
    Play(calls, frame, 0x82223CB8, movie, 83);
    Finish(calls, frame, 0x82223CC4, movie, 83);
    Play(calls, frame, 0x82223CE0, movie, 81);
    calls.Call(kLoopNamed, frame, 0x82223CEC, movie, 81);
    SetUpMainMenu(frame, calls);
    Play(calls, frame, 0x82223D0C, movie, 84);
    Started(started).set_started(1);
  }
  const uint64_t done = Done(RunMovie(calls, frame, 0x82223D1C, movie));
  if (done != 0) {
    MarkSaveItem(frame, movie, 0x82223D30, 0x82223D64, calls);
  }
  return done;
}

uint64_t HighlightMain(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82222DD0 TitleScreen::HighlightMain");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = FindMovie(kTitleMovie, frame + 80);
  if (Started(started).started() == 0) {
    const auto words = Words();
    const uint32_t item = words.main_item();
    if (item != 5 || words.save_item_refused() != 0) {
      Play(calls, frame, 0x82222E4C, movie, uint64_t(item) + 2);
      calls.Call(kLoopLast, frame, 0x82222E54, movie);
    }
    Started(started).set_started(1);
  }
  RunMovie(calls, frame, 0x82222E68, movie);
  return 1;
}

uint64_t MoveMainCursor(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82222E88 TitleScreen::MoveMainCursor");
  const uint32_t frame = Frame(sp, 160);
  const uint32_t movie = FindMovie(kTitleMovie, frame + 80);
  if (Started(started).started() == 0) {
    const uint32_t s = race_screens::ListSelection(List());
    const uint32_t p = MenuListOf().selection_before();
    Finish(calls, frame, 0x82222EEC, movie, uint64_t(p) + 2);
    if (s == 5 && Words().save_item_refused() != 0) {
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
      StoreCursor(frame + 80, node, s);
    } else {
      const uint32_t up = int32_t(MenuListOf().move()) == -1 ? 12 : 13;
      Play(calls, frame, 0x8222302C, movie, up, 1);
    }
    Started(started).set_started(1);
  }
  RunMovie(calls, frame, 0x82223040, movie);
  return 1;
}

uint64_t RefreshSaveItem(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223050 TitleScreen::Transition9");
  const uint32_t frame = Frame(sp, 160);
  const uint32_t movie = FindMovie(kTitleMovie, frame + 80);
  if (Started(started).started() == 0) {
    const uint32_t s = race_screens::ListSelection(List());
    const uint32_t p = MenuListOf().selection_before();
    const uint32_t state = uint32_t(calls.Call(kSlotsState, frame, 0x822230AC, 0));
    Finish(calls, frame, 0x822230BC, movie, uint64_t(p) + 2);
    uint32_t color = 2;
    if (s == 5 && state == 0) {
      Finish(calls, frame, 0x822230DC, movie, 7);
      Play(calls, frame, 0x822230F8, movie, 26);
      Finish(calls, frame, 0x82223104, movie, 26);
      Play(calls, frame, 0x82223120, movie, 27, 1, 1);
      Finish(calls, frame, 0x8222312C, movie, 27);
      Words().set_save_item_refused(1);
    } else {
      Play(calls, frame, 0x82223188, movie, uint64_t(s) + 2);
      calls.Call(kLoopNamed, frame, 0x82223194, movie, uint64_t(s) + 2);
      if (state == 0) {
        Words().set_save_item_refused(1);
      } else {
        color = 0;
        Words().set_save_item_refused(0);
      }
    }
    calls.Call(kSetNodeColor, frame, 0x8222314C, movie, 4, 10, color);
    Started(started).set_started(1);
  }
  RunMovie(calls, frame, 0x8222315C, movie);
  return 1;
}

uint64_t RefreshPadItem(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822231B0 TitleScreen::Transition10");
  const uint32_t frame = Frame(sp, 160);
  const uint32_t movie = FindMovie(kTitleMovie, frame + 80);
  if (Started(started).started() == 0) {
    const uint32_t s = race_screens::ListSelection(List());
    const uint32_t p = MenuListOf().selection_before();
    const uint32_t entry = G::mapper_entries_address() + (G::player1_controller() << 3);
    const uint32_t connected = uint32_t(calls.Call(kPadIsConnected, frame, 0x82223224, entry));
    Finish(calls, frame, 0x82223234, movie, uint64_t(p) + 2);
    uint32_t color = 2;
    if (s == 4 && connected == 0) {
      Finish(calls, frame, 0x82223254, movie, 6);
      Play(calls, frame, 0x82223270, movie, 26);
      Finish(calls, frame, 0x8222327C, movie, 26);
      Play(calls, frame, 0x82223298, movie, 27, 1, 1);
      Finish(calls, frame, 0x822232A4, movie, 27);
      Words().set_pad_item_state(0);
    } else {
      Play(calls, frame, 0x82223300, movie, uint64_t(s) + 2);
      calls.Call(kLoopNamed, frame, 0x8222330C, movie, uint64_t(s) + 2);
      if (connected == 0) {
        Words().set_pad_item_state(0);
      } else {
        color = 0;
        Words().set_pad_item_state(1);
      }
    }
    calls.Call(kSetNodeColor, frame, 0x822232C4, movie, 4, 9, color);
    Started(started).set_started(1);
  }
  RunMovie(calls, frame, 0x822232D4, movie);
  return 1;
}

uint64_t ShowCareerMenu(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223D70 TitleScreen::ShowCareerMenu");
  const uint32_t frame = Frame(sp, 144);
  const uint32_t movie = FindMovie(kTitleMovie, frame + 80);
  if (Started(started).started() == 0) {
    const uint32_t s = race_screens::ListSelection(List());
    Finish(calls, frame, 0x82223DC8, movie, uint64_t(s) + 2);
    Play(calls, frame, 0x82223DE4, movie, 86);
    const auto words = Words();
    words.set_career_selection(words.can_continue() != 0 ? 1 : 0);
    SetUpCareerMenu(frame, calls);
    Started(started).set_started(1);
  }
  const uint64_t done = Done(RunMovie(calls, frame, 0x82223E18, movie));
  if (done != 0 && Words().can_continue() == 0) {
    calls.Call(kSetNodeColor, frame, 0x82223E48, movie, 5, 6, 2);
  }
  return done;
}

namespace {

// HighlightCareer's and MoveCareerCursor's highlight of item `s` (continue,
// 1, refused when the title's +8 is 0).
void HighlightCareerItem(uint32_t frame, uint32_t movie, uint32_t s,
                         const uint32_t (&lrs)[6], const Calls& calls) {
  if (Words().can_continue() == 0 && s == 1) {
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

uint64_t HighlightCareer(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223328 TitleScreen::HighlightCareer");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = FindMovie(kTitleMovie, frame + 80);
  if (Started(started).started() == 0) {
    const uint32_t s = race_screens::ListSelection(List());
    HighlightCareerItem(frame, movie, s,
                        {0x822233A4, 0x822233B0, 0x822233CC, 0x822233D8, 0x822233FC, 0x82223408},
                        calls);
    Started(started).set_started(1);
  }
  RunMovie(calls, frame, 0x8222341C, movie);
  return 1;
}

uint64_t MoveCareerCursor(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223428 TitleScreen::MoveCareerCursor");
  const uint32_t frame = Frame(sp, 144);
  const uint32_t movie = FindMovie(kTitleMovie, frame + 80);
  if (Started(started).started() == 0) {
    const uint32_t s = race_screens::ListSelection(List());
    const uint32_t p = MenuListOf().selection_before();
    Finish(calls, frame, 0x8222348C, movie, uint64_t(p) + 41);
    HighlightCareerItem(frame, movie, s,
                        {0x822234C4, 0x822234D0, 0x822234EC, 0x822234F8, 0x8222351C, 0x82223528},
                        calls);
    Finish(calls, frame, 0x82223534, movie, 51);
    Finish(calls, frame, 0x82223540, movie, 52);
    const schema::MenuList list = MenuListOf();
    if (list.wrapped() != 0) {
      const uint32_t node = uint32_t(calls.Call(kFindNode, frame, 0x82223578, movie, 5, 0));
      StoreCursor(frame + 80, node, s);
    } else {
      const uint32_t up = int32_t(list.move()) == -1 ? 51 : 52;
      Play(calls, frame, 0x822235A8, movie, up, 1);
    }
    Started(started).set_started(1);
  }
  RunMovie(calls, frame, 0x822235BC, movie);
  return 1;
}

uint64_t BackFromCareer(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223E58 TitleScreen::BackFromCareer");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = FindMovie(kTitleMovie, frame + 80);
  if (Started(started).started() == 0) {
    const uint32_t s = race_screens::ListSelection(List());
    Finish(calls, frame, 0x82223EAC, movie, uint64_t(s) + 41);
    Play(calls, frame, 0x82223EC8, movie, 87);
    SetUpMainMenu(frame, calls);
    Started(started).set_started(1);
  }
  if (uint32_t(RunMovie(calls, frame, 0x82223EE0, movie)) == 0) {
    BackToMainMenu();
  }
  return 0;
}

uint64_t ShowMenu4(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223F30 TitleScreen::ShowMenu4");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = FindMovie(kTitleMovie, frame + 80);
  if (Started(started).started() == 0) {
    const uint32_t s = race_screens::ListSelection(List());
    Finish(calls, frame, 0x82223F84, movie, uint64_t(s) + 2);
    Play(calls, frame, 0x82223FA0, movie, 86);
    Words().set_menu4_choice(0);
    SetUpMenu4(frame, calls);
    Started(started).set_started(1);
  }
  return Done(RunMovie(calls, frame, 0x82223FC4, movie));
}

uint64_t BackFromMenu4(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223FD8 TitleScreen::BackFromMenu4");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = FindMovie(kTitleMovie, frame + 80);
  if (Started(started).started() == 0) {
    Finish(calls, frame, 0x82224028, movie, uint64_t(G::menu().previous()) + 41);
    Play(calls, frame, 0x82224044, movie, 87);
    SetUpMainMenu(frame, calls);
    Started(started).set_started(1);
  }
  if (uint32_t(RunMovie(calls, frame, 0x8222405C, movie)) == 0) {
    BackToMainMenu();
  }
  return 0;
}

uint64_t MoveMenu4Cursor(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822235D0 TitleScreen::MoveMenu4Cursor");
  const uint32_t frame = Frame(sp, 144);
  const uint32_t movie = FindMovie(kTitleMovie, frame + 80);
  if (Started(started).started() == 0) {
    const auto menu = G::menu();
    const uint32_t before = menu.previous();
    const uint32_t now = menu.cursor();
    Words().set_menu4_choice(now);
    Finish(calls, frame, 0x82223630, movie, uint64_t(before) + 41);
    Finish(calls, frame, 0x8222363C, movie, 51);
    Finish(calls, frame, 0x82223648, movie, 52);
    const int32_t moved = int32_t(G::menu().moved());
    if (moved == -1) {
      if (before == 0 && now == 1) {
        const uint32_t node = uint32_t(calls.Call(kFindNode, frame, 0x82223678, movie, 5, 0));
        schema::UiMovieNode(node).set_y(std::bit_cast<float>(G::menu4_wrap_up()));
      } else {
        Play(calls, frame, 0x822236E0, movie, 51, 1);
      }
    } else if (moved == 1) {
      if (before == 1 && now == 0) {
        const uint32_t node = uint32_t(calls.Call(kFindNode, frame, 0x822236B4, movie, 5, 0));
        schema::UiMovieNode(node).set_y(std::bit_cast<float>(G::menu4_wrap_down()));
      } else {
        Play(calls, frame, 0x822236E0, movie, 52, 1);
      }
    }
    Started(started).set_started(1);
  }
  RunMovie(calls, frame, 0x822236F4, movie);
  return 1;
}

}  // namespace title_screen
