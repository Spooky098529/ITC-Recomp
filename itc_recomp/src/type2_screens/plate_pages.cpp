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

uint32_t List() { return Globals::plate_list().address; }
schema::PlateEditorData Data() { return Globals::plate_data(); }
schema::PageObject Pages() { return Globals::plate_pages(); }

// MenuList's calls on the plate editor's list and MenuSound::Play, from the
// page's frame.
uint64_t ListCall(const Calls& calls, uint32_t function, uint32_t frame,
                  uint32_t lr) {
  return calls.Call(function, frame, lr, List());
}
uint64_t Sound(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t sound) {
  return calls.Call(kMenuSoundPlay, frame, lr, sound);
}
// The list's selection (MenuList::Selection, a leaf).
uint32_t Selection() { return race_screens::ListSelection(List()); }
// The list moving: +24 or +28 set, or (`scroll` too) +4 not +8.
bool Moving(bool scroll) {
  const uint32_t list = List();
  if (MenuList(list).move() != 0 || MenuList(list).side() != 0) {
    return true;
  }
  return scroll && MenuList(list).scroll() != MenuList(list).scroll_before();
}

}  // namespace

uint64_t PlateRegionPage(uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82199ED0 PlateEditor::RegionPage");
  const uint32_t frame = Frame(sp, 112);
  ListCall(calls, kListUpdate, frame, 0x82199EF4);
  uint64_t r3 = ListCall(calls, kListAccepted, frame, 0x82199EFC);
  if (uint32_t(r3) != 0) {
    Sound(calls, frame, 0x82199F0C, 0);
    const uint32_t selection = Selection();
    r3 = selection;
    const uint32_t row = MenuList(List()).scroll() + selection;
    Data().set_region(uint8_t(row * 3 + Data().column()));
    PagesGo(Pages().address, 1, 5, 6);
    return r3;
  }
  r3 = ListCall(calls, kListCancelled, frame, 0x82199F78);
  if (uint32_t(r3) != 0) {
    r3 = Sound(calls, frame, 0x82199F88, 2);
    PagesGo(Pages().address, 4, 4, kNone);
    Post(mode, 30);
    return r3;
  }
  r3 = ListCall(calls, kListPressed152, frame, 0x82199FFC);
  if (uint32_t(r3) != 0) {
    Sound(calls, frame, 0x8219A00C, 0);
    const uint32_t selection = Selection();
    const uint32_t row = MenuList(List()).scroll() + selection;
    Data().set_region(uint8_t(row * 3 + Data().column()));
    r3 = PlateFinishRegionAnim(frame, calls);
    PagesGo(Pages().address, kNone, 19, kNone);
    Post(mode, 29);
    return r3;
  }
  if (Moving(true)) {
    PagesGo(Pages().address, 0, 3, kNone);
  }
  return r3;
}

uint64_t PlateNumberPage(uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219A138 PlateEditor::NumberPage");
  const uint32_t frame = Frame(sp, 112);
  ListCall(calls, kListUpdate, frame, 0x8219A15C);
  uint64_t r3 = ListCall(calls, kListAccepted, frame, 0x8219A164);
  // The digit at the data's +56 (byte +6 on) from the selection and the
  // column.
  auto set_digit = [&](uint32_t selection) {
    const schema::PlateEditorData data = Data();
    guest::StoreU8(data.edited() + data.digits_address(),
                   uint8_t((selection << 2) + data.column()));
  };
  // The next or previous digit: +56 moved by `by`, transitions 8 then 6.
  auto step_digit = [&](int32_t by) {
    const schema::PlateEditorData data = Data();
    data.set_edited(data.edited() + uint32_t(by));
    PagesGo(Pages().address, 1, 8, 6);
  };
  if (uint32_t(r3) != 0) {
    Sound(calls, frame, 0x8219A174, 0);
    const uint32_t selection = Selection();
    r3 = selection;
    set_digit(selection);
    if (Data().edited() == 2) {
      PagesGo(Pages().address, 2, 10, 11);
    } else {
      step_digit(1);
    }
    return r3;
  }
  r3 = ListCall(calls, kListCancelled, frame, 0x8219A21C);
  if (uint32_t(r3) != 0) {
    r3 = Sound(calls, frame, 0x8219A22C, 2);
    if (Data().edited() == 0) {
      PagesGo(Pages().address, 0, 9, kNone);
    } else {
      step_digit(-1);
    }
    return r3;
  }
  r3 = ListCall(calls, kListPressed152, frame, 0x8219A280);
  if (uint32_t(r3) != 0) {
    Sound(calls, frame, 0x8219A290, 0);
    set_digit(Selection());
    r3 = PlateFinishDigitAnim(frame, calls);
    PagesGo(Pages().address, kNone, 19, kNone);
    Post(mode, 29);
    return r3;
  }
  if (Moving(false)) {
    PagesGo(Pages().address, 1, 7, kNone);
  }
  return r3;
}

