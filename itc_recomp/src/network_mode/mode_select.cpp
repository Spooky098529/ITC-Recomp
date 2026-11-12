// The type 12 mode's mode select (NetModeSelect::, machine 0, UI movie 35):
// its running screen, pages, transitions and replies
// (docs/functions/network-mode.md): see network_mode.h. UiMovies::Find,
// FindNode and the fades' words are plain reads; the movie's setters and
// players, the fades, Menu::Step and Pressed, MenuSound::Play and
// SaveSlots::State are guest calls through their overrides, from the
// originals' frames with their return addresses. The varargs
// UiMovie::SetText gets 0 for the arguments its callers leave unset (the
// user's call of 2026-10-30). The Xbox Live calls are plugged in (the
// user's call, 2026-11-12): the privilege check sub_8214F620 answers 0 (the
// SDK's answer for every user, kPrivilege), and XShowSigninUI sub_822952C0,
// whose answer the sign-in pages don't read, isn't called (its answer, 0,
// left in r3). The Live users' mask, player 1's controller and the system
// UI's words are guest reads, as the original's.

#include "network_mode/network_mode.h"

#include "base/log.h"
#include "network_mode/internal.h"
#include "ui_movies/game.h"

namespace network_mode {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;

constexpr uint32_t kResetPose = 0x8211ABA8;
constexpr uint32_t kSetText = 0x8211C1F8;
constexpr uint32_t kScrollText = 0x8211C570;
constexpr uint32_t kResetTextScroll = 0x8211C5F8;
constexpr uint32_t kSetNodeTexture = 0x8211A708;
constexpr uint32_t kHideNode = 0x8211AC48;
constexpr uint32_t kPlayNamed = 0x8211DAC8;
constexpr uint32_t kLoopLast = 0x8211DDE8;
constexpr uint32_t kFinishNamed = 0x8211DF20;
constexpr uint32_t kFinishAll = 0x8211DFF8;
constexpr uint32_t kRun = 0x8211E1B8;
constexpr uint32_t kMoviesDelete = 0x8211E258;
constexpr uint32_t kFadeOut = 0x8209D0C8;
constexpr uint32_t kFadesOut = 0x82001BF0;
constexpr uint32_t kFadeCount0 = 0x826A50E4;
constexpr uint32_t kFadeCount1 = 0x826A50DC;
constexpr uint32_t kMenuStep = 0x82186568;
constexpr uint32_t kMenuPressed = 0x821864C8;
constexpr uint32_t kMenuSoundPlay = 0x821103F0;
constexpr uint32_t kSlotsState = 0x8228F9E8;
constexpr uint32_t kAccept = 159;
constexpr uint32_t kBack = 158;
constexpr uint32_t kMachineDelete = 9616;

// The pages' table in Draw's frame (+80 on): MenuPage, the empty
// sub_82106540, LeavePage, the empty one, BattlePage, TimeAttackPage,
// SignInPage, SignInPage2, BackPage.
constexpr uint32_t kPageTable[9] = {0x8227C658, 0x82106540, 0x8227C970,
                                    0x82106540, 0x8227C9F0, 0x8227CB38,
                                    0x8227C088, 0x8227C210, 0x8227CC78};
// The transitions' table in Update's frame (+96 on): Intro, ShowCursor,
// MoveCursor, FadeOut, FadeIn.
constexpr uint32_t kTransitionTable[5] = {0x8227CE50, 0x8227D138, 0x8227D380, 0x8227D088,
                                          0x8227CFA8};

uint32_t Pages(uint8_t* base) { return Load(base, kSelectPages); }
uint32_t Menu(uint8_t* base) { return Load(base, kSelectMenu); }
uint32_t Data(uint8_t* base) { return Load(base, kSelectData); }

// UiMovies::Find for movie 35 with its id stored at `key` (the original's
// frame word): the movie in the pair's value.
uint32_t FindMovie(uint8_t* base, uint32_t key) {
  Store(base, key, kSelectMovie);
  return Load(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, key));
}

// The page object's +8 = its page unless that is `unless`.
void KeepPage(uint8_t* base, uint32_t pages, uint32_t unless) {
  const uint32_t page = Load(base, pages + 0);
  if (page != unless) {
    Store(base, pages + 8, page);
  }
}

