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
Pages Keep(uint8_t* base, uint32_t unless) {
  const Pages pages = G::pages(base);
  const uint32_t page = pages.page();
  if (page != unless) {
    pages.set_previous_page(page);
  }
  return pages;
}

// The common move: transition `running` to page `next`, none queued.
void Go(uint8_t* base, uint32_t unless, uint32_t running, uint32_t queued, uint32_t next) {
  const Pages pages = Keep(base, unless);
  pages.set_step(running);
  pages.set_queued_step(queued);
  pages.set_next_page(next);
  pages.set_page(kNone);
}

bool Moving(uint8_t* base, uint32_t list) {
  const MenuList menu(base, list);
  return menu.move() != 0 || int32_t(menu.scroll() - menu.scroll_before()) != 0;
}

uint64_t Sound(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t id) {
  return calls.Call(kMenuSoundPlay, frame, lr, id);
}

}  // namespace

uint64_t Page0(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82180F90 CarDealer::Page0");
  const uint32_t frame = Frame(base, sp, 96);
  calls.Call(kListUpdate, frame, 0x82180FAC, G::list_menu(base).address);
  const uint32_t list = G::list_menu(base).address;
  if (MenuList(base, list).move() != 0) {
    Go(base, 0, 2, kNone, 0);
    return Sound(calls, frame, 0x821810CC, 4);
  }
  if (int32_t(uint32_t(calls.Call(kListAccepted, frame, 0x82180FE8, list))) != 0) {
    const uint32_t selection =
        uint32_t(calls.Call(kListSelection, frame, 0x82180FF8, G::list_menu(base).address));
    if (selection < 2) {
      const Data data = G::data(base);
      data.set_shown_list(selection == 0 ? data.first_list() : data.second_list());
    }
    G::data(base).set_list_shown(selection);
    const List shown = G::data(base).shown_list();
    if (shown.count() == 0) {
      return Sound(calls, frame, 0x821810CC, 3);
    }
    Go(base, 1, 4, 6, 1);
    return Sound(calls, frame, 0x821810CC, 0);
  }
  const uint64_t cancelled = calls.Call(kListCancelled, frame, 0x82181088, G::list_menu(base).address);
  if (int32_t(uint32_t(cancelled)) == 0) {
    return cancelled;
  }
  Go(base, 4, 3, kNone, 4);
  return Sound(calls, frame, 0x821810CC, 2);
}

uint64_t Page1(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82183428 CarDealer::Page1");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kListUpdate, frame, 0x8218344C, G::list_menu(base).address);
  const uint32_t list = G::list_menu(base).address;
  if (Moving(base, list)) {
    const uint64_t r3 = Sound(calls, frame, 0x821835B0, 4);
    const Pages pages = Keep(base, 1);
    pages.set_next_page(1);
    pages.set_step(7);
    pages.set_page(kNone);
    pages.set_queued_step(kNone);
    return r3;
  }
  if (int32_t(uint32_t(calls.Call(kListAccepted, frame, 0x82183470, list))) != 0) {
    const uint32_t selection =
        uint32_t(calls.Call(kListSelection, frame, 0x82183480, G::list_menu(base).address));
    G::data(base).set_car_selection(selection);
    G::data(base).set_car_scroll(G::list_menu(base).scroll());
    Go(base, 2, 9, 11, 2);
    return Sound(calls, frame, 0x821834DC, 0);
  }
  const uint64_t cancelled = calls.Call(kListCancelled, frame, 0x821834E4, G::list_menu(base).address);
  if (int32_t(uint32_t(cancelled)) == 0) {
    return cancelled;
  }
  uint64_t r3 = Sound(calls, frame, 0x821834F4, 2);
  if (G::data(base).first_car() != 0) {
    r3 = calls.Call(kShowFirstCarDialog, frame, 0x8218350C);
    G::data(base).set_buy_step(0);
    Post(base, mode, 34);
    const Pages pages = Keep(base, kNone);
    pages.set_step(kNone);
    pages.set_page(kNone);
    pages.set_queued_step(kNone);
    return r3;
  }
  const Pages pages = Keep(base, 0);
  pages.set_next_page(0);
  pages.set_step(8);
  pages.set_page(kNone);
  pages.set_queued_step(kNone);
  return r3;
}

