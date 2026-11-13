// The type 5 dialog's pages (docs/functions/type5-mode.md): the three Draw
// runs from its table while a dialog shows (the confirm page for dialogs 0
// and 2), r3 the dispatcher. Menu::Step, Menu::Pressed and MenuSound::Play
// go through their overrides; the replies are posted straight into the
// dispatcher's ring as the originals do.

#include "type5_mode/type5_mode.h"

#include "base/log.h"
#include "guest.h"

namespace type5_mode {

namespace {

using guest::LoadU32;
using guest::StoreU32;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(base, frame, sp);
  return frame;
}

// Posts the message (id, 0, 0) on the dispatcher's ring (+204: its count
// +600, start +604, 50 slots of 12 bytes), the id stored first or last, as
// the originals do inline.
void Post(uint8_t* base, uint32_t dispatcher, uint32_t id, bool id_last = false) {
  const uint32_t ring = dispatcher + 204;
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

// The page object's +8 = its page unless that is `unless`; returns it.
uint32_t KeepPage(uint8_t* base, uint32_t unless) {
  const uint32_t pages = LoadU32(base, kPages);
  const uint32_t before = LoadU32(base, pages + 0);
  if (before != unless) {
    StoreU32(base, pages + 8, before);
  }
  return pages;
}

// The confirm page's close: +12 the close step 1, +16 none, +4 page 0, +0
// -1 (the step running).
void CloseConfirm(uint8_t* base) {
  const uint32_t pages = KeepPage(base, 0);
  StoreU32(base, pages + 12, 1);
  StoreU32(base, pages + 16, 0xFFFFFFFFu);
  StoreU32(base, pages + 4, 0);
  StoreU32(base, pages + 0, 0xFFFFFFFFu);
}

}  // namespace

uint64_t ConfirmPage(uint8_t* base, uint32_t dispatcher, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82225D10 Type5Dialog::ConfirmPage");
  const uint32_t frame = Frame(base, sp, 144);
  calls.Call(kMenuStep, frame, 0x82225D2C, LoadU32(base, kMenu));
  uint64_t answer = calls.Call(kMenuPressed, frame, 0x82225D38, LoadU32(base, kMenu), 159);
  if (int32_t(answer) != 0) {
    answer = calls.Call(kMenuSoundPlay, frame, 0x82225D5C, 0);
    CloseConfirm(base);
    const bool yes = LoadU32(base, LoadU32(base, kMenu) + 0) == 0;
    Post(base, dispatcher, yes ? 14 : 15, true);
  }
  answer = calls.Call(kMenuPressed, frame, 0x82225DE8, LoadU32(base, kMenu), 158);
  if (int32_t(answer) != 0) {
    answer = calls.Call(kMenuSoundPlay, frame, 0x82225DF8, 2);
    CloseConfirm(base);
    Post(base, dispatcher, 17);
  }
  if (LoadU32(base, LoadU32(base, kMenu) + 8) != 0) {
    answer = calls.Call(kMenuSoundPlay, frame, 0x82225E7C, 4);
    const uint32_t pages = KeepPage(base, 0);
    StoreU32(base, pages + 16, 0xFFFFFFFFu);
    StoreU32(base, pages + 4, 0);
    StoreU32(base, pages + 0, 0xFFFFFFFFu);
    StoreU32(base, pages + 12, 8);
  }
  if (LoadU32(base, LoadU32(base, kData) + 12) != 0) {
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
  // The close: +12 the close step, +16 none, +4 the page, +0 -1.
  auto close = [&] {
    const uint32_t pages = KeepPage(base, page);
    StoreU32(base, pages + 12, step);
    StoreU32(base, pages + 16, 0xFFFFFFFFu);
    StoreU32(base, pages + 4, page);
    StoreU32(base, pages + 0, 0xFFFFFFFFu);
  };
  calls.Call(kMenuStep, frame, lr[0], LoadU32(base, kMenu));
  uint64_t answer = calls.Call(kMenuPressed, frame, lr[1], LoadU32(base, kMenu), 159);
  if (int32_t(answer) == 0) {
    answer = calls.Call(kMenuPressed, frame, lr[2], LoadU32(base, kMenu), 158);
  }
  if (int32_t(answer) != 0) {
    answer = calls.Call(kMenuSoundPlay, frame, lr[3], 0);
    close();
    Post(base, dispatcher, 16);
  }
  if (LoadU32(base, LoadU32(base, kData) + 12) != 0) {
    close();
    Post(base, dispatcher, 20);
  }
  return answer;
}

}  // namespace type5_mode
