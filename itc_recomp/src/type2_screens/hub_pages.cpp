// The hub's pages and dialog replies (docs/functions/type2-screens.md): see
// type2_screens.h.

#include "type2_screens/type2_screens.h"

#include "base/log.h"
#include "type2_screens/internal.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace type2_screens {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint64_t kAll = 0xFFFFFFFFFFFFFFFFull;  // li rN,-1
constexpr uint32_t kRatioScale = 0x82002A0C;
constexpr uint32_t kZero = 0x82000A34;
// The word the quit question's dialog reads (0x826364EC): cleared before
// the hub's page 3.
constexpr uint32_t kQuitWord = 0x826364EC;
// Movie 16's word at +40960 (the question's kind).
constexpr uint32_t kQuestionKind = 40960;

uint32_t Data(uint8_t* base) { return Load(base, kHubData); }

uint64_t Sound(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t id) {
  return calls.Call(kMenuSoundPlay, frame, lr, id);
}
uint64_t Pressed(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t lr,
                 uint32_t button) {
  return calls.Call(kMenuPressed, frame, lr, Load(base, kHubMenu), button);
}

// The quit questions on movie 16: its texts (-1, 0) `title`, (1, 3) 2 and
// (1, 4) 3, its +40960 = `kind`; the page object to none; message 34.
// SetText's r3.
uint64_t Question(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t mode,
                  uint32_t title, uint32_t kind, const uint32_t lr[3]) {
  const uint32_t movie = FindMovie(base, 16, frame + 80);
  calls.Call(kSetText, frame, lr[0], movie, kAll, 0, title);
  calls.Call(kSetText, frame, lr[1], movie, 1, 3, 2);
  const uint64_t r3 = calls.Call(kSetText, frame, lr[2], movie, 1, 4, 3);
  Store(base, movie + kQuestionKind, kind);
  const uint32_t pages = Load(base, kHubPages);
  const uint32_t before = Load(base, pages + 0);
  if (before != kNone) {
    Store(base, pages + 8, before);
  }
  Store(base, pages + 0, kNone);
  Store(base, pages + 12, kNone);
  Store(base, pages + 16, kNone);
  Post(base, mode, 34);
  return r3;
}

// Page 0 with no transition, then SetPrompts(0, 0, 6, 2, 7, 3).
uint64_t ToMenu(uint8_t* base, uint32_t frame) {
  const uint32_t pages = Load(base, kHubPages);
  const uint32_t before = Load(base, pages + 0);
  if (before != 0) {
    Store(base, pages + 8, before);
  }
  Store(base, pages + 0, 0);
  Store(base, pages + 12, kNone);
  Store(base, pages + 16, kNone);
  static constexpr uint32_t kPrompts[6] = {0, 0, 6, 2, 7, 3};
  return HubSetPrompts(base, kPrompts, frame);
}

}  // namespace

void HubFillPlayerRecord(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218DA48 Type2Hub::FillPlayerRecord");
  const uint32_t frame = Frame(base, sp, 6176);
  const uint32_t record = Load(base, kRecordScreen) + 8;
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t stats = save + 48;
  calls.Call(kMemcpy, frame, 0x8218DA84, frame + 96, stats, 6044);
  const auto half = [&](uint32_t at) { return int32_t(int16_t(guest::LoadU16(base, frame + at))); };
  const int32_t a = half(108), b = half(110), c = half(112), d = half(118);
  Store(base, record + 0, uint32_t(a));
  Store(base, record + 4, uint32_t(b));
  Store(base, record + 8, uint32_t(c));
  float ratio;
  if (d != 0) {
    // The original's stack words for fcfid (+88 a, +80 d, as doublewords).
    Store(base, frame + 88, a < 0 ? kNone : 0);
    Store(base, frame + 92, uint32_t(a));
    Store(base, frame + 80, d < 0 ? kNone : 0);
    Store(base, frame + 84, uint32_t(d));
    const float q = float(double(float(double(a))) / double(float(double(d))));
    ratio = float(double(q) * double(guest::LoadFloat(base, kRatioScale)));
  } else {
    ratio = guest::LoadFloat(base, kZero);
  }
  const int32_t e = half(6064);
  const uint32_t r9 = Load(base, frame + 6120);
  const float f12 = guest::LoadFloat(base, frame + 6088);
  guest::StoreFloat(base, record + 12, ratio);
  guest::StoreFloat(base, record + 20, guest::LoadFloat(base, frame + 6072));
  Store(base, record + 16, uint32_t(e));
  guest::StoreFloat(base, record + 24, guest::LoadFloat(base, frame + 6000));
  Store(base, record + 32, r9);
  Store(base, record + 28, uint32_t(TruncateWord(double(f12))));
  const uint32_t time = uint32_t(calls.Call(kPlayTimeAdd, frame, 0x8218DB2C, frame + 80, stats));
  Store(base, record + 36, Load(base, time + 0));
  Store(base, record + 40, Load(base, time + 4));
  Store(base, record + 44, uint32_t(int32_t(int16_t(guest::LoadU16(base, save + 6328)))));
  Store(base, record + 48, Load(base, save + 0x1991C));
}

