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
  Store(base, machine + kMachineLoaded, 0);
  const uint32_t pages = uint32_t(calls.Call(kVirtualAlloc, frame, 0x82221DC4, 0, 28, 0x1000, 4));
  Store(base, kPages, pages);
  Store(base, pages + 4, 0);
  Store(base, pages + 0, 0);
  Store(base, pages + 8, 0);
  Store(base, pages + 24, 0);
  Store(base, pages + 20, kNone);
  Store(base, pages + 12, kNone);
  const uint32_t list = uint32_t(calls.Call(kVirtualAlloc, frame, 0x82221E00, 0, 72, 0x1000, 4));
  Store(base, kList, list);
  if (list != 0) {
    Store(base, list + 68, 0);
    Store(base, list + 60, 0);
  }
  const uint32_t menu = uint32_t(calls.Call(kVirtualAlloc, frame, 0x82221E2C, 0, 68, 0x1000, 4));
  Store(base, kMenu, menu);
  Store(base, menu + 12, 0);
  Store(base, menu + 16, 1);
  Store(base, menu + 20, 1);
  for (const uint32_t field : {0u, 4u, 8u, 40u, 48u, 52u, 36u, 64u, 24u, 28u, 32u}) {
    Store(base, menu + field, 0);
  }
  const uint64_t r3 = calls.Call(kVirtualAlloc, frame, 0x82221E88, 0, 68, 0x1000, 4);
  const uint32_t words = uint32_t(r3);
  Store(base, kWords, words);
  Store(base, words + 0, 0);
  Store(base, words + 40, 0);
  Store(base, machine + kMachineState, Load(base, machine + kMachineReenter) != 0 ? 1 : 9);
  return r3;
}

uint64_t ScreenOpenMovie(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82221990 TitleScreen::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, kTitleMovie, kTitleAsset);
}

uint64_t ScreenWaitForMovie(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82221FC8 TitleScreen::WaitForMovie");
  const uint32_t machine = uint32_t(r3);
  Frame(base, sp, 96);
  if (Load(base, machine + kMachineLoaded) == 0) {
    r3 = ui_movies::IsLoaded(base, kTitleMovie);
    if (uint32_t(r3) != 0) {
      Store(base, machine + kMachineLoaded, 1);
    }
  }
  if (Load(base, machine + kMachineLoaded) != 0) {
    Store(base, machine + kMachineState, Load(base, machine + kMachineReenter) != 0 ? 9 : 4);
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
  const uint32_t from_boot = Load(base, Load(base, kWords) + 16);
  const uint32_t pages = Load(base, kPages);
  const uint32_t page = Load(base, pages + 0);
  if (from_boot != 0) {
    if (page != 0) {
      Store(base, pages + 8, page);
    }
    Store(base, pages + 12, 0);
    Store(base, pages + 4, 0);
    Store(base, pages + 16, 1);
    Store(base, pages + 0, kNone);
  } else {
    if (page != 2) {
      Store(base, pages + 8, page);
    }
    Store(base, pages + 12, 5);
    Store(base, pages + 16, 6);
    Store(base, pages + 4, 2);
    Store(base, pages + 0, kNone);
  }
  return r3;
}

uint64_t ScreenStart(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223AB8 TitleScreen::Start");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t entered = Load(base, kEntered);
  if (entered == 0) {
    Store(base, Load(base, kWords) + 16, 0);
  } else if (entered == 1) {
    Store(base, Load(base, kWords) + 16, 1);
  }
  ScreenSetUp(base, frame, calls);
  Store(base, frame + 80, 1);
  Store(base, frame + 84, 1);
  const uint32_t manager = Load(base, kSoundManager);
  const uint64_t playing = calls.Call(Load(base, Load(base, manager) + kIsCuePlayingSlot), frame,
                                      0x82223B2C, manager, frame + 84, frame + 80);
  if ((playing & 0xFF) == 0) {
    Store(base, frame + 88, 1);
    Store(base, frame + 92, 1);
    calls.Call(kSoundPlay, frame, 0x82223B48, frame + 92, frame + 88);
  }
  return calls.CallArgs(kFadeIn, frame, 0x82223B64,
                        {Load(base, kFadesPasses), 0, 0, 0, 0, 0, 0, 0});
}

uint64_t ScreenDraw(uint8_t* base, uint64_t r3, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82223A18 TitleScreen::Draw");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t page = Load(base, Load(base, kPages) + 0);
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
  const uint32_t pages = Load(base, kPages);
  const uint32_t running = Load(base, pages + 12);
  for (uint32_t k = 0; k < 18; k++) {
    Store(base, frame + 96 + 4 * k, kTransitionTable[k]);
  }
  if (running == kNone) {
    return calls.Call(kRun, frame, 0x82224264, movie, 0);
  }
  uint64_t r3 = movie;
  if (int32_t(running) >= 0) {
    // No bounds check: the word at the table's index.
    r3 = calls.Call(Load(base, frame + 96 + (running << 2)), frame, 0x822241FC, pages + 24);
    if (uint32_t(r3) == 0) {
      return r3;
    }
  }
  const uint32_t p = Load(base, kPages);
  const uint32_t queued = Load(base, p + 16);
  if (queued == kNone) {
    if (int32_t(running) >= 0) {
      Store(base, p + 0, Load(base, p + 4));
      Store(base, p + 12, queued);
    } else {
      const uint32_t next = Load(base, p + 4);
      Store(base, p + 12, kNone);
      Store(base, p + 0, next);
    }
  } else {
    Store(base, p + 16, kNone);
    Store(base, p + 20, 0);
    Store(base, p + 12, queued);
  }
  Store(base, p + 24, 0);
  return r3;
}

void ScreenOnGameDataLoaded(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_822219A0 TitleScreen::OnGameDataLoaded");
  const uint32_t pages = Load(base, kPages);
  const uint32_t page = Load(base, pages + 0);
  if (page != 2) {
    Store(base, pages + 8, page);
  }
  Store(base, pages + 12, 4);
  Store(base, pages + 16, 6);
  Store(base, pages + 4, 2);
  Store(base, pages + 0, kNone);
}

void ScreenOnInterrupt(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82221A78 TitleScreen::OnInterrupt");
  Store(base, Load(base, kWords) + 40, 1);
}

void ScreenOnResume(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82221A90 TitleScreen::OnResume");
  Store(base, Load(base, kWords) + 40, 0);
  for (uint32_t k = 0; k < 4; k++) {
    Store(base, kSystemChecks + 4 * k, 0);
  }
}

uint64_t ScreenUnload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822224D0 TitleScreen::Unload");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kTitleMovie, frame + 80);
  uint64_t r3 = calls.Call(kFinishAll, frame, 0x82222508, movie);
  if (Load(base, machine + kMachineDelete) != 0) {
    r3 = calls.Call(kMoviesDelete, frame, 0x8222251C, kTitleMovie);
    Store(base, machine + kMachineLoaded, 0);
  }
  return r3;
}

