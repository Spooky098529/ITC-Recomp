// The license plate editor's pages and update
// (docs/functions/type2-screens.md): see type2_screens.h.

#include "type2_screens/type2_screens.h"

#include "base/log.h"
#include "race_screens/race_screens.h"
#include "type2_screens/internal.h"
#include "ui_movies/game.h"

namespace type2_screens {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;

// The pages' handlers, in UpdatePage's table (its frame's +80 on).
constexpr uint32_t kPages[5] = {0x82199ED0, 0x8219A138, 0x82198540, 0x82198830, 0x82198988};
// The transitions' handlers, in Update's table (its frame's +96 on).
constexpr uint32_t kTransitions[20] = {
    0x8219A3A8, 0x82198A40, 0x82198B58, 0x8219A450, 0x82198C20, 0x8219A5E8, 0x82198D00,
    0x8219A6B8, 0x8219A820, 0x8219A8E0, 0x8219AA00, 0x82198DC8, 0x8219AAB0, 0x8219AC88,
    0x8219AD68, 0x8219AE90, 0x82198E70, 0x82198F08, 0x8219AF50, 0x82198FB8};

uint32_t List(uint8_t* base) { return Load(base, kPlateList); }
uint32_t Data(uint8_t* base) { return Load(base, kPlateData); }
uint32_t Pages(uint8_t* base) { return Load(base, kPlatePages); }

// MenuList's calls on the plate editor's list and MenuSound::Play, from the
// page's frame.
uint64_t ListCall(uint8_t* base, const Calls& calls, uint32_t function, uint32_t frame,
                  uint32_t lr) {
  return calls.Call(function, frame, lr, List(base));
}
uint64_t Sound(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t sound) {
  return calls.Call(kMenuSoundPlay, frame, lr, sound);
}
// The list's selection (MenuList::Selection, a leaf).
uint32_t Selection(uint8_t* base) { return race_screens::ListSelection(base, List(base)); }
// The list moving: +24 or +28 set, or (`scroll` too) +4 not +8.
bool Moving(uint8_t* base, bool scroll) {
  const uint32_t list = List(base);
  if (Load(base, list + 24) != 0 || Load(base, list + 28) != 0) {
    return true;
  }
  return scroll && Load(base, list + 4) != Load(base, list + 8);
}

}  // namespace

uint64_t PlateRegionPage(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82199ED0 PlateEditor::RegionPage");
  const uint32_t frame = Frame(base, sp, 112);
  ListCall(base, calls, kListUpdate, frame, 0x82199EF4);
  uint64_t r3 = ListCall(base, calls, kListAccepted, frame, 0x82199EFC);
  if (uint32_t(r3) != 0) {
    Sound(calls, frame, 0x82199F0C, 0);
    const uint32_t selection = Selection(base);
    r3 = selection;
    const uint32_t row = Load(base, List(base) + 4) + selection;
    guest::StoreU8(base, Data(base) + 5, uint8_t(row * 3 + Load(base, Data(base) + 52)));
    PagesGo(base, Pages(base), 1, 5, 6);
    return r3;
  }
  r3 = ListCall(base, calls, kListCancelled, frame, 0x82199F78);
  if (uint32_t(r3) != 0) {
    r3 = Sound(calls, frame, 0x82199F88, 2);
    PagesGo(base, Pages(base), 4, 4, kNone);
    Post(base, mode, 30);
    return r3;
  }
  r3 = ListCall(base, calls, kListPressed152, frame, 0x82199FFC);
  if (uint32_t(r3) != 0) {
    Sound(calls, frame, 0x8219A00C, 0);
    const uint32_t selection = Selection(base);
    const uint32_t row = Load(base, List(base) + 4) + selection;
    guest::StoreU8(base, Data(base) + 5, uint8_t(row * 3 + Load(base, Data(base) + 52)));
    r3 = PlateFinishRegionAnim(base, frame, calls);
    PagesGo(base, Pages(base), kNone, 19, kNone);
    Post(base, mode, 29);
    return r3;
  }
  if (Moving(base, true)) {
    PagesGo(base, Pages(base), 0, 3, kNone);
  }
  return r3;
}

uint64_t PlateNumberPage(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219A138 PlateEditor::NumberPage");
  const uint32_t frame = Frame(base, sp, 112);
  ListCall(base, calls, kListUpdate, frame, 0x8219A15C);
  uint64_t r3 = ListCall(base, calls, kListAccepted, frame, 0x8219A164);
  // The digit at the data's +56 (byte +6 on) from the selection and the
  // column.
  auto set_digit = [&](uint32_t selection) {
    const uint32_t data = Data(base);
    guest::StoreU8(base, Load(base, data + 56) + data + 6,
                   uint8_t((selection << 2) + Load(base, data + 52)));
  };
  // The next or previous digit: +56 moved by `by`, transitions 8 then 6.
  auto step_digit = [&](int32_t by) {
    const uint32_t data = Data(base);
    Store(base, data + 56, Load(base, data + 56) + uint32_t(by));
    PagesGo(base, Pages(base), 1, 8, 6);
  };
  if (uint32_t(r3) != 0) {
    Sound(calls, frame, 0x8219A174, 0);
    const uint32_t selection = Selection(base);
    r3 = selection;
    set_digit(selection);
    if (Load(base, Data(base) + 56) == 2) {
      PagesGo(base, Pages(base), 2, 10, 11);
    } else {
      step_digit(1);
    }
    return r3;
  }
  r3 = ListCall(base, calls, kListCancelled, frame, 0x8219A21C);
  if (uint32_t(r3) != 0) {
    r3 = Sound(calls, frame, 0x8219A22C, 2);
    if (Load(base, Data(base) + 56) == 0) {
      PagesGo(base, Pages(base), 0, 9, kNone);
    } else {
      step_digit(-1);
    }
    return r3;
  }
  r3 = ListCall(base, calls, kListPressed152, frame, 0x8219A280);
  if (uint32_t(r3) != 0) {
    Sound(calls, frame, 0x8219A290, 0);
    set_digit(Selection(base));
    r3 = PlateFinishDigitAnim(base, frame, calls);
    PagesGo(base, Pages(base), kNone, 19, kNone);
    Post(base, mode, 29);
    return r3;
  }
  if (Moving(base, false)) {
    PagesGo(base, Pages(base), 1, 7, kNone);
  }
  return r3;
}

