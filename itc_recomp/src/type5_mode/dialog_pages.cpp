// The type 5 dialog's pages (docs/functions/type5-mode.md): the three Draw
// runs from its table while a dialog shows (the confirm page for dialogs 0
// and 2), r3 the dispatcher. Menu::Step, Menu::Pressed and MenuSound::Play
// go through their overrides; the replies are posted straight into the
// dispatcher's ring as the originals do.

#include "type5_mode/type5_mode.h"

#include "base/log.h"
#include "guest.h"
#include "schema/boot_mode.h"
#include "schema/game_modes.h"
#include "schema/menu_helpers.h"
#include "schema/type5_mode.h"

namespace type5_mode {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using G = schema::Type5ModeGlobals;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(base, frame, sp);
  return frame;
}

// Posts the message (id, 0, 0) on the dispatcher's ring (boot-mode.toml's
// ModeDispatcher: 50 slots of 12 bytes), the id stored first or last, as
// the originals do inline.
void Post(uint8_t* base, uint32_t dispatcher, uint32_t id, bool id_last = false) {
  const schema::ModeDispatcher d(base, dispatcher);
  const uint32_t count = d.ring_count() + 1;
  const uint32_t end = d.ring_start() + count - 1;
  d.set_ring_count(count);
  const schema::ModeMessage slot(base, d.ring_address() + (end - (end / 50) * 50) * 12);
  if (!id_last) {
    slot.set_id(id);
  }
  slot.set_arg0(0);
  slot.set_arg1(0);
  if (id_last) {
    slot.set_id(id);
  }
}

// The page object's page before = its page unless that is `unless`;
// returns it.
schema::PageObject KeepPage(uint8_t* base, uint32_t unless) {
  const auto pages = G::pages(base);
  const uint32_t before = pages.page();
  if (before != unless) {
    pages.set_previous_page(before);
  }
  return pages;
}

// The confirm page's close: the close step 1, none queued, the next page 0,
// the page -1 (the step running).
void CloseConfirm(uint8_t* base) {
  const auto pages = KeepPage(base, 0);
  pages.set_step(1);
  pages.set_queued_step(0xFFFFFFFFu);
  pages.set_next_page(0);
  pages.set_page(0xFFFFFFFFu);
}

}  // namespace

uint64_t ConfirmPage(uint8_t* base, uint32_t dispatcher, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82225D10 Type5Dialog::ConfirmPage");
  const uint32_t frame = Frame(base, sp, 144);
  calls.Call(kMenuStep, frame, 0x82225D2C, G::menu(base).address);
  uint64_t answer = calls.Call(kMenuPressed, frame, 0x82225D38, G::menu(base).address, 159);
  if (int32_t(answer) != 0) {
    answer = calls.Call(kMenuSoundPlay, frame, 0x82225D5C, 0);
    CloseConfirm(base);
    const bool yes = G::menu(base).cursor() == 0;
    Post(base, dispatcher, yes ? 14 : 15, true);
  }
  answer = calls.Call(kMenuPressed, frame, 0x82225DE8, G::menu(base).address, 158);
  if (int32_t(answer) != 0) {
    answer = calls.Call(kMenuSoundPlay, frame, 0x82225DF8, 2);
    CloseConfirm(base);
    Post(base, dispatcher, 17);
  }
  if (G::menu(base).moved() != 0) {
    answer = calls.Call(kMenuSoundPlay, frame, 0x82225E7C, 4);
    const auto pages = KeepPage(base, 0);
    pages.set_queued_step(0xFFFFFFFFu);
    pages.set_next_page(0);
    pages.set_page(0xFFFFFFFFu);
    pages.set_step(8);
  }
  if (G::data(base).interrupted() != 0) {
    CloseConfirm(base);
    Post(base, dispatcher, 20);
  }
  return answer;
}

uint64_t NoticePage(uint8_t* base, uint32_t dispatcher, bool second, uint32_t sp,
                    const Calls& calls) {
  if (second) {
    ITC_NATIVE_RUNNING("sub_82226088 Type5Dialog::Notice2Page");
  } else {
    ITC_NATIVE_RUNNING("sub_82225F30 Type5Dialog::NoticePage");
  }
  // Step, Pressed(159), Pressed(158), Play(0).
  static constexpr uint32_t kLr[2][4] = {{0x82225F4C, 0x82225F58, 0x82225F80, 0x82225F90},
                                         {0x822260A4, 0x822260B0, 0x822260D8, 0x822260E8}};
  const uint32_t* lr = kLr[second ? 1 : 0];
  const uint32_t page = second ? 3 : 1;
  const uint32_t step = second ? 7 : 3;
  const uint32_t frame = Frame(base, sp, 144);
  // The close: the close step, none queued, the next page, the page -1.
  auto close = [&] {
    const auto pages = KeepPage(base, page);
    pages.set_step(step);
    pages.set_queued_step(0xFFFFFFFFu);
    pages.set_next_page(page);
    pages.set_page(0xFFFFFFFFu);
  };
  calls.Call(kMenuStep, frame, lr[0], G::menu(base).address);
  uint64_t answer = calls.Call(kMenuPressed, frame, lr[1], G::menu(base).address, 159);
  if (int32_t(answer) == 0) {
    answer = calls.Call(kMenuPressed, frame, lr[2], G::menu(base).address, 158);
  }
  if (int32_t(answer) != 0) {
    answer = calls.Call(kMenuSoundPlay, frame, lr[3], 0);
    close();
    Post(base, dispatcher, 16);
  }
  if (G::data(base).interrupted() != 0) {
    close();
    Post(base, dispatcher, 20);
  }
  return answer;
}

}  // namespace type5_mode
