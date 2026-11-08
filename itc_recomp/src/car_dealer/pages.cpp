// The car dealer's pages (docs/functions/car-dealer.md): see car_dealer.h.

#include "car_dealer/car_dealer.h"

#include "base/log.h"
#include "car_dealer/internal.h"
#include "guest.h"

namespace car_dealer {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;

// The page object's page before (+8) kept unless the page (+0) is `unless`;
// the page object returned.
uint32_t Keep(uint8_t* base, uint32_t unless) {
  const uint32_t pages = Load(base, kPages);
  const uint32_t page = Load(base, pages + 0);
  if (page != unless) {
    Store(base, pages + 8, page);
  }
  return pages;
}

// The common move: transition `running` to page `next`, none queued.
void Go(uint8_t* base, uint32_t unless, uint32_t running, uint32_t queued, uint32_t next) {
  const uint32_t pages = Keep(base, unless);
  Store(base, pages + 12, running);
  Store(base, pages + 16, queued);
  Store(base, pages + 4, next);
  Store(base, pages + 0, kNone);
}

bool Moving(uint8_t* base, uint32_t list) {
  return Load(base, list + 24) != 0 || int32_t(Load(base, list + 4) - Load(base, list + 8)) != 0;
}

uint64_t Sound(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t id) {
  return calls.Call(kMenuSoundPlay, frame, lr, id);
}

}  // namespace

uint64_t Page0(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82180F90 CarDealer::Page0");
  const uint32_t frame = Frame(base, sp, 96);
  calls.Call(kListUpdate, frame, 0x82180FAC, Load(base, kListMenu));
  const uint32_t list = Load(base, kListMenu);
  if (Load(base, list + 24) != 0) {
    Go(base, 0, 2, kNone, 0);
    return Sound(calls, frame, 0x821810CC, 4);
  }
  if (int32_t(uint32_t(calls.Call(kListAccepted, frame, 0x82180FE8, list))) != 0) {
    const uint32_t selection =
        uint32_t(calls.Call(kListSelection, frame, 0x82180FF8, Load(base, kListMenu)));
    if (selection < 2) {
      const uint32_t data = Load(base, kData);
      Store(base, data + 1136, data + (selection == 0 ? 104 : 620));
    }
    Store(base, Load(base, kData) + 44, selection);
    const uint32_t shown = Load(base, Load(base, kData) + 1136);
    if (Load(base, shown + 512) == 0) {
      return Sound(calls, frame, 0x821810CC, 3);
    }
    Go(base, 1, 4, 6, 1);
    return Sound(calls, frame, 0x821810CC, 0);
  }
  const uint64_t cancelled = calls.Call(kListCancelled, frame, 0x82181088, Load(base, kListMenu));
  if (int32_t(uint32_t(cancelled)) == 0) {
    return cancelled;
  }
  Go(base, 4, 3, kNone, 4);
  return Sound(calls, frame, 0x821810CC, 2);
}

uint64_t Page1(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82183428 CarDealer::Page1");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kListUpdate, frame, 0x8218344C, Load(base, kListMenu));
  const uint32_t list = Load(base, kListMenu);
  if (Moving(base, list)) {
    const uint64_t r3 = Sound(calls, frame, 0x821835B0, 4);
    const uint32_t pages = Keep(base, 1);
    Store(base, pages + 4, 1);
    Store(base, pages + 12, 7);
    Store(base, pages + 0, kNone);
    Store(base, pages + 16, kNone);
    return r3;
  }
  if (int32_t(uint32_t(calls.Call(kListAccepted, frame, 0x82183470, list))) != 0) {
    const uint32_t selection =
        uint32_t(calls.Call(kListSelection, frame, 0x82183480, Load(base, kListMenu)));
    Store(base, Load(base, kData) + 48, selection);
    Store(base, Load(base, kData) + 52, Load(base, Load(base, kListMenu) + 4));
    Go(base, 2, 9, 11, 2);
    return Sound(calls, frame, 0x821834DC, 0);
  }
  const uint64_t cancelled = calls.Call(kListCancelled, frame, 0x821834E4, Load(base, kListMenu));
  if (int32_t(uint32_t(cancelled)) == 0) {
    return cancelled;
  }
  uint64_t r3 = Sound(calls, frame, 0x821834F4, 2);
  if (Load(base, Load(base, kData) + 4) != 0) {
    r3 = calls.Call(kShowFirstCarDialog, frame, 0x8218350C);
    Store(base, Load(base, kData) + 36, 0);
    Post(base, mode, 34);
    const uint32_t pages = Keep(base, kNone);
    Store(base, pages + 12, kNone);
    Store(base, pages + 0, kNone);
    Store(base, pages + 16, kNone);
    return r3;
  }
  const uint32_t pages = Keep(base, 0);
  Store(base, pages + 4, 0);
  Store(base, pages + 12, 8);
  Store(base, pages + 0, kNone);
  Store(base, pages + 16, kNone);
  return r3;
}

