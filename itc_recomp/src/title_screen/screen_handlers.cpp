// The title screen's machine handlers (docs/functions/title-screen.md): see
// title_screen.h.

#include "base/log.h"
#include "race_screens/race_screens.h"
#include "title_screen/internal.h"

namespace title_screen {

namespace {

// The pages' table in Draw's frame: PressStart (TitleScreen::Update),
// LoadGameData, MainMenu, CareerMenu, Menu4, LeavePage.
constexpr uint32_t kPageTable[6] = {0x822221A0, 0x82222688, 0x82222700,
                                    0x82221AC0, 0x82222378, 0x822229D0};
// The transitions' table in Update's frame (+96 on): 0 Intro to 17
// MoveMenu4Cursor; 8 the type 2 hub's fade out.
constexpr uint32_t kTransitionTable[18] = {
    0x82222B60, 0x82222BF8, 0x82222C90, 0x82222D40, 0x82223B78, 0x82223C38,
    0x82222DD0, 0x82222E88, 0x8218ED98, 0x82223050, 0x822231B0, 0x82223D70,
    0x82223328, 0x82223428, 0x82223E58, 0x82223F30, 0x82223FD8, 0x822235D0};
// The main menu's texts.
constexpr uint32_t kMainTexts[7] = {248, 249, 250, 257, 251, 252, 253};

}  // namespace

uint64_t ScreenCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82221D98 TitleScreen::Create");
  const uint32_t frame = Frame(base, sp, 112);
  const Machine m(base, machine);
  m.set_loaded(0);
  // Host objects (E5 step 5) in place of the VirtualAlloc(28) at 0x82221DC4,
  // the VirtualAlloc(72) at 0x82221E00 and the VirtualAlloc(68) at
  // 0x82221E2C; the globals hold their handles.
  const schema::PageObject pages = schema::PageObject::New(base);
  G::set_pages(base, pages);
  pages.set_next_page(0);
  pages.set_page(0);
  pages.set_previous_page(0);
  pages.set_step_started(0);
  pages.set_unknown_20(kNone);
  pages.set_step(kNone);
  // The menu list (race-screens' MenuList).
  const schema::MenuList list = schema::MenuList::New(base);
  G::set_list(base, list);
  if (list.address != 0) {
    list.set_enabled(0u);
    list.set_player(0);
  }
  const schema::FrontEndMenu menu = schema::FrontEndMenu::New(base);
  G::set_menu(base, menu);
  ResetMenu(menu);
  // VirtualAlloc(68) in host memory (E5 step 5); its answer is Create's r3.
  const schema::TitleScreenData words = schema::TitleScreenData::New(base);
  const uint64_t r3 = words.address;
  G::set_data(base, words);
  words.set_unknown_0(0);
  words.set_interrupted(0);
  m.set_state(m.reenter() != 0 ? 1 : 9);
  return r3;
}

uint64_t ScreenOpenMovie(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82221990 TitleScreen::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, kTitleMovie, kTitleAsset);
}

uint64_t ScreenWaitForMovie(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82221FC8 TitleScreen::WaitForMovie");
  const Machine m(base, uint32_t(r3));
  Frame(base, sp, 96);
  if (m.loaded() == 0) {
    r3 = ui_movies::IsLoaded(base, kTitleMovie);
    if (uint32_t(r3) != 0) {
      m.set_loaded(1);
    }
  }
  if (m.loaded() != 0) {
    m.set_state(m.reenter() != 0 ? 9 : 4);
  }
  return r3;
}