// On the dispatcher's ring (+204).
void Post(uint8_t* base, uint32_t dispatcher, uint32_t id) {
  PostRing(base, dispatcher + 204, id);
}

// The menu reset as Create makes it (two items).
void ResetMenu(uint8_t* base) {
  const uint32_t menu = Menu(base);
  Store(base, menu + 12, 0);
  Store(base, menu + 16, 1);
  Store(base, menu + 20, 1);
  for (const uint32_t field : {0u, 4u, 8u, 40u, 48u, 52u, 36u, 64u, 24u, 28u, 32u}) {
    Store(base, menu + field, 0);
  }
}

// Both fades done (their counts 0).
bool FadesDone(uint8_t* base) {
  return (Load(base, kFadeCount0) | Load(base, kFadeCount1)) == 0;
}

uint64_t Play(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t id) {
  return calls.Call(kPlayNamed, frame, lr, movie, id, 0, 0);
}
uint64_t RunMovie(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie) {
  return calls.Call(kRun, frame, lr, movie, 0);
}

// UiMovie::SetText(movie, sub, child, string) with r7 to r10 0.
uint64_t SetText(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint32_t sub,
                 uint32_t child, uint64_t string) {
  return calls.Call(kSetText, frame, lr, movie, sub, child, string);
}

// The jump to `page` (+8 the page unless it, +0 it, +12 -1).
void JumpTo(uint8_t* base, uint32_t pages, uint32_t page) {
  KeepPage(base, pages, page);
  Store(base, pages + 0, page);
  Store(base, pages + 12, kNone);
}

// BattlePage's and TimeAttackPage's "no Live user" way: dialog 13's reason
// 7, message 19 posted, page 3.
void NotOnLive(uint8_t* base, uint32_t dispatcher) {
  Store(base, kDialog13Reason, 7);
  Post(base, dispatcher, 19);
  const uint32_t pages = Pages(base);
  KeepPage(base, pages, 3);
  Store(base, pages + 12, kNone);
  Store(base, pages + 0, 3);
  Store(base, pages + 16, kNone);
}

// The leave after the fade out: transition 3 (FadeOut), then page 2.
void FadeOutToLeave(uint8_t* base) {
  const uint32_t pages = Pages(base);
  KeepPage(base, pages, 2);
  Store(base, pages + 12, 3);
  Store(base, pages + 0, kNone);
  Store(base, pages + 4, 2);
  Store(base, pages + 16, kNone);
}

// Player 1's controller's bit in the Live users' mask (a shift of 32 or
// more gives 0, as slw does). False: `r3` the controller's word, as the
// original leaves it.
bool OnLive(uint8_t* base, uint64_t* r3) {
  const uint32_t controller = Load(base, kPlayerController);
  *r3 = controller;
  const uint32_t bit = (controller & 0x20) != 0 ? 0 : 1u << (controller & 0x1F);
  return (bit & Load(base, kLiveMask)) != 0;
}

uint64_t SignInPageFor(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls,
                       uint32_t lr, uint32_t signed_in_page) {
  const uint32_t frame = Frame(base, sp, 96);
  if (Load(base, kSystemUi0) != 0 || Load(base, kSystemUi1) != 0) {
    Store(base, Data(base) + 28, 0);
    return r3;
  }
  if (Load(base, Data(base) + 12) == 0) {
    // XShowSigninUI(1, 2) plugged in: no dialog (one local user is always
    // signed in), its answer 0 not read.
    Store(base, Data(base) + 12, 1);
    return 0;
  }
  r3 = calls.Call(kSlotsState, frame, lr, 0);
  uint64_t controller = 0;
  const bool live = uint32_t(r3) != 0 && OnLive(base, &controller);
  const uint32_t data = Data(base);
  const uint32_t count = Load(base, data + 28);
  Store(base, data + 28, count + 1);
  if (int32_t(count) < 10) {
    return r3;
  }
  Store(base, Data(base) + 28, 10);
  uint32_t page;
  if (live) {
    page = Load(base, Data(base) + 32) != 0 ? signed_in_page : 0;
  } else {
    const uint32_t d = Data(base);
    if (Load(base, d + 12) == 0) {
      return r3;
    }
    Store(base, d + 12, 0);
    page = 0;
  }
  const uint32_t pages = Pages(base);
  KeepPage(base, pages, page);
  Store(base, pages + 0, page);
  Store(base, pages + 12, kNone);
  Store(base, pages + 16, kNone);
  return r3;
}

}  // namespace

