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
#include "schema/tuning_screen.h"
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
constexpr uint32_t kMenuStep = 0x82186568;
constexpr uint32_t kMenuPressed = 0x821864C8;
constexpr uint32_t kMenuSoundPlay = 0x821103F0;
constexpr uint32_t kSlotsState = 0x8228F9E8;
constexpr uint32_t kAccept = 159;
constexpr uint32_t kBack = 158;

using G = schema::NetworkModeGlobals;
using NetMachine = schema::NetMachine;
using PageObject = schema::PageObject;

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

PageObject Pages(uint8_t* base) { return G::select_pages(base); }
schema::FrontEndMenu Menu(uint8_t* base) { return G::select_menu(base); }
schema::NetModeSelectData Data(uint8_t* base) { return G::select_data(base); }

// UiMovies::Find for movie 35 with its id stored at `key` (the original's
// frame word): the movie in the pair's value.
uint32_t FindMovie(uint8_t* base, uint32_t key) {
  Store(base, key, kSelectMovie);
  return Load(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, key));
}

// The page object's page before = its page unless that is `unless`.
void KeepPage(const PageObject& pages, uint32_t unless) {
  const uint32_t page = pages.page();
  if (page != unless) {
    pages.set_previous_page(page);
  }
}

// On the dispatcher's ring.
void Post(uint8_t* base, uint32_t dispatcher, uint32_t id) { PostRing(base, dispatcher, id); }

// The menu reset as Create makes it (two items).
void ResetMenu(uint8_t* base) {
  const schema::FrontEndMenu menu = Menu(base);
  menu.set_first_item(0);
  menu.set_last_item(1);
  menu.set_wraps(1);
  menu.set_cursor(0);
  menu.set_previous(0);
  menu.set_moved(0);
  menu.set_wrapped(0);
  menu.set_visible_row(0);
  menu.set_previous_row(0);
  menu.set_side(0);
  menu.set_player(0);
  menu.set_up_down_locked(0);
  menu.set_left_right_steps(0);
  menu.set_shoulders_step(0);
}

// Both fades done (their counts 0).
bool FadesDone(uint8_t* base) { return (G::fade_count0(base) | G::fade_count1(base)) == 0; }

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

// The jump to `page` (the page before kept unless it, the page it, no step).
void JumpTo(const PageObject& pages, uint32_t page) {
  KeepPage(pages, page);
  pages.set_page(page);
  pages.set_step(kNone);
}

// BattlePage's and TimeAttackPage's "no Live user" way: dialog 13's reason
// 7, message 19 posted, page 3.
void NotOnLive(uint8_t* base, uint32_t dispatcher) {
  G::set_dialog13_reason(base, 7);
  Post(base, dispatcher, 19);
  const PageObject pages = Pages(base);
  KeepPage(pages, 3);
  pages.set_step(kNone);
  pages.set_page(3);
  pages.set_queued_step(kNone);
}

// The leave after the fade out: transition 3 (FadeOut), then page 2.
void FadeOutToLeave(uint8_t* base) {
  const PageObject pages = Pages(base);
  KeepPage(pages, 2);
  pages.set_step(3);
  pages.set_page(kNone);
  pages.set_next_page(2);
  pages.set_queued_step(kNone);
}

// Player 1's controller's bit in the Live users' mask (a shift of 32 or
// more gives 0, as slw does). False: `r3` the controller's word, as the
// original leaves it.
bool OnLive(uint8_t* base, uint64_t* r3) {
  const uint32_t controller = G::player_controller(base);
  *r3 = controller;
  const uint32_t bit = (controller & 0x20) != 0 ? 0 : 1u << (controller & 0x1F);
  return (bit & G::live_mask(base)) != 0;
}

uint64_t SignInPageFor(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls,
                       uint32_t lr, uint32_t signed_in_page) {
  const uint32_t frame = Frame(base, sp, 96);
  if (G::system_ui0(base) != 0 || G::system_ui1(base) != 0) {
    Data(base).set_sign_in_ticks(0);
    return r3;
  }
  if (Data(base).sign_in_ui_shown() == 0) {
    // XShowSigninUI(1, 2) plugged in: no dialog (one local user is always
    // signed in), its answer 0 not read.
    Data(base).set_sign_in_ui_shown(1);
    return 0;
  }
  r3 = calls.Call(kSlotsState, frame, lr, 0);
  uint64_t controller = 0;
  const bool live = uint32_t(r3) != 0 && OnLive(base, &controller);
  const schema::NetModeSelectData data = Data(base);
  const uint32_t count = data.sign_in_ticks();
  data.set_sign_in_ticks(count + 1);
  if (int32_t(count) < 10) {
    return r3;
  }
  Data(base).set_sign_in_ticks(10);
  uint32_t page;
  if (live) {
    page = Data(base).sign_in_continues() != 0 ? signed_in_page : 0;
  } else {
    const schema::NetModeSelectData d = Data(base);
    if (d.sign_in_ui_shown() == 0) {
      return r3;
    }
    d.set_sign_in_ui_shown(0);
    page = 0;
  }
  const PageObject pages = Pages(base);
  KeepPage(pages, page);
  pages.set_page(page);
  pages.set_step(kNone);
  pages.set_queued_step(kNone);
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
  Data(base).set_shown_before(1);
  Data(base).set_back(0);
  Data(base).set_sign_in_continues(1);
  return r3;
}

