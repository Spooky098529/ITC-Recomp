// The type 2 dialog's pages (docs/functions/type2-dialog.md): the four Draw
// runs from its table while a dialog shows, r3 the mode. Menu::Step,
// Menu::Pressed and MenuSound::Play go through their overrides; the replies
// are posted straight into the mode's ring as the originals do.

#include "type2_dialog/type2_dialog.h"

#include "base/log.h"
#include "guest.h"

namespace type2_dialog {

namespace {

using guest::LoadU32;
using guest::StoreU32;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(base, frame, sp);
  return frame;
}

// The page object's move to `step` for `page`: +8 the page before unless it
// was `page`, then +12 the step, +16 none, +4 the page after, +0 -1.
void ToStep(uint8_t* base, uint32_t page, uint32_t step) {
  const uint32_t pages = LoadU32(base, kPages);
  const uint32_t before = LoadU32(base, pages + 0);
  if (before != page) {
    StoreU32(base, pages + 8, before);
  }
  StoreU32(base, pages + 12, step);
  StoreU32(base, pages + 16, 0xFFFFFFFFu);
  StoreU32(base, pages + 4, page);
  StoreU32(base, pages + 0, 0xFFFFFFFFu);
}

// The page's first lines: the dialog showing, and the status: 4 (the frame
// TakesInput asked for) skips the input. True when the page goes on.
bool TakeInput(uint8_t* base, uint32_t showing) {
  StoreU32(base, kShowing, showing);
  const uint32_t status = LoadU32(base, kStatus);
  StoreU32(base, kStatus, 1);
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
  calls.Call(kMenuStep, frame, 0x821871B4, LoadU32(base, kMenu));
  uint64_t answer = calls.Call(kMenuPressed, frame, 0x821871C0, LoadU32(base, kMenu), 159);
  if (int32_t(answer) != 0) {
    answer = calls.Call(kMenuSoundPlay, frame, 0x821871DC, 0);
    // The page before: +8 when the page isn't 0 (its own).
    const uint32_t pages = LoadU32(base, kPages);
    const uint32_t before = LoadU32(base, pages + 0);
    if (before != 0) {
      StoreU32(base, pages + 8, before);
    }
    StoreU32(base, pages + 12, 1);
    StoreU32(base, pages + 16, 0xFFFFFFFFu);
    StoreU32(base, pages + 4, 0);
    StoreU32(base, pages + 0, 0xFFFFFFFFu);
    const bool yes = LoadU32(base, LoadU32(base, kMenu) + 0) == 0;
    Post(base, mode, yes ? 38 : 39, true);
  }
  answer = calls.Call(kMenuPressed, frame, 0x82187268, LoadU32(base, kMenu), 158);
  if (int32_t(answer) != 0) {
    answer = calls.Call(kMenuSoundPlay, frame, 0x82187278, 2);
    ToStep(base, 0, 1);
    Post(base, mode, 41);
  }
  if (LoadU32(base, LoadU32(base, kMenu) + 8) == 0) {
    return answer;
  }
  answer = calls.Call(kMenuSoundPlay, frame, 0x821872FC, 4);
  const uint32_t pages = LoadU32(base, kPages);
  const uint32_t before = LoadU32(base, pages + 0);
  if (before != 0) {
    StoreU32(base, pages + 8, before);
  }
  StoreU32(base, pages + 16, 0xFFFFFFFFu);
  StoreU32(base, pages + 4, 0);
  StoreU32(base, pages + 0, 0xFFFFFFFFu);
  StoreU32(base, pages + 12, 8);
  return answer;
}

uint64_t Confirm2Page(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82187330 Type2Dialog::Confirm2Page");
  const uint32_t mode = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 112);
  if (!TakeInput(base, 2)) {
    return r3;
  }
  calls.Call(kMenuStep, frame, 0x82187374, LoadU32(base, kMenu));
  const uint32_t menu = LoadU32(base, kMenu);
  if (LoadU32(base, menu + 8) != 0) {
    const uint64_t answer = calls.Call(kMenuSoundPlay, frame, 0x8218738C, 4);
    const uint32_t pages = LoadU32(base, kPages);
    const uint32_t before = LoadU32(base, pages + 0);
    if (before != 2) {
      StoreU32(base, pages + 8, before);
    }
    StoreU32(base, pages + 4, 2);
    StoreU32(base, pages + 12, 9);
    StoreU32(base, pages + 16, 0xFFFFFFFFu);
    StoreU32(base, pages + 0, 0xFFFFFFFFu);
    return answer;
  }
  uint64_t answer = calls.Call(kMenuPressed, frame, 0x821873C4, menu, 159);
  if (int32_t(answer) != 0) {
    answer = calls.Call(kMenuSoundPlay, frame, 0x821873D4, 0);
    const uint32_t pages = LoadU32(base, kPages);
    const uint32_t before = LoadU32(base, pages + 0);
    if (before != 2) {
      StoreU32(base, pages + 8, before);
    }
    StoreU32(base, pages + 4, 2);
    StoreU32(base, pages + 16, 0xFFFFFFFFu);
    StoreU32(base, pages + 12, 5);
    StoreU32(base, pages + 0, 0xFFFFFFFFu);
    const bool yes = LoadU32(base, LoadU32(base, kMenu) + 0) == 0;
    Post(base, mode, yes ? 38 : 39, true);
    return answer;
  }
  answer = calls.Call(kMenuPressed, frame, 0x82187474, LoadU32(base, kMenu), 158);
  if (int32_t(answer) == 0) {
    return answer;
  }
  answer = calls.Call(kMenuSoundPlay, frame, 0x82187484, 2);
  const uint32_t pages = LoadU32(base, kPages);
  const uint32_t before = LoadU32(base, pages + 0);
  if (before != 2) {
    StoreU32(base, pages + 8, before);
  }
  StoreU32(base, pages + 4, 2);
  StoreU32(base, pages + 16, 0xFFFFFFFFu);
  StoreU32(base, pages + 0, 0xFFFFFFFFu);
  StoreU32(base, pages + 12, 5);
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
  calls.Call(kMenuStep, frame, lr[0], LoadU32(base, kMenu));
  uint64_t answer = calls.Call(kMenuPressed, frame, lr[1], LoadU32(base, kMenu), 159);
  if (int32_t(answer) == 0) {
    answer = calls.Call(kMenuPressed, frame, lr[2], LoadU32(base, kMenu), 158);
    if (int32_t(answer) == 0) {
      return answer;
    }
  }
  answer = calls.Call(kMenuSoundPlay, frame, lr[3], 0);
  const uint32_t pages = LoadU32(base, kPages);
  const uint32_t before = LoadU32(base, pages + 0);
  if (before != page) {
    StoreU32(base, pages + 8, before);
  }
  StoreU32(base, pages + 16, 0xFFFFFFFFu);
  StoreU32(base, pages + 0, 0xFFFFFFFFu);
  StoreU32(base, pages + 12, second ? 7 : 3);
  StoreU32(base, pages + 4, page);
  Post(base, mode, 40);
  return answer;
}

}  // namespace type2_dialog