uint64_t PlateCharsPage(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198540 PlateEditor::CharsPage");
  const uint32_t frame = Frame(base, sp, 112);
  ListCall(base, calls, kListUpdate, frame, 0x82198564);
  uint64_t r3 = ListCall(base, calls, kListAccepted, frame, 0x8219856C);
  // The character at the data's +56 (byte +9 on) from the list's scroll,
  // the selection and the column.
  auto set_char = [&](uint32_t selection) {
    const uint32_t scroll = Load(base, List(base) + 4);
    const uint32_t data = Data(base);
    guest::StoreU8(base, Load(base, data + 56) + data + 9,
                   uint8_t((scroll + selection) * 10 + Load(base, data + 52)));
  };
  // The next or previous character: +56 moved by `by`, transitions 13 then
  // 11.
  auto step_char = [&](int32_t by) {
    const uint32_t data = Data(base);
    Store(base, data + 56, Load(base, data + 56) + uint32_t(by));
    PagesGo(base, Pages(base), 2, 13, 11);
  };
  // The plate done: transition 19 (no page after it), message 29.
  auto done = [&]() {
    PagesGo(base, Pages(base), kNone, 19, kNone);
    Post(base, mode, 29);
  };
  if (uint32_t(r3) != 0) {
    Sound(calls, frame, 0x8219857C, 0);
    const uint32_t selection = Selection(base);
    r3 = selection;
    set_char(selection);
    if (Load(base, Data(base) + 56) == 6) {
      done();
    } else {
      step_char(1);
    }
    return r3;
  }
  r3 = ListCall(base, calls, kListCancelled, frame, 0x8219863C);
  if (uint32_t(r3) != 0) {
    r3 = Sound(calls, frame, 0x8219864C, 2);
    if (Load(base, Data(base) + 56) == 0) {
      PagesGo(base, Pages(base), 1, 14, kNone);
    } else {
      step_char(-1);
    }
    return r3;
  }
  r3 = ListCall(base, calls, kListPressed152, frame, 0x821986E8);
  if (uint32_t(r3) != 0) {
    Sound(calls, frame, 0x821986F8, 0);
    const uint32_t selection = Selection(base);
    r3 = selection;
    set_char(selection);
    done();
    return r3;
  }
  if (Moving(base, true)) {
    PagesGo(base, Pages(base), 2, 12, kNone);
    r3 = Sound(calls, frame, 0x82198814, 4);
  }
  return r3;
}

uint64_t PlateUpdatePage(uint8_t* base, uint64_t r3, uint32_t mode, uint32_t sp,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219B050 PlateEditor::UpdatePage");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t machine = uint32_t(r3);
  for (uint32_t i = 0; i < 5; i++) {
    Store(base, frame + 80 + 4 * i, kPages[i]);
  }
  const uint32_t page = Load(base, Pages(base) + 0);
  if (page != kNone) {
    r3 = calls.Call(Load(base, frame + 80 + (page << 2)), frame, 0x8219B0CC, mode);
  }
  const uint32_t next = Load(base, Data(base) + 72);
  if (next != 21) {
    Store(base, machine + kMachineState, next);
    Store(base, Data(base) + 72, 21);
  }
  return r3;
}

uint64_t PlateUpdate(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219B108 PlateEditor::Update");
  const uint32_t frame = Frame(base, sp, 304);
  calls.Call(kEmpty, frame, 0x8219B12C, 100, 30, 0, 0x8200A408);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  const uint32_t pages = Pages(base);
  Store(base, frame + 172, kTransitions[19]);
  for (uint32_t i = 0; i < 19; i++) {
    Store(base, frame + 96 + 4 * i, kTransitions[i]);
  }
  const int32_t running = int32_t(Load(base, pages + 12));
  if (running == -1) {
    return calls.Call(kRun, frame, 0x8219B2DC, movie, 0);
  }
  uint64_t r3 = movie;
  if (running >= 0) {
    r3 = calls.Call(Load(base, frame + 96 + (uint32_t(running) << 2)), frame, 0x8219B26C,
                    pages + 24);
    if (uint32_t(r3) == 0) {
      return r3;
    }
  }
  const uint32_t now = Pages(base);
  const uint32_t queued = Load(base, now + 16);
  if (queued == kNone) {
    Store(base, now + 0, Load(base, now + 4));
    Store(base, now + 12, queued);
  } else {
    Store(base, now + 16, kNone);
    Store(base, now + 20, 0);
    Store(base, now + 12, queued);
  }
  Store(base, now + 24, 0);
  return r3;
}

}  // namespace type2_screens