uint64_t SelectSetUp(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227CCE0 NetModeSelect::SetUp");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, frame + 80);
  calls.Call(kResetPose, frame, 0x8227CD10, movie);
  ResetMenu(base);
  SetText(calls, frame, 0x8227CD6C, movie, 4, 5, 298);
  SetText(calls, frame, 0x8227CD80, movie, 4, 6, 299);
  SetText(calls, frame, 0x8227CD94, movie, 1, 0, 935);
  calls.Call(kScrollText, frame, 0x8227CDA8, movie, 1, 0, 1);
  calls.Call(kResetTextScroll, frame, 0x8227CDB8, movie, 1, 0);
  calls.Call(kSetNodeTexture, frame, 0x8227CDCC, movie, 0, 4, 0);
  calls.Call(kSetNodeTexture, frame, 0x8227CDE0, movie, 0, 5, 0);
  calls.Call(kSetNodeTexture, frame, 0x8227CDF4, movie, 0, 6, 1);
  calls.Call(kSetNodeTexture, frame, 0x8227CE08, movie, 0, 7, 1);
  calls.Call(kHideNode, frame, 0x8227CE18, movie, 0, 2);
  const uint64_t r3 = calls.Call(kHideNode, frame, 0x8227CE28, movie, 0, 10);
  Store(base, Data(base) + 0, 1);
  Store(base, Data(base) + 4, 0);
  Store(base, Data(base) + 32, 1);
  return r3;
}

uint64_t SelectStart(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227D300 NetModeSelect::Start");
  const uint32_t frame = Frame(base, sp, 96);
  const uint64_t r3 = SelectSetUp(base, frame, calls);
  Store(base, Data(base) + 28, 0);
  uint32_t pages = Pages(base);
  Store(base, pages + 4, 0);
  Store(base, pages + 0, 0);
  Store(base, pages + 8, 0);
  Store(base, pages + 20, kNone);
  Store(base, pages + 12, kNone);
  Store(base, pages + 24, 0);
  pages = Pages(base);
  KeepPage(base, pages, 0);
  Store(base, pages + 12, 0);
  Store(base, pages + 4, 0);
  Store(base, pages + 0, kNone);
  Store(base, pages + 16, 1);
  return r3;
}

void SelectOpen(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_8227C5C8 NetModeSelect::Open");
  if (Load(base, machine + kMachineLoaded) == 0) {
    Store(base, machine + kMachineState, 1);
    return;
  }
  const bool shown = Load(base, Data(base) + 0) != 0;
  const uint32_t pages = Pages(base);
  KeepPage(base, pages, 0);
  Store(base, pages + 12, shown ? 4 : 0);
  Store(base, pages + 16, 1);
  Store(base, pages + 4, 0);
  Store(base, pages + 0, kNone);
  Store(base, machine + kMachineState, 4);
}

uint64_t SelectDraw(uint8_t* base, uint64_t r3, uint64_t dispatcher, uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227D248 NetModeSelect::Draw");
  const uint32_t frame = Frame(base, sp, 160);
  for (uint32_t k = 0; k < 9; k++) {
    Store(base, frame + 80 + 4 * k, kPageTable[k]);
  }
  const uint32_t page = Load(base, Pages(base) + 0);
  if (page == kNone) {
    return r3;
  }
  // No bounds check, as the original: the word at the table's index.
  return calls.Call(Load(base, frame + 80 + (page << 2)), frame, 0x8227D2F4, dispatcher, r3);
}

