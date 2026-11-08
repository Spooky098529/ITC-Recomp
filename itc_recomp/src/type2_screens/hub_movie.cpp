// The hub's page dispatch, movie helpers and transitions
// (docs/functions/type2-screens.md): see type2_screens.h.

#include "type2_screens/type2_screens.h"

#include "base/log.h"
#include "data_tables/data_tables.h"
#include "type2_screens/internal.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace type2_screens {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint64_t kAll = 0xFFFFFFFFFFFFFFFFull;  // li rN,-1
// The hub's page handlers, in UpdatePage's table (its frame's +80 on):
// MenuPage, the empty sub_82106540 twice, Choose, NoticePage, RecordPage,
// SavePromptPage.
constexpr uint32_t kHubPagesTable[7] = {0x8218FCC8, kEmpty,     kEmpty,    0x8218E768,
                                        0x8218E8F8, 0x821900B8, 0x82190168};
// The screen fades' counts, and the fades' layer words.
constexpr uint32_t kFadeOutCount = 0x826A50E4;
constexpr uint32_t kFadeInCount = 0x826A50DC;
constexpr uint32_t kFadeInLayer = 0x82001BEC;
constexpr uint32_t kFadeOutLayer = 0x82001BF0;
// The floats Transition2 stores into node (11, 0)'s +16.
constexpr uint32_t kWrapLast = 0x82008914;
constexpr uint32_t kZero = 0x82000A34;

uint32_t Data(uint8_t* base) { return Load(base, kHubData); }

uint64_t Play(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t sub,
              uint64_t child, uint64_t r6 = 0) {
  return calls.Call(kPlay, frame, lr, movie, sub, child, r6);
}
uint64_t PlayNamed(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t id,
                   uint64_t r5 = 0, uint64_t r6 = 0) {
  return calls.Call(kPlayNamed, frame, lr, movie, id, r5, r6);
}
uint64_t SetText(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t sub,
                 uint64_t child, uint64_t text) {
  return calls.Call(kSetText, frame, lr, movie, sub, child, text);
}
uint64_t Run(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie) {
  return calls.Call(kRun, frame, lr, movie, 0);
}
uint32_t FadesDone(uint8_t* base) {
  return (Load(base, kFadeOutCount) | Load(base, kFadeInCount)) == 0 ? 1 : 0;
}
// The description's string for the menu's item (`item`, the cursor + 1),
// from `first` on: 587 and 588 for items 2 and 3 with a second-list car.
uint32_t Description(uint8_t* base, uint32_t item, uint32_t first) {
  if (item == 2 && Load(base, Data(base) + 12) != 0) {
    return 587;
  }
  if (item == 3 && Load(base, Data(base) + 12) != 0) {
    return 588;
  }
  return first + item;
}

}  // namespace

uint64_t HubUpdatePage(uint8_t* base, uint64_t r3, uint32_t mode, uint32_t sp,
                       const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82190498 Type2Hub::UpdatePage");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t machine = uint32_t(r3);
  for (uint32_t i = 0; i < 7; i++) {
    Store(base, frame + 80 + 4 * i, kHubPagesTable[i]);
  }
  const uint32_t page = Load(base, Load(base, kHubPages) + 0);
  if (page != kNone) {
    r3 = calls.Call(Load(base, frame + 80 + (page << 2)), frame, 0x82190530, mode, machine);
  }
  return r3;
}

uint64_t HubSetRecordText(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218F4D8 Type2Hub::SetRecordText");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t movie = FindMovie(base, 17, frame + 80);
  return SetText(calls, frame, 0x8218F51C, movie, kAll, 1, Load(base, Data(base) + 424) + 7527);
}

