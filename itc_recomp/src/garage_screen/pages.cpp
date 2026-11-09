// The garage screen's pages seen in play and the page table's caller
// (docs/functions/garage-screen.md): see garage_screen.h. The pages never
// seen in play (ActionsPage, ViewPage, ViewOptionsPage) stay the originals.

#include "garage_screen/garage_screen.h"

#include "base/log.h"
#include "garage_screen/internal.h"
#include "race_screens/race_screens.h"

namespace garage_screen {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;

// The pages' table in UpdatePage's frame: CarListPage, ActionsPage,
// ViewPage, ViewOptionsPage, LeavePage.
constexpr uint32_t kPageTable[5] = {0x82194078, 0x821941A8, 0x82191DE0, 0x82192030,
                                    0x82192C18};

}  // namespace

uint64_t CarListPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82194078 GarageScreen::CarListPage");
  const uint32_t frame = Frame(base, sp, 96);
  calls.Call(kListUpdate, frame, 0x82194094, Load(base, kList));
  uint32_t list = Load(base, kList);
  uint32_t page;
  uint32_t sound;
  if (Load(base, list + 24) == 0 && Load(base, list + 4) == Load(base, list + 8)) {
    const uint64_t accepted = calls.Call(kListAccepted, frame, 0x821940B8, list);
    list = Load(base, kList);
    if (uint32_t(accepted) != 0) {
      // Into the car's menu: the selected car, transition 4 then 5, page 1.
      const uint32_t selection = race_screens::ListSelection(base, list);
      Store(base, Data(base) + 44, selection);
      Store(base, Data(base) + 48, Load(base, Load(base, kList) + 4));
      calls.Call(kReloadStandCar, frame, 0x821940E8);
      const uint32_t pages = Load(base, kPages);
      const uint32_t before = Load(base, pages + 0);
      if (before != 1) {
        Store(base, pages + 8, before);
      }
      Store(base, pages + 12, 4);
      Store(base, pages + 16, 5);
      Store(base, pages + 0, kNone);
      Store(base, pages + 4, 1);
      return calls.Call(kMenuSoundPlay, frame, 0x82194194, 0);
    }
    const uint64_t cancelled = calls.Call(kListCancelled, frame, 0x82194128, list);
    if (uint32_t(cancelled) == 0) {
      return cancelled;
    }
    page = 4;  // the closing (transition 3)
    sound = 2;
  } else {
    page = 0;  // the list moved (transition 2)
    sound = 4;
  }
  PagesGo(base, Load(base, kPages), page, page == 4 ? 3 : 2, kNone);
  return calls.Call(kMenuSoundPlay, frame, 0x82194194, sound);
}

void LeavePage(uint8_t* base, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82192C18 GarageScreen::LeavePage");
  Store(base, Load(base, Holder(base)) + 6052, 1);
  Post(base, mode, 22);
}

uint64_t UpdatePage(uint8_t* base, uint64_t r3, uint64_t mode, uint64_t r5, uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82194EA8 GarageScreen::UpdatePage");
  const uint32_t frame = Frame(base, sp, 112);
  for (uint32_t i = 0; i < 5; i++) {
    Store(base, frame + 80 + 4 * i, kPageTable[i]);
  }
  const uint32_t page = Load(base, Load(base, kPages) + 0);
  if (page == kNone) {
    return r3;
  }
  // No bounds check, as the original: the word at the table's index. r6 to
  // r10 as the original leaves them (the table's build).
  const uint32_t target = Load(base, frame + 80 + (page << 2));
  return calls.CallArgs(target, frame, 0x82194F1C,
                        {mode, mode, r5, kPageTable[0], kPageTable[1], kPageTable[2],
                         kPageTable[3], kPageTable[4]});
}

}  // namespace garage_screen