uint64_t SelectStart(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227D300 NetModeSelect::Start");
  const uint32_t frame = Frame(base, sp, 96);
  const uint64_t r3 = SelectSetUp(base, frame, calls);
  Data(base).set_sign_in_ticks(0);
  PageObject pages = Pages(base);
  pages.set_next_page(0);
  pages.set_page(0);
  pages.set_previous_page(0);
  pages.set_unknown_20(kNone);
  pages.set_step(kNone);
  pages.set_step_started(0);
  pages = Pages(base);
  KeepPage(pages, 0);
  pages.set_step(0);
  pages.set_next_page(0);
  pages.set_page(kNone);
  pages.set_queued_step(1);
  return r3;
}

void SelectOpen(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_8227C5C8 NetModeSelect::Open");
  const NetMachine m(base, machine);
  if (m.loaded() == 0) {
    m.set_state(1);
    return;
  }
  const bool shown = Data(base).shown_before() != 0;
  const PageObject pages = Pages(base);
  KeepPage(pages, 0);
  pages.set_step(shown ? 4 : 0);
  pages.set_queued_step(1);
  pages.set_next_page(0);
  pages.set_page(kNone);
  m.set_state(4);
}

uint64_t SelectDraw(uint8_t* base, uint64_t r3, uint64_t dispatcher, uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227D248 NetModeSelect::Draw");
  const uint32_t frame = Frame(base, sp, 160);
  for (uint32_t k = 0; k < 9; k++) {
    Store(base, frame + 80 + 4 * k, kPageTable[k]);
  }
  const uint32_t page = Pages(base).page();
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
  const uint32_t running = Pages(base).step();
  if (running == kNone) {
    return RunMovie(calls, frame, 0x8227D674, movie);
  }
  uint64_t r3 = movie;
  if (int32_t(running) >= 0) {
    // No bounds check: the word at the table's index.
    r3 = calls.Call(Load(base, frame + 96 + (running << 2)), frame, 0x8227D604,
                    Pages(base).step_started_address());
    if (uint32_t(r3) == 0) {
      return r3;
    }
  }
  const PageObject p = Pages(base);
  const uint32_t queued = p.queued_step();
  if (queued == kNone) {
    const uint32_t next = p.next_page();
    p.set_step(kNone);
    p.set_page(next);
  } else {
    p.set_queued_step(kNone);
    p.set_unknown_20(0);
    p.set_step(queued);
  }
  p.set_step_started(0);
  return r3;
}

uint64_t SelectMenuPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227C658 NetModeSelect::MenuPage");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kMenuStep, frame, 0x8227C670, Menu(base).address);
  uint64_t r3 = calls.Call(kMenuPressed, frame, 0x8227C67C, Menu(base).address, kAccept);
  if (uint32_t(r3) != 0) {
    r3 = calls.Call(kMenuSoundPlay, frame, 0x8227C694, 0);
    const uint32_t item = Menu(base).cursor();
    if (item == 0 || item == 1) {
      Store(base, G::screen1_data(base).address + 8, 1);
      if (item == 1) {
        r3 = SessionMake(base, r3, frame, calls);
      }
      const uint32_t page = item == 0 ? 4 : 5;
      const PageObject pages = Pages(base);
      JumpTo(pages, page);
      pages.set_queued_step(kNone);
    }
  } else {
    r3 = calls.Call(kMenuPressed, frame, 0x8227C71C, Menu(base).address, kBack);
    if (uint32_t(r3) != 0) {
      r3 = calls.Call(kMenuSoundPlay, frame, 0x8227C72C, 2);
      const PageObject pages = Pages(base);
      KeepPage(pages, 8);
      pages.set_page(kNone);
      pages.set_step(3);
      pages.set_next_page(8);
      pages.set_queued_step(kNone);
    }
  }
  if (Menu(base).moved() != 0) {
    r3 = calls.Call(kMenuSoundPlay, frame, 0x8227C770, 4);
    const PageObject pages = Pages(base);
    KeepPage(pages, 0);
    pages.set_queued_step(kNone);
    pages.set_page(kNone);
    pages.set_step(2);
    pages.set_next_page(0);
  }
  return r3;
}