uint64_t HubNoticePage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218E8F8 Type2Hub::NoticePage");
  const uint32_t frame = Frame(base, sp, 112);
  uint64_t r3 = Pressed(base, calls, frame, 0x8218E91C, 159);
  if (uint32_t(r3) == 0) {
    return r3;
  }
  Sound(calls, frame, 0x8218E92C, 0);
  const uint32_t movie = FindMovie(base, kHubMovie, frame + 80);
  r3 = calls.Call(kRevealText, frame, 0x8218E958, movie, 0, 2);
  if (uint32_t(r3) != 0) {
    const uint32_t more = uint32_t(calls.Call(kHasMoreText, frame, 0x8218EA48, movie, 0, 2));
    const uint32_t node = ui_movies::FindNode(base, movie, 0, 0);
    Store(base, node + 80, more != 0 ? 1 : 0);
    return node;
  }
  const uint32_t next = HubNextNotice(base, frame);
  if (next == 0) {
    PagesGo(base, Load(base, kHubPages), 4, 11, 10);
  } else if (next == 1) {
    PagesGo(base, Load(base, kHubPages), 4, 9, 8);
  } else if (next == 2) {
    const uint32_t found = HubNextNotice2(base, frame);
    PagesGo(base, Load(base, kHubPages), found != 0 ? 5 : 0, 6, 7);
    return found;
  }
  return next;
}

uint64_t HubOnDialogDone(uint8_t* base, uint64_t r3, uint32_t mode, uint32_t sp,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218F800 Type2Hub::OnDialogDone");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t data = Data(base);
  const uint32_t step = Load(base, data + 24);
  if (step == 0) {
    const uint32_t answer = Load(base, data + 28);
    if (answer == 0) {
      static constexpr uint32_t kLr[3] = {0x8218F9E0, 0x8218F9F4, 0x8218FA08};
      r3 = Question(base, calls, frame, mode, 29, 0, kLr);
      Store(base, Data(base) + 24, 1);
      return r3;
    }
    return answer < 3 ? ToMenu(base, frame) : r3;
  }
  if (step == 1) {
    const uint32_t answer = Load(base, data + 28);
    if (answer == 0) {
      const uint32_t pages = Load(base, kHubPages);
      const uint32_t before = Load(base, pages + 0);
      if (before != 6) {
        Store(base, pages + 8, before);
      }
      Store(base, pages + 12, kNone);
      Store(base, pages + 16, kNone);
      Store(base, pages + 0, 6);
      Post(base, mode, 43);
      return r3;
    }
    if (answer == 1) {
      const uint32_t pages = Load(base, kHubPages);
      const uint32_t before = Load(base, pages + 0);
      if (before != 3) {
        Store(base, pages + 8, before);
      }
      Store(base, pages + 12, 3);
      Store(base, pages + 16, kNone);
      Store(base, pages + 0, kNone);
      Store(base, pages + 4, 3);
      Store(base, kQuitWord, 0);
      return calls.Call(kPlayTimeUpdate, frame, 0x8218F8D0, vehicle_accessors::SaveDataOf(base, 0));
    }
    return answer < 3 ? ToMenu(base, frame) : r3;
  }
  if (step == 2) {
    if (Load(base, data + 20) == 1) {
      return ToMenu(base, frame);
    }
    const uint32_t found = HubNextNotice2(base, frame);
    if (found == 0) {
      return ToMenu(base, frame);
    }
    const uint32_t pages = Load(base, kHubPages);
    const uint32_t before = Load(base, pages + 0);
    if (before != 5) {
      Store(base, pages + 8, before);
    }
    Store(base, pages + 12, kNone);
    Store(base, pages + 0, 5);
    Store(base, pages + 16, kNone);
    return found;
  }
  return r3;
}