uint64_t ScreenSetUp(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82222A50 TitleScreen::SetUp");
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 80);
  for (uint32_t i = 0; i < 7; i++) {
    Store(base, frame + 96 + 4 * i, kMainTexts[i]);
  }
  calls.Call(kResetPose, frame, 0x82222AB8, movie);
  uint64_t r3 = 0;
  for (uint32_t i = 0; i < 7; i++) {
    // r7 to r10 unset in the original: 0 (the user's call, 2026-10-30).
    r3 = calls.Call(kSetText, frame, 0x82222AD4, movie, 4, i + 5, Load(base, frame + 96 + 4 * i));
  }
  const uint32_t from_boot = Words(base).from_boot();
  const auto pages = G::pages(base);
  const uint32_t page = pages.page();
  if (from_boot != 0) {
    if (page != 0) {
      pages.set_previous_page(page);
    }
    pages.set_step(0);
    pages.set_next_page(0);
    pages.set_queued_step(1);
    pages.set_page(kNone);
  } else {
    if (page != 2) {
      pages.set_previous_page(page);
    }
    pages.set_step(5);
    pages.set_queued_step(6);
    pages.set_next_page(2);
    pages.set_page(kNone);
  }
  return r3;
}

uint64_t ScreenStart(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223AB8 TitleScreen::Start");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t entered = G::entered(base);
  if (entered == 0) {
    Words(base).set_from_boot(0);
  } else if (entered == 1) {
    Words(base).set_from_boot(1);
  }
  ScreenSetUp(base, frame, calls);
  Store(base, frame + 80, 1);
  Store(base, frame + 84, 1);
  const uint32_t manager = G::sound_manager(base);
  const uint64_t playing = calls.Call(Load(base, Load(base, manager) + kIsCuePlayingSlot), frame,
                                      0x82223B2C, manager, frame + 84, frame + 80);
  if ((playing & 0xFF) == 0) {
    Store(base, frame + 88, 1);
    Store(base, frame + 92, 1);
    calls.Call(kSoundPlay, frame, 0x82223B48, frame + 92, frame + 88);
  }
  return calls.CallArgs(kFadeIn, frame, 0x82223B64,
                        {G::fades_passes(base), 0, 0, 0, 0, 0, 0, 0});
}

uint64_t ScreenDraw(uint8_t* base, uint64_t r3, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223A18 TitleScreen::Draw");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t page = G::pages(base).page();
  for (uint32_t k = 0; k < 6; k++) {
    Store(base, frame + 80 + 4 * k, kPageTable[k]);
  }
  if (page == kNone) {
    return r3;
  }
  // No bounds check, as the original: the word at the table's index.
  return calls.Call(Load(base, frame + 80 + (page << 2)), frame, 0x82223AA4, mode, r3);
}

uint64_t ScreenUpdate(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822240B0 TitleScreen::Update");
  // The empty sub_82106540(100, 30, 0, string) the original calls first is
  // left out: it returns at once.
  const uint32_t frame = Frame(base, sp, 288);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 80);
  const auto pages = G::pages(base);
  const uint32_t running = pages.step();
  for (uint32_t k = 0; k < 18; k++) {
    Store(base, frame + 96 + 4 * k, kTransitionTable[k]);
  }
  if (running == kNone) {
    return calls.Call(kRun, frame, 0x82224264, movie, 0);
  }
  uint64_t r3 = movie;
  if (int32_t(running) >= 0) {
    // No bounds check: the word at the table's index.
    r3 = calls.Call(Load(base, frame + 96 + (running << 2)), frame, 0x822241FC,
                    pages.step_started_address());
    if (uint32_t(r3) == 0) {
      return r3;
    }
  }
  const auto p = G::pages(base);
  const uint32_t queued = p.queued_step();
  if (queued == kNone) {
    if (int32_t(running) >= 0) {
      p.set_page(p.next_page());
      p.set_step(queued);
    } else {
      const uint32_t next = p.next_page();
      p.set_step(kNone);
      p.set_page(next);
    }
  } else {
    p.set_queued_step(kNone);
    p.set_unknown_20(0);
    p.set_step(queued);
  }
  p.set_step_started(0);
  return r3;
}

