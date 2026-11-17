// The type 2 dialog's pages (docs/functions/type2-dialog.md): the four Draw
// runs from its table while a dialog shows, r3 the mode. Menu::Step,
// Menu::Pressed and MenuSound::Play go through their overrides; the replies
// are posted straight into the mode's ring as the originals do.

#include "type2_dialog/type2_dialog.h"

#include "base/log.h"
#include "guest.h"
#include "schema/game_modes.h"
#include "schema/menu_helpers.h"
#include "schema/type2_dialog.h"

namespace type2_dialog {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using G = schema::Type2DialogGlobals;
using Machine = schema::StateMachine;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(base, frame, sp);
  return frame;
}

// The page object's move to `step` for `page`: +8 the page before unless it
// was `page`, then +12 the step, +16 none, +4 the page after, +0 -1.
void ToStep(uint8_t* base, uint32_t page, uint32_t step) {
  const auto pages = G::pages(base);
  const uint32_t before = pages.page();
  if (before != page) {
    pages.set_previous_page(before);
  }
  pages.set_step(step);
  pages.set_queued_step(0xFFFFFFFFu);
  pages.set_next_page(page);
  pages.set_page(0xFFFFFFFFu);
}

// The page's first lines: the dialog showing, and the status: 4 (the frame
// TakesInput asked for) skips the input. True when the page goes on.
bool TakeInput(uint8_t* base, uint32_t showing) {
  G::set_showing(base, showing);
  const uint32_t status = G::status(base);
  G::set_status(base, 1);
  return status != 4;
}

}  // namespace

void Post(uint8_t* base, uint32_t mode, uint32_t id, bool id_last) {
  const uint32_t ring = mode + 204;
  const uint32_t count = LoadU32(base, ring + 600) + 1;
  const uint32_t end = LoadU32(base, ring + 604) + count - 1;
  StoreU32(base, ring + 600, count);
  const uint32_t slot = ring + (end - (end / 50) * 50) * 12;
  if (!id_last) {
    StoreU32(base, slot + 0, id);
  }
  StoreU32(base, slot + 4, 0);
  StoreU32(base, slot + 8, 0);
  if (id_last) {
    StoreU32(base, slot + 0, id);
  }
}

uint64_t ConfirmPage(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82187160 Type2Dialog::ConfirmPage");
  const uint32_t mode = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 144);
  if (!TakeInput(base, 0)) {
    return r3;
  }
  calls.Call(kMenuStep, frame, 0x821871B4, G::menu(base).address);
  uint64_t answer = calls.Call(kMenuPressed, frame, 0x821871C0, G::menu(base).address, 159);
  if (int32_t(answer) != 0) {
    answer = calls.Call(kMenuSoundPlay, frame, 0x821871DC, 0);
    // The page before: +8 when the page isn't 0 (its own).
    const auto pages = G::pages(base);
    const uint32_t before = pages.page();
    if (before != 0) {
      pages.set_previous_page(before);
    }
    pages.set_step(1);
    pages.set_queued_step(0xFFFFFFFFu);
    pages.set_next_page(0);
    pages.set_page(0xFFFFFFFFu);
    const bool yes = G::menu(base).cursor() == 0;
    Post(base, mode, yes ? 38 : 39, true);
  }
  answer = calls.Call(kMenuPressed, frame, 0x82187268, G::menu(base).address, 158);
  if (int32_t(answer) != 0) {
    answer = calls.Call(kMenuSoundPlay, frame, 0x82187278, 2);
    ToStep(base, 0, 1);
    Post(base, mode, 41);
  }
  if (G::menu(base).moved() == 0) {
    return answer;
  }
  answer = calls.Call(kMenuSoundPlay, frame, 0x821872FC, 4);
  const auto pages = G::pages(base);
  const uint32_t before = pages.page();
  if (before != 0) {
    pages.set_previous_page(before);
  }
  pages.set_queued_step(0xFFFFFFFFu);
  pages.set_next_page(0);
  pages.set_page(0xFFFFFFFFu);
  pages.set_step(8);
  return answer;
}