uint64_t HubMenuPage(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218FCC8 Type2Hub::MenuPage");
  const uint32_t frame = Frame(base, sp, 2112);
  calls.Call(kMenuStep, frame, 0x8218FCE4, Load(base, kHubMenu));
  // The page object to `page` after `transition` (none queued), +8 the page
  // before unless it was `page`.
  const auto go = [&](uint32_t page, uint32_t transition) {
    const uint32_t pages = Load(base, kHubPages);
    const uint32_t before = Load(base, pages + 0);
    if (before != page) {
      Store(base, pages + 8, before);
    }
    Store(base, pages + 12, transition);
    Store(base, pages + 16, kNone);
    Store(base, pages + 4, page);
    Store(base, pages + 0, kNone);
  };
  // Pages 2's moves (the spec sheet, the record): +8 the page before unless
  // 2; then SetPrompts with the pair given.
  const auto other = [&](uint32_t a, uint32_t b) {
    const uint32_t pages = Load(base, kHubPages);
    const uint32_t before = Load(base, pages + 0);
    if (before != 2) {
      Store(base, pages + 8, before);
    }
    Store(base, pages + 16, kNone);
    Store(base, pages + 12, kNone);
    Store(base, pages + 0, 2);
    const uint32_t prompts[6] = {a, b, kNone, kNone, kNone, kNone};
    return HubSetPrompts(base, prompts, frame);
  };
  if (Load(base, Load(base, kHubMenu) + 8) != 0) {
    const uint64_t r3 = Sound(calls, frame, 0x8218FCFC, 4);
    go(0, 2);
    return r3;
  }
  uint64_t r3 = Pressed(base, calls, frame, 0x8218FD24, 159);
  if (uint32_t(r3) != 0) {
    const int32_t item = int32_t(Load(base, Load(base, kHubMenu) + 0) + 1);
    if (item <= 0 || item > 8) {
      return r3;
    }
    if (item == 8) {
      Sound(calls, frame, 0x8218FD64, 0);
      static constexpr uint32_t kLr[3] = {0x8218FD94, 0x8218FDA8, 0x8218FDBC};
      Question(base, calls, frame, mode, 27, 1, kLr);
      Store(base, Data(base) + 24, 0);
      static constexpr uint32_t kPrompts[6] = {0, 0, 1, 1, kNone, kNone};
      return HubSetPrompts(base, kPrompts, frame);
    }
    if ((item == 2 || item == 3) && Load(base, Data(base) + 12) != 0) {
      return Sound(calls, frame, 0x8218FE80, 3);
    }
    r3 = Sound(calls, frame, 0x8218FE8C, 0);
    go(3, 3);
    return r3;
  }
  r3 = Pressed(base, calls, frame, 0x8218FEC8, 158);
  if (uint32_t(r3) != 0) {
    r3 = Sound(calls, frame, 0x8218FED8, 2);
    go(0, 12);
    return r3;
  }
  r3 = Pressed(base, calls, frame, 0x8218FF1C, 163);
  if (uint32_t(r3) != 0) {
    Sound(calls, frame, 0x8218FF2C, 0);
    const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
    calls.Call(kMemcpy, frame, 0x8218FF80, frame + 96, SelectedGarageCar(base, save) + 212,
               1968);
    calls.Call(kSheetCarData, frame, 0x8218FF88, frame + 96);
    Post(base, mode, 24);
    return other(6, 2);
  }
  r3 = Pressed(base, calls, frame, 0x82190004, 164);
  if (uint32_t(r3) != 0) {
    Sound(calls, frame, 0x82190014, 0);
    HubFillPlayerRecord(base, frame, calls);
    Post(base, mode, 27);
    return other(7, 3);
  }
  return r3;
}

}  // namespace type2_screens