uint64_t SelectUpdate(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227D4E8 NetModeSelect::Update");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t movie = FindMovie(base, frame + 80);
  Store(base, frame + 112, kTransitionTable[4]);
  for (uint32_t k = 0; k < 4; k++) {
    Store(base, frame + 96 + 4 * k, kTransitionTable[k]);
  }
  const uint32_t running = Load(base, Pages(base) + 12);
  if (running == kNone) {
    return RunMovie(calls, frame, 0x8227D674, movie);
  }
  uint64_t r3 = movie;
  if (int32_t(running) >= 0) {
    // No bounds check: the word at the table's index.
    r3 = calls.Call(Load(base, frame + 96 + (running << 2)), frame, 0x8227D604,
                    Pages(base) + 24);
    if (uint32_t(r3) == 0) {
      return r3;
    }
  }
  const uint32_t p = Pages(base);
  const uint32_t queued = Load(base, p + 16);
  if (queued == kNone) {
    const uint32_t next = Load(base, p + 4);
    Store(base, p + 12, kNone);
    Store(base, p + 0, next);
  } else {
    Store(base, p + 16, kNone);
    Store(base, p + 20, 0);
    Store(base, p + 12, queued);
  }
  Store(base, p + 24, 0);
  return r3;
}

uint64_t SelectMenuPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227C658 NetModeSelect::MenuPage");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kMenuStep, frame, 0x8227C670, Menu(base));
  uint64_t r3 = calls.Call(kMenuPressed, frame, 0x8227C67C, Menu(base), kAccept);
  if (uint32_t(r3) != 0) {
    r3 = calls.Call(kMenuSoundPlay, frame, 0x8227C694, 0);
    const uint32_t item = Load(base, Menu(base) + 0);
    if (item == 0 || item == 1) {
      Store(base, Load(base, kScreen1Data) + 8, 1);
      if (item == 1) {
        r3 = SessionMake(base, r3, frame, calls);
      }
      const uint32_t page = item == 0 ? 4 : 5;
      const uint32_t pages = Pages(base);
      JumpTo(base, pages, page);
      Store(base, pages + 16, kNone);
    }
  } else {
    r3 = calls.Call(kMenuPressed, frame, 0x8227C71C, Menu(base), kBack);
    if (uint32_t(r3) != 0) {
      r3 = calls.Call(kMenuSoundPlay, frame, 0x8227C72C, 2);
      const uint32_t pages = Pages(base);
      KeepPage(base, pages, 8);
      Store(base, pages + 0, kNone);
      Store(base, pages + 12, 3);
      Store(base, pages + 4, 8);
      Store(base, pages + 16, kNone);
    }
  }
  if (Load(base, Menu(base) + 8) != 0) {
    r3 = calls.Call(kMenuSoundPlay, frame, 0x8227C770, 4);
    const uint32_t pages = Pages(base);
    KeepPage(base, pages, 0);
    Store(base, pages + 16, kNone);
    Store(base, pages + 0, kNone);
    Store(base, pages + 12, 2);
    Store(base, pages + 4, 0);
  }
  return r3;
}

void SelectLeavePage(uint8_t* base, uint32_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_8227C970 NetModeSelect::LeavePage");
  const uint32_t pages = Pages(base);
  KeepPage(base, pages, 3);
  Store(base, pages + 12, kNone);
  Store(base, pages + 16, kNone);
  Store(base, pages + 0, 3);
  Post(base, dispatcher, 20);
}

uint64_t SelectBattlePage(uint8_t* base, uint32_t dispatcher, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227C9F0 NetModeSelect::BattlePage");
  const uint32_t frame = Frame(base, sp, 112);
  uint64_t r3 = calls.Call(kSlotsState, frame, 0x8227CA10, 0);
  if (uint32_t(r3) == 0 || !OnLive(base, &r3)) {
    NotOnLive(base, dispatcher);
    return r3;
  }
  // sub_8214F620(controller, 254) plugged in.
  r3 = kPrivilege;
  if (kPrivilege != 0) {
    FadeOutToLeave(base);
    return r3;
  }
  Store(base, Data(base) + 16, 1);
  Store(base, kDialog12Reason, 10);
  Post(base, dispatcher, 18);
  const uint32_t pages = Pages(base);
  KeepPage(base, pages, 3);
  Store(base, pages + 12, kNone);
  Store(base, pages + 0, 3);
  Store(base, pages + 16, kNone);
  return r3;
}