uint64_t HubRecordPage(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821900B8 Type2Hub::RecordPage");
  const uint32_t frame = Frame(base, sp, 96);
  const uint64_t r3 = HubSetRecordText(base, frame, calls);
  const uint32_t pages = Load(base, kHubPages);
  const uint32_t before = Load(base, pages + 0);
  if (before != kNone) {
    Store(base, pages + 8, before);
  }
  Store(base, pages + 0, kNone);
  Store(base, pages + 12, kNone);
  Store(base, pages + 16, kNone);
  Post(base, mode, 35);
  Store(base, Data(base) + 24, 2);
  return r3;
}

uint64_t HubShowBusy(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218EA90 Type2Hub::ShowBusy");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kHubMovie, frame + 80);
  PlayNamed(calls, frame, 0x8218EAD8, movie, 56);
  PlayNamed(calls, frame, 0x8218EAF4, movie, 54);
  return calls.Call(kLoopLast, frame, 0x8218EAFC, movie);
}

uint64_t HubHideBusy(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218EB10 Type2Hub::HideBusy");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kHubMovie, frame + 80);
  PlayNamed(calls, frame, 0x8218EB58, movie, 57);
  return calls.Call(kFinishNamed, frame, 0x8218EB64, movie, 54);
}

uint32_t HubShowNoticeImage(uint8_t* base, uint32_t image, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8218F3F8 Type2Hub::ShowNoticeImage");
  const uint32_t frame = Frame(base, sp, 112);
  Store(base, frame + 80, kHubMovie);
  const uint32_t pair = ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80);
  const uint32_t movie = Load(base, pair);
  static constexpr struct {
    uint32_t list, field;
  } kLists[2] = {{32, 36}, {220, 32}};
  for (const auto& list : kLists) {
    const uint32_t at = Data(base) + list.list;
    const uint32_t count = Load(base, at + 184);
    uint32_t texture = 0;
    for (uint32_t i = 0; i < count; i++) {
      const uint32_t record = Load(base, at + 4 * i);
      if (Load(base, record + 4) == image) {
        texture = Load(base, record + 8);
        break;
      }
    }
    Store(base, Load(base, movie + 28) + list.field, texture);
  }
  return pair;
}

uint32_t HubShowNotice(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821903D0 Type2Hub::ShowNotice");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kHubMovie, frame + 80);
  const uint32_t entry =
      data_tables::Entry56(base, Load(base, Data(base) + 432), Load(base, Data(base) + 436));
  SetText(calls, frame, 0x8219042C, movie, 0, 1, Load(base, entry + 12) + 4650);
  SetText(calls, frame, 0x82190448, movie, 0, 2, Load(base, Data(base) + 436) + 6683);
  HubShowNoticeImage(base, Load(base, entry + 16), frame);
  const uint32_t more = uint32_t(calls.Call(kHasMoreText, frame, 0x82190460, movie, 0, 2));
  const uint32_t node = ui_movies::FindNode(base, movie, 0, 0);
  Store(base, node + 80, more != 0 ? 1 : 0);
  return node;
}

uint64_t HubFillMenu(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218FB20 Type2Hub::FillMenu");
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t movie = FindMovie(base, kHubMovie, frame + 80);
  static constexpr uint32_t kItems[8] = {109, 116, 111, 117, 118, 119, 113, 114};
  for (uint32_t i = 0; i < 8; i++) {
    Store(base, frame + 96 + 4 * i, kItems[i]);
  }
  for (uint32_t i = 0; i < 8; i++) {
    SetText(calls, frame, 0x8218FBA4, movie, 11, 5 + i, Load(base, frame + 96 + 4 * i));
  }
  const uint32_t cursor = Load(base, Load(base, kHubMenu) + 0);
  calls.Call(kScrollText, frame, 0x8218FBD4, movie, 9, 0, 1);
  calls.Call(kScrollReset, frame, 0x8218FBE4, movie, 9, 0);
  uint32_t text = 536 + cursor;
  if (cursor == 2 && Load(base, Data(base) + 12) != 0) {
    text = 587;
  } else if (cursor == 3 && Load(base, Data(base) + 12) != 0) {
    text = 588;
  }
  SetText(calls, frame, 0x8218FC38, movie, 9, 0, text);
  static constexpr uint32_t kPrompts[6] = {0, 0, 6, 2, 7, 3};
  HubSetPrompts(base, kPrompts, frame);
  const uint32_t car = SelectedGarageCar(base, vehicle_accessors::SaveDataOf(base, 0));
  const uint64_t r3 = calls.Call(kGetCarSpec, frame, 0x8218FCA0, Load(base, car + 224));
  const bool second = guest::LoadU8(base, uint32_t(r3) + 771) == 1;
  Store(base, Data(base) + 12, second ? 1 : 0);
  return r3;
}

