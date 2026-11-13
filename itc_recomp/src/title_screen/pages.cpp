// The title screen's menu pages (docs/functions/title-screen.md): see
// title_screen.h. Their debug prints (the empty sub_82106540) are left out;
// the strings' addresses they store in their frames are written.

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

// The debug prints' strings each page stores in its frame (+80 on).
constexpr uint32_t kMainStrings[7] = {0x82010734, 0x82010720, 0x82010710, 0x82010700,
                                      0x820106F8, 0x820106E8, 0x8200A18C};
constexpr uint32_t kCareerStrings[3] = {0x8201069C, 0x8200CBDC, 0x82010690};
constexpr uint32_t kMenu4Strings[2] = {0x820106D8, 0x820106C4};

// The page object to `page` through `transition`: +8 the old page unless
// it's `page` already, +12, +16 -1, +4, +0 -1.
void Through(uint8_t* base, uint32_t transition, uint32_t page) {
  const uint32_t pages = Load(base, kPages);
  const uint32_t old = Load(base, pages + 0);
  if (old != page) {
    Store(base, pages + 8, old);
  }
  Store(base, pages + 12, transition);
  Store(base, pages + 16, kNone);
  Store(base, pages + 4, page);
  Store(base, pages + 0, kNone);
}

}  // namespace

uint64_t MainMenu(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82222700 TitleScreen::MainMenu");
  const uint32_t frame = Frame(base, sp, 144);
  for (uint32_t k = 0; k < 7; k++) {
    Store(base, frame + 80 + 4 * k, kMainStrings[k]);
  }
  calls.Call(kListUpdate, frame, 0x822227C4, Load(base, kList));
  const uint32_t list = Load(base, kList);
  uint32_t sound = 0;
  if (Load(base, list + 24) != 0) {
    Through(base, 7, 2);
    sound = 4;
  } else {
    const uint64_t accepted = calls.Call(kListAccepted, frame, 0x82222814, list);
    if (uint32_t(accepted) == 0) {
      return accepted;
    }
    const uint32_t s = race_screens::ListSelection(base, Load(base, kList));
    if (s >= 7) {
      return s;
    }
    bool leave = false;  // items 1, 2 and 5: the item kept, then the page
    switch (s) {
      case 0:
        Through(base, 11, 3);
        Store(base, Load(base, kPages) + 16, 12);
        Store(base, Words(base) + 4, race_screens::ListSelection(base, Load(base, kList)));
        break;
      case 3:
      case 6:
        Through(base, 8, 5);
        Store(base, Words(base) + 4, race_screens::ListSelection(base, Load(base, kList)));
        break;
      case 4: {
        Store(base, Words(base) + 4, race_screens::ListSelection(base, Load(base, kList)));
        const uint32_t pages = Load(base, kPages);
        const uint32_t old = Load(base, pages + 0);
        if (old != kNone) {
          Store(base, pages + 8, old);
        }
        Store(base, pages + 0, kNone);
        Store(base, pages + 12, kNone);
        Store(base, pages + 16, kNone);
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
      Store(base, Words(base) + 4, race_screens::ListSelection(base, Load(base, kList)));
      Through(base, 8, 5);
    }
  }
  return calls.Call(kMenuSoundPlay, frame, 0x822229C8, sound);
}

uint64_t CareerMenu(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82221AC0 TitleScreen::CareerMenu");
  const uint32_t frame = Frame(base, sp, 128);
  for (uint32_t k = 0; k < 3; k++) {
    Store(base, frame + 80 + 4 * k, kCareerStrings[k]);
  }
  calls.Call(kListUpdate, frame, 0x82221B58, Load(base, kList));
  const uint32_t list = Load(base, kList);
  uint32_t sound = 0;
  if (Load(base, list + 24) != 0) {
    Through(base, 13, 3);
    sound = 4;
  } else if (uint32_t(calls.Call(kListAccepted, frame, 0x82221B94, list)) == 0) {
    const uint64_t cancelled = calls.Call(kListCancelled, frame, 0x82221CA0, Load(base, kList));
    if (uint32_t(cancelled) == 0) {
      return cancelled;
    }
    Through(base, 14, 2);
    sound = 2;
  } else {
    const uint32_t s = race_screens::ListSelection(base, Load(base, kList));
    if (s >= 3) {
      return s;
    }
    if (s != 0 && Load(base, Words(base) + 8) == 0) {
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
  for (uint32_t k = 0; k < 2; k++) {
    Store(base, frame + 80 + 4 * k, kMenu4Strings[k]);
  }
  calls.Call(kMenuStep, frame, 0x82222400, Load(base, kMenu));
  uint64_t r3 = calls.Call(kMenuPressed, frame, 0x8222240C, Load(base, kMenu), 159);
  if (uint32_t(r3) != 0) {
    Through(base, 8, 5);
    return r3;
  }
  r3 = calls.Call(kMenuPressed, frame, 0x82222444, Load(base, kMenu), 158);
  if (uint32_t(r3) != 0) {
    Through(base, 16, 2);
  } else if (Load(base, Load(base, kMenu) + 8) != 0) {
    Through(base, 17, 4);
  }
  return r3;
}

}  // namespace title_screen