uint64_t SelectTimeAttackPage(uint8_t* base, uint32_t dispatcher, uint32_t sp,
                              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227CB38 NetModeSelect::TimeAttackPage");
  const uint32_t frame = Frame(base, sp, 112);
  uint64_t r3 = calls.Call(kSlotsState, frame, 0x8227CB58, 0);
  if (uint32_t(r3) == 0 || !OnLive(base, &r3)) {
    NotOnLive(base, dispatcher);
    return r3;
  }
  // sub_8214F620(controller, 254) plugged in; its answer kept in the
  // session's +228 (no check for a null session, as the original).
  r3 = kPrivilege;
  Store(base, Load(base, kSession) + 228, kPrivilege != 0 ? 1 : 0);
  FadeOutToLeave(base);
  return r3;
}

uint64_t SelectSignInPage(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227C088 NetModeSelect::SignInPage");
  return SignInPageFor(base, r3, sp, calls, 0x8227C0F0, 5);
}

uint64_t SelectSignInPage2(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227C210 NetModeSelect::SignInPage2");
  return SignInPageFor(base, r3, sp, calls, 0x8227C278, 4);
}

void SelectBackPage(uint8_t* base, uint32_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_8227CC78 NetModeSelect::BackPage");
  Store(base, Data(base) + 8, 1);
  Store(base, Data(base) + 4, 1);
  Post(base, dispatcher, 4);
}

uint64_t SelectIntro(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227CE50 NetModeSelect::Intro");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, frame + 80);
  if (Load(base, started) == 0) {
    Play(calls, frame, 0x8227CEA8, movie, 48);
    Play(calls, frame, 0x8227CEC4, movie, 46);
    calls.Call(kLoopLast, frame, 0x8227CECC, movie);
    calls.Call(kFadeIn, frame, 0x8227CEE8, Load(base, kFadesPasses), 0, 0, 0);
    Store(base, started, 1);
  }
  const uint64_t run = RunMovie(calls, frame, 0x8227CEF8, movie);
  if (!FadesDone(base) || uint32_t(run) != 0) {
    return 0;
  }
  Play(calls, frame, 0x8227CF44, movie, 43);
  calls.Call(kLoopLast, frame, 0x8227CF4C, movie);
  ResetMenu(base);
  return 1;
}

uint64_t SelectShowCursor(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227D138 NetModeSelect::ShowCursor");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80);
  if (Load(base, started) == 0) {
    Play(calls, frame, 0x8227D1A0, movie, uint64_t(Load(base, Menu(base))) + 5);
    calls.Call(kLoopLast, frame, 0x8227D1A8, movie);
    Store(base, started, 1);
  }
  RunMovie(calls, frame, 0x8227D1BC, movie);
  return 1;
}

uint32_t SelectSetCursorY(uint8_t* base, uint32_t y, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8227D1D8 NetModeSelect::SetCursorY");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80);
  Store(base, frame + 80, 0);
  Store(base, frame + 84, y);
  const float value = float(double(int64_t(uint64_t(y))));
  const uint32_t node = ui_movies::FindNode(base, movie, 4, 0);
  guest::StoreFloat(base, node + 16, value);
  return node;
}

uint64_t SelectMoveCursor(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227D380 NetModeSelect::MoveCursor");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, frame + 80);
  if (Load(base, started) == 0) {
    const uint32_t menu = Menu(base);
    const uint32_t before = Load(base, menu + 4), item = Load(base, menu + 0);
    // The rows from the words as loaded (zero-extended), as the original adds.
    auto row = [](uint32_t i) { return uint64_t(i) + 5; };
    calls.Call(kFinishNamed, frame, 0x8227D3D4, movie, row(before));
    calls.Call(kFinishNamed, frame, 0x8227D3E4, movie, row(item));
    Play(calls, frame, 0x8227D400, movie, row(item));
    calls.Call(kLoopLast, frame, 0x8227D408, movie);
    calls.Call(kFinishNamed, frame, 0x8227D414, movie, 15);
    calls.Call(kFinishNamed, frame, 0x8227D420, movie, 16);
    const uint32_t direction = Load(base, Menu(base) + 8);
    if (direction == kNone) {
      if (before == 0 && item == 1) {
        SelectSetCursorY(base, 20, frame);
      } else {
        Play(calls, frame, 0x8227D490, movie, 15);
      }
    } else if (direction == 1) {
      if (before == 1 && item == 0) {
        SelectSetCursorY(base, 0, frame);
      } else {
        Play(calls, frame, 0x8227D490, movie, 16);
      }
    }
    SetText(calls, frame, 0x8227D4A4, movie, 1, 0, uint64_t(item) + 935);
    calls.Call(kScrollText, frame, 0x8227D4B8, movie, 1, 0, 1);
    calls.Call(kResetTextScroll, frame, 0x8227D4C8, movie, 1, 0);
    Store(base, started, 1);
  }
  RunMovie(calls, frame, 0x8227D4DC, movie);
  return 1;
}