uint64_t Confirm2Page(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82187330 Type2Dialog::Confirm2Page");
  const uint32_t mode = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 112);
  if (!TakeInput(base, 2)) {
    return r3;
  }
  calls.Call(kMenuStep, frame, 0x82187374, G::menu(base).address);
  const auto menu = G::menu(base);
  if (menu.moved() != 0) {
    const uint64_t answer = calls.Call(kMenuSoundPlay, frame, 0x8218738C, 4);
    const auto pages = G::pages(base);
    const uint32_t before = pages.page();
    if (before != 2) {
      pages.set_previous_page(before);
    }
    pages.set_next_page(2);
    pages.set_step(9);
    pages.set_queued_step(0xFFFFFFFFu);
    pages.set_page(0xFFFFFFFFu);
    return answer;
  }
  uint64_t answer = calls.Call(kMenuPressed, frame, 0x821873C4, menu.address, 159);
  if (int32_t(answer) != 0) {
    answer = calls.Call(kMenuSoundPlay, frame, 0x821873D4, 0);
    const auto pages = G::pages(base);
    const uint32_t before = pages.page();
    if (before != 2) {
      pages.set_previous_page(before);
    }
    pages.set_next_page(2);
    pages.set_queued_step(0xFFFFFFFFu);
    pages.set_step(5);
    pages.set_page(0xFFFFFFFFu);
    const bool yes = G::menu(base).cursor() == 0;
    Post(base, mode, yes ? 38 : 39, true);
    return answer;
  }
  answer = calls.Call(kMenuPressed, frame, 0x82187474, G::menu(base).address, 158);
  if (int32_t(answer) == 0) {
    return answer;
  }
  answer = calls.Call(kMenuSoundPlay, frame, 0x82187484, 2);
  const auto pages = G::pages(base);
  const uint32_t before = pages.page();
  if (before != 2) {
    pages.set_previous_page(before);
  }
  pages.set_next_page(2);
  pages.set_queued_step(0xFFFFFFFFu);
  pages.set_page(0xFFFFFFFFu);
  pages.set_step(5);
  Post(base, mode, 41);
  return answer;
}

uint64_t NoticePage(uint8_t* base, uint64_t r3, bool second, uint32_t sp, const Calls& calls) {
  if (second) {
    ITC_NATIVE_RUNNING("sub_82187618 Type2Dialog::Notice2Page");
  } else {
    ITC_NATIVE_RUNNING("sub_82187508 Type2Dialog::NoticePage");
  }
  // Step, Pressed(159), Pressed(158), Play(0).
  static constexpr uint32_t kLr[2][4] = {{0x82187550, 0x8218755C, 0x82187570, 0x82187580},
                                         {0x82187660, 0x8218766C, 0x82187680, 0x82187690}};
  const uint32_t* lr = kLr[second ? 1 : 0];
  const uint32_t mode = uint32_t(r3);
  const uint32_t page = second ? 3 : 1;
  const uint32_t frame = Frame(base, sp, 112);
  if (!TakeInput(base, page)) {
    return r3;
  }
  calls.Call(kMenuStep, frame, lr[0], G::menu(base).address);
  uint64_t answer = calls.Call(kMenuPressed, frame, lr[1], G::menu(base).address, 159);
  if (int32_t(answer) == 0) {
    answer = calls.Call(kMenuPressed, frame, lr[2], G::menu(base).address, 158);
    if (int32_t(answer) == 0) {
      return answer;
    }
  }
  answer = calls.Call(kMenuSoundPlay, frame, lr[3], 0);
  const auto pages = G::pages(base);
  const uint32_t before = pages.page();
  if (before != page) {
    pages.set_previous_page(before);
  }
  pages.set_queued_step(0xFFFFFFFFu);
  pages.set_page(0xFFFFFFFFu);
  pages.set_step(second ? 7 : 3);
  pages.set_next_page(page);
  Post(base, mode, 40);
  return answer;
}

}  // namespace type2_dialog