uint64_t HubTransition0(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821901C0 Type2Hub::Transition0");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kHubMovie, frame + 80);
  if (Load(base, step) == 0) {
    Play(calls, frame, 0x82190220, movie, kAll, 0);
    Play(calls, frame, 0x82190240, movie, 8, 0);
    calls.Call(kLoopLast, frame, 0x82190248, movie);
    Play(calls, frame, 0x82190268, movie, 15, 0);
    calls.Call(kLoopLast, frame, 0x82190270, movie);
    calls.Call(kFadeIn, frame, 0x8219028C, Load(base, kFadeInLayer), 0, 0, 0);
    HubFillMenu(base, frame, calls);
    Store(base, step, 1);
  }
  return uint32_t(Run(calls, frame, 0x821902A4, movie)) == 0 ? 1 : 0;
}

uint64_t HubTransition1(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218EC78 Type2Hub::Transition1");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kHubMovie, frame + 80);
  if (Load(base, step) == 0) {
    const uint32_t item = Load(base, Load(base, kHubMenu) + 0) + 1;
    const uint32_t second = Load(base, Data(base) + 12);
    if (second == 0 || (item != 2 && item != 3)) {
      Play(calls, frame, 0x8218ED04, movie, 11, item + 1);
      calls.Call(kLoopLast, frame, 0x8218ED0C, movie);
    }
    Play(calls, frame, 0x8218ED2C, movie, 12, 1);
    calls.Call(kLoop, frame, 0x8218ED3C, movie, 12, 1);
    if (Load(base, Data(base) + 12) != 0) {
      calls.Call(kSetNodeColor, frame, 0x8218ED60, movie, 11, 6, 2);
      calls.Call(kSetNodeColor, frame, 0x8218ED74, movie, 11, 7, 2);
    }
    Store(base, step, 1);
  }
  Run(calls, frame, 0x8218ED88, movie);
  return 1;
}

uint64_t HubTransition2(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218EE48 Type2Hub::Transition2");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, kHubMovie, frame + 80);
  if (Load(base, step) == 0) {
    const uint32_t menu = Load(base, kHubMenu);
    const uint32_t before = Load(base, menu + 4);
    const uint32_t item = Load(base, menu + 0) + 1;
    calls.Call(kFinish, frame, 0x8218EEA4, movie, 11, before + 2);
    if (Load(base, Data(base) + 12) != 0 && (item == 2 || item == 3)) {
      PlayNamed(calls, frame, 0x8218EEE4, movie, 39);
      calls.Call(kFinishNamed, frame, 0x8218EEF0, movie, 39);
      PlayNamed(calls, frame, 0x8218EF0C, movie, 40, 1, 1);
      calls.Call(kFinishNamed, frame, 0x8218EF18, movie, 40);
    } else {
      Play(calls, frame, 0x8218EF44, movie, 11, Load(base, Load(base, kHubMenu) + 0) + 2, 1);
      calls.Call(kLoopLast, frame, 0x8218EF4C, movie);
    }
    calls.Call(kFinish, frame, 0x8218EF5C, movie, 11, 13);
    calls.Call(kFinish, frame, 0x8218EF6C, movie, 11, 12);
    const uint32_t cursor = item - 1;
    const uint32_t way = Load(base, Load(base, kHubMenu) + 8);
    if (way == 1) {
      if (cursor == 7) {
        Store(base, ui_movies::FindNode(base, movie, 11, 0) + 16, Load(base, kWrapLast));
      } else {
        Play(calls, frame, 0x8218EFDC, movie, 11, 13);
      }
    } else if (way == kNone) {
      if (cursor == 0) {
        Store(base, ui_movies::FindNode(base, movie, 11, 0) + 16, Load(base, kZero));
      } else {
        Play(calls, frame, 0x8218EFDC, movie, 11, 12);
      }
    }
    const uint32_t now = Load(base, Load(base, kHubMenu) + 0) + 1;
    calls.Call(kScrollText, frame, 0x8218F014, movie, 9, 0, 1);
    SetText(calls, frame, 0x8218F064, movie, 9, 0, Description(base, now, 535));
    calls.Call(kScrollReset, frame, 0x8218F074, movie, 9, 0);
    Store(base, step, 1);
  }
  Run(calls, frame, 0x8218F088, movie);
  return 1;
}