uint64_t Page2(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82183600 CarDealer::Page2");
  const uint32_t frame = Frame(base, sp, 144);
  calls.Call(kListUpdate, frame, 0x8218361C, Load(base, kListMenu));
  calls.Call(kListUpdate, frame, 0x82183628, Load(base, kColorMenu));
  const uint32_t list = Load(base, kListMenu);
  if (Moving(base, list) || Moving(base, Load(base, kColorMenu))) {
    const uint32_t running = Moving(base, list) ? 12 : 13;
    const uint32_t pages = Keep(base, 2);
    Store(base, pages + 0, kNone);
    Store(base, pages + 4, 2);
    Store(base, pages + 16, kNone);
    Store(base, pages + 12, running);
    return Sound(calls, frame, 0x82183AC0, 4);
  }
  if (int32_t(uint32_t(calls.Call(kListAccepted, frame, 0x8218366C, list))) != 0) {
    const uint32_t price = Load(base, SelectedEntry(base) + 744);
    const uint32_t movie = FindMovie(base, 19, frame + 80);
    Sound(calls, frame, 0x821836C0, 0);
    const uint64_t selection =
        calls.Call(kListSelection, frame, 0x821836C8, Load(base, kListMenu));
    const uint32_t picked = uint32_t(selection);
    if (picked > 1) {
      return selection;
    }
    // A screen of another's (the dialogs, the spec sheet) with no
    // transition here: message `id`, the page object to none.
    auto other = [&](uint32_t id) {
      Post(base, mode, id);
      const uint32_t pages = Keep(base, kNone);
      Store(base, pages + 0, kNone);
      Store(base, pages + 12, kNone);
      Store(base, pages + 16, kNone);
    };
    if (picked == 1) {
      calls.Call(kSetUpSpecSheet, frame, 0x821836D8);
      other(24);
      const uint32_t prompts[6] = {0, 1, kNone, kNone, kNone, kNone};
      return SetPrompts(base, prompts, frame);
    }
    const uint32_t data = Load(base, kData);
    uint64_t text = 0;
    if (int32_t(Load(base, data + 100)) <= int32_t(Load(base, data + 96))) {
      text = 22;  // the garage full
    } else if (int32_t(price) > int32_t(Load(base, data + 84))) {
      text = 23;  // the money short
    }
    if (text != 0) {
      other(37);
      const uint32_t prompts[6] = {0, 0, kNone, kNone, kNone, kNone};
      SetPrompts(base, prompts, frame);
      const uint64_t args[8] = {movie, 0xFFFFFFFFFFFFFFFFull, 1, text, 0, 0, 0, 0};
      return calls.CallArgs(kSetText, frame, 0x821838C8, args);
    }
    if (Load(base, data + 44) == 0) {
      Go(base, 3, 15, 16, 3);
      return selection;
    }
    CheckLimits(base);
    Post(base, mode, 28);
    const uint32_t pages = Keep(base, kNone);
    Store(base, pages + 16, kNone);
    Store(base, pages + 12, 21);
    Store(base, pages + 4, kNone);
    Store(base, pages + 0, kNone);
    return selection;
  }
  const uint64_t cancelled = calls.Call(kListCancelled, frame, 0x821839A0, Load(base, kListMenu));
  if (int32_t(uint32_t(cancelled)) != 0) {
    const uint32_t pages = Keep(base, 1);
    Store(base, pages + 16, kNone);
    Store(base, pages + 0, kNone);
    Store(base, pages + 12, 14);
    Store(base, pages + 4, 1);
    Store(base, Load(base, kData) + 28, 0);
    return Sound(calls, frame, 0x82183AC0, 2);
  }
  const uint64_t pressed = calls.Call(kListPressed166, frame, 0x821839FC, Load(base, kListMenu));
  if (int32_t(uint32_t(pressed)) == 0) {
    return pressed;
  }
  // The two versions of one car (ids 43 and 2) swapped.
  const uint32_t entry = SelectedEntry(base);
  const int32_t id = int32_t(Load(base, entry + 8432));
  if (id != 43 && id != 2) {
    return pressed;
  }
  Store(base, entry + 8432, id == 43 ? 2 : 43);
  ReloadStandModel(base, frame, calls);
  return HelpLineSet(base, id == 43 ? 1864 : 1905, frame, calls);
}

uint64_t Page3(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82181A10 CarDealer::Page3");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kListUpdate, frame, 0x82181A34, Load(base, kListMenu));
  const uint32_t list = Load(base, kListMenu);
  auto go = [&](uint32_t unless, uint32_t running, uint32_t next, uint32_t sound) {
    const uint32_t pages = Keep(base, unless);
    Store(base, pages + 0, kNone);
    Store(base, pages + 4, next);
    Store(base, pages + 16, kNone);
    Store(base, pages + 12, running);
    return Sound(calls, frame, 0x82181B80, sound);
  };
  if (Moving(base, list)) {
    return go(3, 17, 3, 4);
  }
  if (int32_t(uint32_t(calls.Call(kListAccepted, frame, 0x82181A58, list))) != 0) {
    CheckLimits(base);
    const uint32_t selection =
        uint32_t(calls.Call(kListSelection, frame, 0x82181A6C, Load(base, kListMenu)));
    Store(base, Load(base, kData) + 56, selection);
    Store(base, Load(base, kData) + 60, Load(base, Load(base, kListMenu) + 4));
    const uint32_t pages = Keep(base, kNone);
    Store(base, pages + 16, kNone);
    Store(base, pages + 4, kNone);
    Store(base, pages + 0, kNone);
    Store(base, pages + 12, 19);
    Post(base, mode, 28);
    return Sound(calls, frame, 0x82181B80, 0);
  }
  const uint64_t cancelled = calls.Call(kListCancelled, frame, 0x82181B14, Load(base, kListMenu));
  if (int32_t(uint32_t(cancelled)) == 0) {
    return cancelled;
  }
  return go(2, 18, 2, 2);
}

}  // namespace car_dealer