uint64_t Page2(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82183600 CarDealer::Page2");
  const uint32_t frame = Frame(base, sp, 144);
  calls.Call(kListUpdate, frame, 0x8218361C, G::list_menu(base).address);
  calls.Call(kListUpdate, frame, 0x82183628, G::color_menu(base).address);
  const uint32_t list = G::list_menu(base).address;
  if (Moving(base, list) || Moving(base, G::color_menu(base).address)) {
    const uint32_t running = Moving(base, list) ? 12 : 13;
    const Pages pages = Keep(base, 2);
    pages.set_page(kNone);
    pages.set_next_page(2);
    pages.set_queued_step(kNone);
    pages.set_step(running);
    return Sound(calls, frame, 0x82183AC0, 4);
  }
  if (int32_t(uint32_t(calls.Call(kListAccepted, frame, 0x8218366C, list))) != 0) {
    const uint32_t price = SelectedEntry(base).price();
    const uint32_t movie = FindMovie(base, 19, frame + 80);
    Sound(calls, frame, 0x821836C0, 0);
    const uint64_t selection =
        calls.Call(kListSelection, frame, 0x821836C8, G::list_menu(base).address);
    const uint32_t picked = uint32_t(selection);
    if (picked > 1) {
      return selection;
    }
    // A screen of another's (the dialogs, the spec sheet) with no
    // transition here: message `id`, the page object to none.
    auto other = [&](uint32_t id) {
      Post(base, mode, id);
      const Pages pages = Keep(base, kNone);
      pages.set_page(kNone);
      pages.set_step(kNone);
      pages.set_queued_step(kNone);
    };
    if (picked == 1) {
      calls.Call(kSetUpSpecSheet, frame, 0x821836D8);
      other(24);
      const uint32_t prompts[6] = {0, 1, kNone, kNone, kNone, kNone};
      return SetPrompts(base, prompts, frame);
    }
    const Data data = G::data(base);
    uint64_t text = 0;
    if (int32_t(data.garage_size()) <= int32_t(data.garage_count())) {
      text = 22;  // the garage full
    } else if (int32_t(price) > int32_t(data.money())) {
      text = 23;  // the money short
    }
    if (text != 0) {
      other(37);
      const uint32_t prompts[6] = {0, 0, kNone, kNone, kNone, kNone};
      SetPrompts(base, prompts, frame);
      const uint64_t args[8] = {movie, 0xFFFFFFFFFFFFFFFFull, 1, text, 0, 0, 0, 0};
      return calls.CallArgs(kSetText, frame, 0x821838C8, args);
    }
    if (data.list_shown() == 0) {
      Go(base, 3, 15, 16, 3);
      return selection;
    }
    CheckLimits(base);
    Post(base, mode, 28);
    const Pages pages = Keep(base, kNone);
    pages.set_queued_step(kNone);
    pages.set_step(21);
    pages.set_next_page(kNone);
    pages.set_page(kNone);
    return selection;
  }
  const uint64_t cancelled = calls.Call(kListCancelled, frame, 0x821839A0, G::list_menu(base).address);
  if (int32_t(uint32_t(cancelled)) != 0) {
    const Pages pages = Keep(base, 1);
    pages.set_queued_step(kNone);
    pages.set_page(kNone);
    pages.set_step(14);
    pages.set_next_page(1);
    G::data(base).set_car_shown(0);
    return Sound(calls, frame, 0x82183AC0, 2);
  }
  const uint64_t pressed = calls.Call(kListPressed166, frame, 0x821839FC, G::list_menu(base).address);
  if (int32_t(uint32_t(pressed)) == 0) {
    return pressed;
  }
  // The two versions of one car (ids 43 and 2) swapped.
  const Entry entry = SelectedEntry(base);
  const int32_t id = int32_t(entry.car_id());
  if (id != 43 && id != 2) {
    return pressed;
  }
  entry.set_car_id(id == 43 ? 2 : 43);
  ReloadStandModel(base, frame, calls);
  return HelpLineSet(base, id == 43 ? 1864 : 1905, frame, calls);
}

uint64_t Page3(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82181A10 CarDealer::Page3");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kListUpdate, frame, 0x82181A34, G::list_menu(base).address);
  const uint32_t list = G::list_menu(base).address;
  auto go = [&](uint32_t unless, uint32_t running, uint32_t next, uint32_t sound) {
    const Pages pages = Keep(base, unless);
    pages.set_page(kNone);
    pages.set_next_page(next);
    pages.set_queued_step(kNone);
    pages.set_step(running);
    return Sound(calls, frame, 0x82181B80, sound);
  };
  if (Moving(base, list)) {
    return go(3, 17, 3, 4);
  }
  if (int32_t(uint32_t(calls.Call(kListAccepted, frame, 0x82181A58, list))) != 0) {
    CheckLimits(base);
    const uint32_t selection =
        uint32_t(calls.Call(kListSelection, frame, 0x82181A6C, G::list_menu(base).address));
    G::data(base).set_color_selection(selection);
    G::data(base).set_color_scroll(G::list_menu(base).scroll());
    const Pages pages = Keep(base, kNone);
    pages.set_queued_step(kNone);
    pages.set_next_page(kNone);
    pages.set_page(kNone);
    pages.set_step(19);
    Post(base, mode, 28);
    return Sound(calls, frame, 0x82181B80, 0);
  }
  const uint64_t cancelled = calls.Call(kListCancelled, frame, 0x82181B14, G::list_menu(base).address);
  if (int32_t(uint32_t(cancelled)) == 0) {
    return cancelled;
  }
  return go(2, 18, 2, 2);
}

}  // namespace car_dealer