void SelectLeavePage(uint8_t* base, uint32_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_8227C970 NetModeSelect::LeavePage");
  const PageObject pages = Pages(base);
  KeepPage(pages, 3);
  pages.set_step(kNone);
  pages.set_queued_step(kNone);
  pages.set_page(3);
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
  Data(base).set_privilege_dialog(1);
  G::set_dialog12_reason(base, 10);
  Post(base, dispatcher, 18);
  const PageObject pages = Pages(base);
  KeepPage(pages, 3);
  pages.set_step(kNone);
  pages.set_page(3);
  pages.set_queued_step(kNone);
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
  G::session(base).set_privilege(kPrivilege != 0 ? 1 : 0);
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
  Data(base).set_back_to_title(1);
  Data(base).set_back(1);
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
    calls.Call(kFadeIn, frame, 0x8227CEE8, G::fades_passes(base), 0, 0, 0);
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
    Play(calls, frame, 0x8227D1A0, movie, uint64_t(Menu(base).cursor()) + 5);
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
  schema::UiMovieNode(base, node).set_y(value);
  return node;
}

uint64_t SelectMoveCursor(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227D380 NetModeSelect::MoveCursor");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, frame + 80);
  if (Load(base, started) == 0) {
    const schema::FrontEndMenu menu = Menu(base);
    const uint32_t before = menu.previous(), item = menu.cursor();
    // The rows from the words as loaded (zero-extended), as the original adds.
    auto row = [](uint32_t i) { return uint64_t(i) + 5; };
    calls.Call(kFinishNamed, frame, 0x8227D3D4, movie, row(before));
    calls.Call(kFinishNamed, frame, 0x8227D3E4, movie, row(item));
    Play(calls, frame, 0x8227D400, movie, row(item));
    calls.Call(kLoopLast, frame, 0x8227D408, movie);
    calls.Call(kFinishNamed, frame, 0x8227D414, movie, 15);
    calls.Call(kFinishNamed, frame, 0x8227D420, movie, 16);
    const uint32_t direction = Menu(base).moved();
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
    calls.Call(kFadeOut, frame, 0x8227D0E4, G::fades_out(base), 0, 0, 0);
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
    calls.Call(kFadeIn, frame, 0x8227D000, G::fades_passes(base), 0, 0, 0);
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
  const uint32_t page = Menu(base).cursor() == 0 ? 7 : 6;
  Data(base).set_dialog_page(page);
  Data(base).set_dialog_transition(kNone);
}

void SelectOnYes(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8227BFB8 NetModeSelect::OnYes");
  Data(base).set_dialog_page(0);
  Data(base).set_dialog_transition(kNone);
}

void SelectOnDialogClosed(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8227BFD8 NetModeSelect::OnDialogClosed");
  schema::NetModeSelectData data = Data(base);
  if (data.privilege_dialog() != 0 && Menu(base).cursor() == 0) {
    data.set_dialog_page(0);
    Data(base).set_dialog_transition(kNone);
    data = Data(base);
  }
  const uint32_t page = data.dialog_page();
  const uint32_t transition = data.dialog_transition();
  const PageObject pages = Pages(base);
  KeepPage(pages, page);
  pages.set_queued_step(kNone);
  if (transition == kNone) {
    pages.set_page(page);
    pages.set_step(kNone);
  } else {
    pages.set_step(transition);
    pages.set_next_page(page);
    pages.set_page(kNone);
  }
  Data(base).set_dialog_page(kNone);
  Data(base).set_dialog_transition(kNone);
  Data(base).set_privilege_dialog(0);
}

uint64_t SelectUnload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227C7A8 NetModeSelect::Unload");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80);
  uint64_t r3 = calls.Call(kFinishAll, frame, 0x8227C7DC, movie);
  if (NetMachine(base, machine).delete_movie() != 0) {
    r3 = calls.Call(kMoviesDelete, frame, 0x8227C7F0, kSelectMovie);
    NetMachine(base, machine).set_loaded(0);
  }
  return r3;
}

void SelectUnloaded(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8227C810 NetModeSelect::Unloaded");
  if (Data(base).back_to_title() != 0) {
    NetMachine(base, machine).set_state(14);
    return;
  }
  const uint32_t item = Menu(base).cursor();
  uint32_t id;
  if (item <= 1) {
    id = item == 0 ? 6 : 7;
    G::set_sign_in_live_check(base, 0);
    NetMachine(base, machine).set_state(9);
  } else {
    // Never in play (two items): the caller's frame word the original
    // leaves in r9.
    id = Load(base, sp - 16);
  }
  Post(base, dispatcher, id);
}

}  // namespace network_mode