uint64_t HubTransition3(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218ED98 Type2Hub::Transition3");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kHubMovie, frame + 80);
  if (Load(base, step) == 0) {
    calls.Call(kFadeOut, frame, 0x8218EDF4, Load(base, kFadeOutLayer), 0, 0, 0);
    Store(base, step, 1);
  }
  Run(calls, frame, 0x8218EE08, movie);
  return FadesDone(base);
}

uint64_t HubTransition5(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82190548 Type2Hub::Transition5");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kHubMovie, frame + 80);
  if (Load(base, step) == 0) {
    Play(calls, frame, 0x821905A8, movie, kAll, 2);
    Play(calls, frame, 0x821905C8, movie, 8, 0);
    calls.Call(kLoopLast, frame, 0x821905D0, movie);
    Play(calls, frame, 0x821905F0, movie, 15, 0);
    calls.Call(kLoopLast, frame, 0x821905F8, movie);
    PlayNamed(calls, frame, 0x82190614, movie, 2);
    calls.Call(kLoopNamed, frame, 0x82190620, movie, 2);
    calls.Call(kFadeIn, frame, 0x8219063C, Load(base, kFadeInLayer), 0, 0, 0);
    static constexpr uint32_t kPrompts[6] = {0, 0, kNone, kNone, kNone, kNone};
    HubSetPrompts(base, kPrompts, frame);
    HubShowNotice(base, frame, calls);
    Store(base, step, 1);
  }
  return uint32_t(Run(calls, frame, 0x82190670, movie)) == 0 ? 1 : 0;
}

uint64_t HubTransition6(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218F098 Type2Hub::Transition6");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kHubMovie, frame + 80);
  if (Load(base, step) == 0) {
    calls.Call(kFinishNamed, frame, 0x8218F0E4, movie, 2);
    calls.Call(kFadeOut, frame, 0x8218F0FC, 60, 0, 0, 0);
    Store(base, step, 1);
  }
  Run(calls, frame, 0x8218F110, movie);
  return FadesDone(base);
}

uint64_t HubTransition7(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821902C8 Type2Hub::Transition7");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kHubMovie, frame + 80);
  if (Load(base, step) == 0) {
    Play(calls, frame, 0x82190328, movie, kAll, 3);
    calls.Call(kFadeIn, frame, 0x82190340, 60, 0, 0, 0);
    HubFillMenu(base, frame, calls);
    Store(base, step, 1);
  }
  Run(calls, frame, 0x82190358, movie);
  const uint32_t done = FadesDone(base);
  if (done != 0 && Load(base, Data(base) + 12) != 0) {
    calls.Call(kSetNodeColor, frame, 0x821903A0, movie, 11, 6, 2);
    calls.Call(kSetNodeColor, frame, 0x821903B4, movie, 11, 7, 2);
  }
  return done;
}

}  // namespace type2_screens