uint64_t PlateCharsPage(uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198540 PlateEditor::CharsPage");
  const uint32_t frame = Frame(sp, 112);
  ListCall(calls, kListUpdate, frame, 0x82198564);
  uint64_t r3 = ListCall(calls, kListAccepted, frame, 0x8219856C);
  // The character at the data's +56 (byte +9 on) from the list's scroll,
  // the selection and the column.
  auto set_char = [&](uint32_t selection) {
    const uint32_t scroll = MenuList(List()).scroll();
    const schema::PlateEditorData data = Data();
    guest::StoreU8(data.edited() + data.chars_address(),
                   uint8_t((scroll + selection) * 10 + data.column()));
  };
  // The next or previous character: +56 moved by `by`, transitions 13 then
  // 11.
  auto step_char = [&](int32_t by) {
    const schema::PlateEditorData data = Data();
    data.set_edited(data.edited() + uint32_t(by));
    PagesGo(Pages().address, 2, 13, 11);
  };
  // The plate done: transition 19 (no page after it), message 29.
  auto done = [&]() {
    PagesGo(Pages().address, kNone, 19, kNone);
    Post(mode, 29);
  };
  if (uint32_t(r3) != 0) {
    Sound(calls, frame, 0x8219857C, 0);
    const uint32_t selection = Selection();
    r3 = selection;
    set_char(selection);
    if (Data().edited() == 6) {
      done();
    } else {
      step_char(1);
    }
    return r3;
  }
  r3 = ListCall(calls, kListCancelled, frame, 0x8219863C);
  if (uint32_t(r3) != 0) {
    r3 = Sound(calls, frame, 0x8219864C, 2);
    if (Data().edited() == 0) {
      PagesGo(Pages().address, 1, 14, kNone);
    } else {
      step_char(-1);
    }
    return r3;
  }
  r3 = ListCall(calls, kListPressed152, frame, 0x821986E8);
  if (uint32_t(r3) != 0) {
    Sound(calls, frame, 0x821986F8, 0);
    const uint32_t selection = Selection();
    r3 = selection;
    set_char(selection);
    done();
    return r3;
  }
  if (Moving(true)) {
    PagesGo(Pages().address, 2, 12, kNone);
    r3 = Sound(calls, frame, 0x82198814, 4);
  }
  return r3;
}

uint64_t PlateUpdatePage(uint64_t r3, uint32_t mode, uint32_t sp,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219B050 PlateEditor::UpdatePage");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t machine = uint32_t(r3);
  for (uint32_t i = 0; i < 5; i++) {
    Store(frame + 80 + 4 * i, kPages[i]);
  }
  const uint32_t page = Pages().page();
  if (page != kNone) {
    r3 = calls.Call(Load(frame + 80 + (page << 2)), frame, 0x8219B0CC, mode);
  }
  const uint32_t next = Data().next_state();
  if (next != 21) {
    schema::PlateEditor(machine).set_state(next);
    Data().set_next_state(21);
  }
  return r3;
}

uint64_t PlateUpdate(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219B108 PlateEditor::Update");
  const uint32_t frame = Frame(sp, 304);
  calls.Call(kEmpty, frame, 0x8219B12C, 100, 30, 0, 0x8200A408);
  const uint32_t movie = FindMovie(kPlateMovie, frame + 80);
  const schema::PageObject pages = Pages();
  Store(frame + 172, kTransitions[19]);
  for (uint32_t i = 0; i < 19; i++) {
    Store(frame + 96 + 4 * i, kTransitions[i]);
  }
  const int32_t running = int32_t(pages.step());
  if (running == -1) {
    return calls.Call(kRun, frame, 0x8219B2DC, movie, 0);
  }
  uint64_t r3 = movie;
  if (running >= 0) {
    r3 = calls.Call(Load(frame + 96 + (uint32_t(running) << 2)), frame, 0x8219B26C,
                    pages.step_started_address());
    if (uint32_t(r3) == 0) {
      return r3;
    }
  }
  const schema::PageObject now = Pages();
  const uint32_t queued = now.queued_step();
  if (queued == kNone) {
    now.set_page(now.next_page());
    now.set_step(queued);
  } else {
    now.set_queued_step(kNone);
    now.set_unknown_20(0);
    now.set_step(queued);
  }
  now.set_step_started(0);
  return r3;
}

}  // namespace type2_screens