uint64_t ScreenOpenNext(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82222538 TitleScreen::OpenNext");
  Frame(base, sp, 112);
  const uint32_t selection = race_screens::ListSelection(base, Load(base, kList));
  if (selection == 6) {
    Post(base, mode, 10);
  }
  Store(base, machine + kMachineState, 9);
  return selection;
}

uint64_t ScreenLeave(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp,
                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822225D8 TitleScreen::Leave");
  const uint32_t frame = Frame(base, sp, 112);
  Store(base, machine + kMachineState, 7);
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
  const uint32_t item = Load(base, Load(base, kWords) + 4);
  uint32_t state = 0;
  bool set = true;
  switch (item) {
    case 0:
      state = race_screens::ListSelection(base, Load(base, kList)) == 2 ? 20 : 13;
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
    Store(base, machine + kMachineState, state);
  }
  calls.Call(kListRelease, frame, 0x82221F60, Load(base, kList));
  calls.Call(kVirtualFree, frame, 0x82221F7C, Load(base, kPages), 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x82221F8C, Load(base, kList), 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x82221FA0, Load(base, kMenu), 0, kMemRelease);
  const uint64_t r3 =
      calls.Call(kVirtualFree, frame, 0x82221FB0, Load(base, kWords), 0, kMemRelease);
  Store(base, kPages, 0);
  Store(base, kList, 0);
  Store(base, kWords, 0);
  return r3;
}

void ScreenLoadGameData(uint8_t* base, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82222688 TitleScreen::LoadGameData");
  Post(base, mode, 13);
  const uint32_t pages = Load(base, kPages);
  const uint32_t page = Load(base, pages + 0);
  if (page != kNone) {
    Store(base, pages + 8, page);
  }
  Store(base, pages + 0, kNone);
  Store(base, pages + 12, kNone);
  Store(base, pages + 16, kNone);
}

void ScreenLeavePage(uint8_t* base, uint32_t mode, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_822229D0 TitleScreen::LeavePage");
  const uint32_t item = Load(base, Load(base, kWords) + 4);
  if (item == 6) {
    Post(base, mode, 7);
  } else if (item < 6 || item == 8) {
    Store(base, machine + kMachineState, 10);
  }
}

}  // namespace title_screen
