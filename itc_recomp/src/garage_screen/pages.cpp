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
  calls.Call(kListUpdate, frame, 0x82194094, List(base));
  uint32_t list = List(base);
  uint32_t page;
  uint32_t sound;
  if (MenuList(base, list).move() == 0 &&
      MenuList(base, list).scroll() == MenuList(base, list).scroll_before()) {
    const uint64_t accepted = calls.Call(kListAccepted, frame, 0x821940B8, list);
    list = List(base);
    if (uint32_t(accepted) != 0) {
      // Into the car's menu: the selected car, transition 4 then 5, page 1.
      const uint32_t selection = race_screens::ListSelection(base, list);
      Data(base).set_selection(selection);
      Data(base).set_scroll(ListView(base).scroll());
      ReloadStandCar(base, frame, calls);
      const PageObject pages = PagesOf(base);
      const uint32_t before = pages.page();
      if (before != 1) {
        pages.set_previous_page(before);
      }
      pages.set_step(4);
      pages.set_queued_step(5);
      pages.set_page(kNone);
      pages.set_next_page(1);
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
  PagesGo(PagesOf(base), page, page == 4 ? 3 : 2, kNone);
  return calls.Call(kMenuSoundPlay, frame, 0x82194194, sound);
}

void LeavePage(uint8_t* base, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82192C18 GarageScreen::LeavePage");
  Stage(base).set_reflection(1);
  Post(base, mode, 22);
}

uint64_t UpdatePage(uint8_t* base, uint64_t r3, uint64_t mode, uint64_t r5, uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82194EA8 GarageScreen::UpdatePage");
  const uint32_t frame = Frame(base, sp, 112);
  for (uint32_t i = 0; i < 5; i++) {
    Store(base, frame + 80 + 4 * i, kPageTable[i]);
  }
  const uint32_t page = PagesOf(base).page();
  if (page == kNone) {
    return r3;
  }
  // No bounds check, as the original: the word at the table's index. r6 to
  // r10 as the original leaves them (the table's build).
  const uint32_t target = Load(base, frame + 80 + (page << 2));
  // The pages' plain C++ by the word's address (all five native); a word
  // past the table through the guest.
  switch (target) {
    case 0x82194078: return CarListPage(base, frame, calls);
    case 0x821941A8: return ActionsPage(base, mode, frame, calls);
    case 0x82191DE0: return ViewPage(base, mode, frame, calls);
    case 0x82192030: return ViewOptionsPage(base, mode, frame, calls);
    case 0x82192C18: LeavePage(base, uint32_t(mode)); return mode;
    default: break;
  }
  return calls.CallArgs(target, frame, 0x82194F1C,
                        {mode, mode, r5, kPageTable[0], kPageTable[1], kPageTable[2],
                         kPageTable[3], kPageTable[4]});
}

}  // namespace garage_screen
