// The title screen's menu pages (docs/functions/title-screen.md): see
// title_screen.h. Their debug prints (the empty sub_82106540) are left out,
// with the strings' addresses they stored in their frames (+80 on: read by
// nothing, dropped in E5 step 6; the frames stay for the guest calls' r1).

#include "base/log.h"
#include "race_screens/race_screens.h"
#include "title_screen/internal.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace title_screen {

namespace {

constexpr uint32_t kListUpdate = 0x8220C1E0;
constexpr uint32_t kListAccepted = 0x8220BD50;
constexpr uint32_t kListCancelled = 0x8220BDE0;
constexpr uint32_t kSlotsState = 0x8228F9E8;
constexpr uint32_t kMenuSoundPlay = 0x821103F0;
constexpr uint32_t kMenuStep = 0x82186568;
constexpr uint32_t kMenuPressed = 0x821864C8;
constexpr uint32_t kProgressClear = 0x82113BD8;
constexpr uint32_t kProgressSetDefaults = 0x82113E60;
constexpr uint32_t kProgressRestart = 0x82114128;
constexpr uint32_t kPlayTimeMark = 0x82111628;
constexpr uint32_t kCareerPickFromCar = 0x82115CC0;
constexpr uint32_t kCareerNextDay = 0x82116AF0;
constexpr uint32_t kStartFirstRace = 0x82180CE8;

// The page object to `page` through `transition`: +8 the old page unless
// it's `page` already, +12, +16 -1, +4, +0 -1.
void Through(uint8_t* base, uint32_t transition, uint32_t page) {
  const auto pages = G::pages(base);
  const uint32_t old = pages.page();
  if (old != page) {
    pages.set_previous_page(old);
  }
  pages.set_step(transition);
  pages.set_queued_step(kNone);
  pages.set_next_page(page);
  pages.set_page(kNone);
}

// The title's +4 from the list's selection: the selection first, then the
// title's pointer, as the original.
void SetMainItem(uint8_t* base) {
  const uint32_t selection = race_screens::ListSelection(base, ListAddress(base));
  Words(base).set_main_item(selection);
}

}  // namespace

uint64_t MainMenu(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls,
                  bool multiplayer_items) {
  ITC_NATIVE_RUNNING("sub_82222700 TitleScreen::MainMenu");
  const uint32_t frame = Frame(base, sp, 144);
  calls.Call(kListUpdate, frame, 0x822227C4, ListAddress(base));
  const schema::MenuList list = MenuListOf(base);
  uint32_t sound = 0;
  if (list.move() != 0) {
    Through(base, 7, 2);
    sound = 4;
  } else {
    const uint64_t accepted = calls.Call(kListAccepted, frame, 0x82222814, list.address);
    if (uint32_t(accepted) == 0) {
      return accepted;
    }
    const uint32_t s = race_screens::ListSelection(base, ListAddress(base));
    if (s >= 7) {
      return s;
    }
    bool leave = false;  // items 1, 2 and 5: the item kept, then the page
    if (!multiplayer_items && (s == 3 || s == 4)) {
      // Multiplayer bypassed (the user's calls, 2026-11-14): Xbox Live (3,
      // the type 12 mode) and split screen (4, the controller pick, then the
      // type 8 mode) are silent, answered as an input the menu ignores (no
      // accept): no sound, the item, page and cursor left as they are, and
      // r3 0, MenuList::Accepted's r3 on that path.
      return 0;
    }
    switch (s) {
      case 0:
        Through(base, 11, 3);
        G::pages(base).set_queued_step(12);
        SetMainItem(base);
        break;
      case 3:
      case 6:
        Through(base, 8, 5);
        SetMainItem(base);
        break;
      case 4: {
        SetMainItem(base);
        const auto pages = G::pages(base);
        const uint32_t old = pages.page();
        if (old != kNone) {
          pages.set_previous_page(old);
        }
        pages.set_page(kNone);
        pages.set_step(kNone);
        pages.set_queued_step(kNone);
        Post(base, mode, 15);
        break;
      }
      case 5:
        if (uint32_t(calls.Call(kSlotsState, frame, 0x82222888, 0)) == 0) {
          sound = 3;
          break;
        }
        leave = true;
        break;
      default:  // 1, 2
        leave = true;
        break;
    }
    if (leave) {
      SetMainItem(base);
      Through(base, 8, 5);
    }
  }
  return calls.Call(kMenuSoundPlay, frame, 0x822229C8, sound);
}

uint64_t CareerMenu(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82221AC0 TitleScreen::CareerMenu");
  const uint32_t frame = Frame(base, sp, 128);
  calls.Call(kListUpdate, frame, 0x82221B58, ListAddress(base));
  const schema::MenuList list = MenuListOf(base);
  uint32_t sound = 0;
  if (list.move() != 0) {
    Through(base, 13, 3);
    sound = 4;
  } else if (uint32_t(calls.Call(kListAccepted, frame, 0x82221B94, list.address)) == 0) {
    const uint64_t cancelled = calls.Call(kListCancelled, frame, 0x82221CA0, ListAddress(base));
    if (uint32_t(cancelled) == 0) {
      return cancelled;
    }
    Through(base, 14, 2);
    sound = 2;
  } else {
    const uint32_t s = race_screens::ListSelection(base, ListAddress(base));
    if (s >= 3) {
      return s;
    }
    if (s != 0 && Words(base).can_continue() == 0) {
      sound = 3;
    } else if (s == 0) {
      const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
      calls.Call(kProgressClear, frame, 0x82221C90, save);
      calls.Call(kProgressSetDefaults, frame, 0x82221C98, save);
      Through(base, 8, 5);
    } else if (s == 1) {
      const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
      calls.Call(kPlayTimeMark, frame, 0x82221C58, save);
      Through(base, 8, 5);
    } else {
      const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
      calls.Call(kProgressRestart, frame, 0x82221BDC, save);
      calls.Call(kCareerPickFromCar, frame, 0x82221BE4, save);
      calls.Call(kCareerNextDay, frame, 0x82221BEC, save);
      Through(base, 8, 5);
      // StartFirstRace reads no argument: r3 MenuSound::Play's, as the
      // original leaves it.
      const uint64_t played = calls.Call(kMenuSoundPlay, frame, 0x82221C28, 0);
      return calls.Call(kStartFirstRace, frame, 0x82221C2C, played);
    }
  }
  return calls.Call(kMenuSoundPlay, frame, 0x82221CE4, sound);
}

uint64_t Menu4(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82222378 TitleScreen::Menu4");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kMenuStep, frame, 0x82222400, G::menu(base).address);
  uint64_t r3 = calls.Call(kMenuPressed, frame, 0x8222240C, G::menu(base).address, 159);
  if (uint32_t(r3) != 0) {
    Through(base, 8, 5);
    return r3;
  }
  r3 = calls.Call(kMenuPressed, frame, 0x82222444, G::menu(base).address, 158);
  if (uint32_t(r3) != 0) {
    Through(base, 16, 2);
  } else if (G::menu(base).moved() != 0) {
    Through(base, 17, 4);
  }
  return r3;
}

}  // namespace title_screen
