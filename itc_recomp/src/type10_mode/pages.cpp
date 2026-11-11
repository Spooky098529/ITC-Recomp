// The replay theater's pages and its button prompts
// (docs/functions/type10-mode.md): see type10_mode.h.

#include "base/log.h"
#include "race_screens/race_screens.h"
#include "type10_mode/internal.h"

namespace type10_mode {

namespace {

// The page object to page 5 at once (AskList's and AskLoad's), the data's
// +4 = `asked`.
void AskSaveScreen(uint8_t* base, uint32_t asked) {
  const uint32_t pages = Pages(base);
  KeepPage(base, pages, 5);
  Store(base, pages + 12, kNone);
  Store(base, pages + 0, 5);
  Store(base, pages + 16, kNone);
  Store(base, Data(base) + 4, asked);
}

}  // namespace

uint64_t ListPage(uint8_t* base, uint32_t dispatcher, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82289768 ReplayTheater::ListPage");
  const uint32_t frame = Frame(base, sp, 144);
  if (int32_t(calls.Call(kSlotsState, frame, 0x82289780, 0)) == 1) {
    // The save device lost: to no page, the save screen's kind 5.
    const uint32_t pages = Pages(base);
    KeepPage(base, pages, kNone);
    Store(base, pages + 0, kNone);
    Store(base, pages + 12, kNone);
    Store(base, pages + 16, kNone);
    Store(base, Data(base) + 4, 2);
    Post(base, dispatcher, 9);
  }
  calls.Call(kListUpdate, frame, 0x82289818, Load(base, kList));
  const uint32_t list = Load(base, kList);
  uint32_t sound;
  if (Load(base, list + 24) == 0 && Load(base, list + 4) - Load(base, list + 8) == 0) {
    if (uint32_t(calls.Call(kListAccepted, frame, 0x8228983C, list)) != 0) {
      const uint32_t selection = race_screens::ListSelection(base, Load(base, kList));
      Store(base, Load(base, kSaveScreenData) + 40,
            Load(base, Load(base, kList) + 4) + selection);
      const uint32_t pages = Pages(base);
      KeepPage(base, pages, 2);
      Store(base, pages + 4, 2);
      Store(base, pages + 12, 6);
      Store(base, pages + 16, kNone);
      Store(base, pages + 0, kNone);
      sound = 0;
    } else {
      const uint64_t cancelled = calls.Call(kListCancelled, frame, 0x82289890, Load(base, kList));
      if (uint32_t(cancelled) == 0) {
        return cancelled;
      }
      const uint32_t pages = Pages(base);
      KeepPage(base, pages, 5);
      Store(base, pages + 12, kNone);
      Store(base, pages + 16, kNone);
      Store(base, pages + 0, 5);
      Store(base, Data(base) + 4, 3);
      Post(base, dispatcher, 9);
      sound = 2;
    }
  } else {
    // The list moved: transition 4, page 0 after.
    const uint32_t pages = Pages(base);
    KeepPage(base, pages, 0);
    Store(base, pages + 12, 4);
    Store(base, pages + 4, 0);
    Store(base, pages + 16, kNone);
    Store(base, pages + 0, kNone);
    sound = 4;
  }
  return calls.Call(kMenuSoundPlay, frame, 0x8228994C, sound);
}

void AskList(uint8_t* base, uint32_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_82289958 ReplayTheater::AskList");
  Post(base, dispatcher, 7);
  AskSaveScreen(base, 0);
}

void AskLoad(uint8_t* base, uint32_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_822899E0 ReplayTheater::AskLoad");
  Post(base, dispatcher, 8);
  AskSaveScreen(base, 1);
}

void LeavePage(uint8_t* base, uint32_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_82289A70 ReplayTheater::LeavePage");
  Post(base, dispatcher, 6);
}

uint64_t WaitPage(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8228A4B0 ReplayTheater::WaitPage");
  const uint32_t state = Load(base, Load(base, kSaveScreenData) + 56);
  static constexpr uint32_t kHidden[3] = {kNone, kNone, kNone};
  static constexpr uint32_t kBack[3] = {0, kNone, kNone};
  if (state == 0) {
    return SetPrompts(base, kHidden, kHidden, sp, calls);
  }
  if (state == 1) {
    return SetPrompts(base, kBack, kBack, sp, calls);
  }
  return r3;
}

uint64_t SetPrompts(uint8_t* base, const uint32_t a[3], const uint32_t b[3], uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8228A3B8 ReplayTheater::SetPrompts");
  (void)calls;
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t movie = FindMovie(base, frame + 80);
  Store(base, frame + 112, b[2]);
  Store(base, frame + 92, a[1]);
  Store(base, frame + 96, a[2]);
  Store(base, frame + 104, b[0]);
  Store(base, frame + 108, b[1]);
  Store(base, frame + 88, a[0]);
  uint32_t last = 0;
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t pair = ui_movies::FindNode(base, movie, 2, i);
    const uint32_t first = ui_movies::FindNode(base, movie, 2, Load(base, pair + 100));
    const uint32_t second = ui_movies::FindNode(base, movie, 2, Load(base, pair + 100) + 1);
    last = second;
    const uint32_t value_a = Load(base, frame + 88 + 4 * i);
    const uint32_t value_b = Load(base, frame + 104 + 4 * i);
    if (value_a != kNone && value_b != kNone) {
      Store(base, first + 80, 1);
      Store(base, second + 80, 1);
      Store(base, first + 108, value_a);
      Store(base, second + 108, value_b);
    } else {
      Store(base, first + 80, 0);
      Store(base, second + 80, 0);
    }
  }
  return last;
}

}  // namespace type10_mode