void ScreenOnGameDataLoaded(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_822219A0 TitleScreen::OnGameDataLoaded");
  const auto pages = G::pages(base);
  const uint32_t page = pages.page();
  if (page != 2) {
    pages.set_previous_page(page);
  }
  pages.set_step(4);
  pages.set_queued_step(6);
  pages.set_next_page(2);
  pages.set_page(kNone);
}

void ScreenOnInterrupt(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82221A78 TitleScreen::OnInterrupt");
  Words(base).set_interrupted(1);
}

void ScreenOnResume(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82221A90 TitleScreen::OnResume");
  Words(base).set_interrupted(0);
  for (uint32_t k = 0; k < 4; k++) {
    Store(base, G::system_checks_address() + 4 * k, 0);
  }
}

uint64_t ScreenUnload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822224D0 TitleScreen::Unload");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 80);
  uint64_t r3 = calls.Call(kFinishAll, frame, 0x82222508, movie);
  const Machine m(base, machine);
  if (m.delete_movie() != 0) {
    r3 = calls.Call(kMoviesDelete, frame, 0x8222251C, kTitleMovie);
    m.set_loaded(0);
  }
  return r3;
}

uint64_t ScreenOpenNext(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82222538 TitleScreen::OpenNext");
  Frame(base, sp, 112);
  const uint32_t selection = race_screens::ListSelection(base, ListAddress(base));
  if (selection == 6) {
    Post(base, mode, 10);
  }
  Machine(base, machine).set_state(9);
  return selection;
}

uint64_t ScreenLeave(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp,
                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822225D8 TitleScreen::Leave");
  const uint32_t frame = Frame(base, sp, 112);
  Machine(base, machine).set_state(7);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 80);
  uint64_t r3 = movie;
  if (movie != 0) {
    calls.Call(kFinishAll, frame, 0x82222620, movie);
    r3 = calls.Call(kMoviesDelete, frame, 0x82222628, kTitleMovie);
  }
  Post(base, mode, 4);
  return r3;
}

uint64_t ScreenDestroy(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82221EC0 TitleScreen::Destroy");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t item = Words(base).main_item();
  uint32_t state = 0;
  bool set = true;
  switch (item) {
    case 0:
      state = race_screens::ListSelection(base, ListAddress(base)) == 2 ? 20 : 13;
      break;
    case 1: state = 16; break;
    case 2: state = 17; break;
    case 3: state = 15; break;
    case 4: state = 18; break;
    case 5: state = 19; break;
    case 8: state = 11; break;
    default: set = false; break;
  }
  if (set) {
    Machine(base, machine).set_state(state);
  }
  calls.Call(kListRelease, frame, 0x82221F60, ListAddress(base));
  calls.Call(kVirtualFree, frame, 0x82221F7C, G::pages(base).address, 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x82221F8C, ListAddress(base), 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x82221FA0, G::menu(base).address, 0, kMemRelease);
  const uint64_t r3 =
      calls.Call(kVirtualFree, frame, 0x82221FB0, Words(base).address, 0, kMemRelease);
  G::set_pages(base, 0u);
  G::set_list(base, 0u);
  G::set_data(base, 0u);
  return r3;
}

void ScreenLoadGameData(uint8_t* base, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82222688 TitleScreen::LoadGameData");
  Post(base, mode, 13);
  const auto pages = G::pages(base);
  const uint32_t page = pages.page();
  if (page != kNone) {
    pages.set_previous_page(page);
  }
  pages.set_page(kNone);
  pages.set_step(kNone);
  pages.set_queued_step(kNone);
}

void ScreenLeavePage(uint8_t* base, uint32_t mode, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_822229D0 TitleScreen::LeavePage");
  const uint32_t item = Words(base).main_item();
  if (item == 6) {
    Post(base, mode, 7);
  } else if (item < 6 || item == 8) {
    Machine(base, machine).set_state(10);
  }
}

}  // namespace title_screen