uint64_t SelectFadeOut(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227D088 NetModeSelect::FadeOut");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80);
  if (Load(base, started) == 0) {
    calls.Call(kFadeOut, frame, 0x8227D0E4, Load(base, kFadesOut), 0, 0, 0);
    Store(base, started, 1);
  }
  RunMovie(calls, frame, 0x8227D0F8, movie);
  return FadesDone(base) ? 1 : 0;
}

uint64_t SelectFadeIn(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227CFA8 NetModeSelect::FadeIn");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, frame + 80);
  if (Load(base, started) == 0) {
    calls.Call(kFadeIn, frame, 0x8227D000, Load(base, kFadesPasses), 0, 0, 0);
    Store(base, started, 1);
  }
  // The fades read before the movie runs, as the original.
  const bool done = FadesDone(base);
  RunMovie(calls, frame, 0x8227D02C, movie);
  if (!done) {
    return 0;
  }
  ResetMenu(base);
  return 1;
}

void SelectOnNo(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8227BF80 NetModeSelect::OnNo");
  const uint32_t page = Load(base, Menu(base) + 0) == 0 ? 7 : 6;
  Store(base, Data(base) + 20, page);
  Store(base, Data(base) + 24, kNone);
}

void SelectOnYes(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8227BFB8 NetModeSelect::OnYes");
  Store(base, Data(base) + 20, 0);
  Store(base, Data(base) + 24, kNone);
}

void SelectOnDialogClosed(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8227BFD8 NetModeSelect::OnDialogClosed");
  uint32_t data = Data(base);
  if (Load(base, data + 16) != 0 && Load(base, Menu(base) + 0) == 0) {
    Store(base, data + 20, 0);
    Store(base, Data(base) + 24, kNone);
    data = Data(base);
  }
  const uint32_t page = Load(base, data + 20);
  const uint32_t transition = Load(base, data + 24);
  const uint32_t pages = Pages(base);
  KeepPage(base, pages, page);
  Store(base, pages + 16, kNone);
  if (transition == kNone) {
    Store(base, pages + 0, page);
    Store(base, pages + 12, kNone);
  } else {
    Store(base, pages + 12, transition);
    Store(base, pages + 4, page);
    Store(base, pages + 0, kNone);
  }
  Store(base, Data(base) + 20, kNone);
  Store(base, Data(base) + 24, kNone);
  Store(base, Data(base) + 16, 0);
}

uint64_t SelectUnload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227C7A8 NetModeSelect::Unload");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80);
  uint64_t r3 = calls.Call(kFinishAll, frame, 0x8227C7DC, movie);
  if (Load(base, machine + kMachineDelete) != 0) {
    r3 = calls.Call(kMoviesDelete, frame, 0x8227C7F0, kSelectMovie);
    Store(base, machine + kMachineLoaded, 0);
  }
  return r3;
}

void SelectUnloaded(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8227C810 NetModeSelect::Unloaded");
  if (Load(base, Data(base) + 8) != 0) {
    Store(base, machine + kMachineState, 14);
    return;
  }
  const uint32_t item = Load(base, Menu(base) + 0);
  uint32_t id;
  if (item <= 1) {
    id = item == 0 ? 6 : 7;
    Store(base, kSignInWatch + 16, 0);
    Store(base, machine + kMachineState, 9);
  } else {
    // Never in play (two items): the caller's frame word the original
    // leaves in r9.
    id = Load(base, sp - 16);
  }
  Post(base, dispatcher, id);
}

}  // namespace network_mode
