// The options screen's load page (category 5, page 9) and save page
// (category 6, page 10): their dialogs' set-ups, sub-pages and closes
// (docs/functions/options-load-save.md). See options_screen.h.

#include "options_screen/options_screen.h"

#include "base/log.h"
#include "guest.h"
#include "native/buffers.h"
#include "schema/menu_helpers.h"
#include "schema/options_load_save.h"
#include "schema/options_screen.h"
#include "schema/save_interface.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace options_screen {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using G = schema::OptionsLoadSaveGlobals;
using SubPageBlock = schema::OptionsSubPageBlock;
using OptionsGlobals = schema::OptionsScreenGlobals;
using OptionsSettings = schema::OptionsSettings;
using PageObject = schema::PageObject;
using FrontEndMenu = schema::FrontEndMenu;

constexpr uint32_t kMinusOne = 0xFFFFFFFFu;
constexpr uint64_t kMinusOne64 = ~uint64_t(0);

// [0x826D8478], loaded again at every use as the original does.
OptionsSettings Settings(uint8_t* base) { return OptionsGlobals::settings(base); }

// [0x826D8470], the options' page block, loaded again at every use.
PageObject Pages(uint8_t* base) { return OptionsGlobals::pages(base); }

// [0x826D8484], the dialog's menu, loaded again at every use.
FrontEndMenu DialogMenu(uint8_t* base) { return OptionsGlobals::dialog_menu(base); }

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// The two pages' differences: the page, the sub-pages' block, the dialog's
// steps (the change, the menu's cursor move, the close), the settings'
// words (the next state, the kind, the wait counter).
struct Side {
  uint32_t page;
  uint32_t pages_global;
  uint32_t change_step;
  uint32_t show_step;
  uint32_t cursor_step;
  uint32_t leave_step;
  uint32_t (OptionsSettings::*next)() const;
  void (OptionsSettings::*set_next)(uint32_t) const;
  uint32_t (OptionsSettings::*kind)() const;
  void (OptionsSettings::*set_kind)(uint32_t) const;
  uint32_t (OptionsSettings::*counter)() const;
  void (OptionsSettings::*set_counter)(uint32_t) const;
};
constexpr Side kLoad{9, G::load_pages_address(), 44, 43, 45, 41,
                     &OptionsSettings::load_dialog_next, &OptionsSettings::set_load_dialog_next,
                     &OptionsSettings::load_dialog_kind, &OptionsSettings::set_load_dialog_kind,
                     &OptionsSettings::load_dialog_passes, &OptionsSettings::set_load_dialog_passes};
constexpr Side kSave{10, G::save_pages_address(), 47, 46, 48, 42,
                     &OptionsSettings::save_dialog_next, &OptionsSettings::set_save_dialog_next,
                     &OptionsSettings::save_dialog_kind, &OptionsSettings::set_save_dialog_kind,
                     &OptionsSettings::save_dialog_passes, &OptionsSettings::set_save_dialog_passes};

// The side's settings words, through the settings block loaded again.
uint32_t Kind(uint8_t* base, const Side& s) { return (Settings(base).*s.kind)(); }
void SetKind(uint8_t* base, const Side& s, uint32_t v) { (Settings(base).*s.set_kind)(v); }
void SetNext(uint8_t* base, const Side& s, uint32_t v) { (Settings(base).*s.set_next)(v); }

SubPageBlock SubPages(uint8_t* base, const Side& s) {
  return SubPageBlock(base, LoadU32(base, s.pages_global));
}

// A block's page (+0) set to `sub`: +8 the one before unless the same,
// +12 and +16 -1, in the order `order` gives (0: +0, +12, +16; 1: +16, +12,
// +0; 2: +12, +16, +0).
void SetSubPage(const SubPageBlock& block, uint32_t sub, int order) {
  const uint32_t current = block.page();
  if (current != sub) {
    block.set_previous_page(current);
  }
  if (order == 0) {
    block.set_page(sub);
    block.set_step(kMinusOne);
    block.set_queued_step(kMinusOne);
  } else if (order == 1) {
    block.set_queued_step(kMinusOne);
    block.set_step(kMinusOne);
    block.set_page(sub);
  } else {
    block.set_step(kMinusOne);
    block.set_queued_step(kMinusOne);
    block.set_page(sub);
  }
}

// The options' page block p to the dialog's change step, its show queued
// and the page after: +8 the page before unless it's the side's, +12, +16,
// +4, +0 -1.
void QueueDialog(uint8_t* base, const Side& s) {
  const PageObject p = Pages(base);
  const uint32_t current = p.page();
  if (current != s.page) {
    p.set_previous_page(current);
  }
  p.set_step(s.change_step);
  p.set_queued_step(s.show_step);
  p.set_next_page(s.page);
  p.set_page(kMinusOne);
}

// LoadPage::SetDialog and SavePage::SetDialog (their native-running prints
// here, as the pages call this directly).
void SetDialog(uint8_t* base, const Side& s, uint32_t kind, uint32_t next) {
  if (s.page == 9) {
    ITC_NATIVE_RUNNING("sub_821BB858 LoadPage::SetDialog");
  } else {
    ITC_NATIVE_RUNNING("sub_821BB708 SavePage::SetDialog");
  }
  const SubPageBlock pages = SubPages(base, s);
  if (next == 2) {
    QueueDialog(base, s);
    SetSubPage(pages, 3, 2);
    SetNext(base, s, 2);
  } else if (next < 2) {
    SetSubPage(pages, next + 1, 0);
    QueueDialog(base, s);
    SetNext(base, s, next);
  }
  SetKind(base, s, kind);
}

// The sub-page close (4): p +8 the page before unless -1, +16 -1, +12 the
// leave step, +4 and +0 -1; the settings' +712 1.
void ClosePage(uint8_t* base, const Side& s) {
  const PageObject p = Pages(base);
  const uint32_t current = p.page();
  if (current != kMinusOne) {
    p.set_previous_page(current);
  }
  p.set_queued_step(kMinusOne);
  p.set_step(s.leave_step);
  p.set_next_page(kMinusOne);
  p.set_page(kMinusOne);
  Settings(base).set_save_page_closed(1);
}

// The dialog menu's move: the cursor step with the side's page after, sound
// 4.
uint64_t MoveCursor(uint8_t* base, const Side& s, uint32_t frame, uint32_t lr,
                    const Calls& calls) {
  const PageObject p = Pages(base);
  const uint32_t current = p.page();
  if (current != s.page) {
    p.set_previous_page(current);
  }
  p.set_queued_step(kMinusOne);
  p.set_step(s.cursor_step);
  p.set_next_page(s.page);
  p.set_page(kMinusOne);
  return calls.Call(kMenuSoundPlay, frame, lr, 4);
}

// The device check (sub-page 0): unless SaveSlots::State(0) is 4, the
// check's wait with a device, else the question `no_device`.
uint64_t CheckDevice(uint8_t* base, const Side& s, uint32_t sp, uint32_t no_device,
                     const uint32_t (&lr)[2], const Calls& calls) {
  const uint32_t frame = Frame(base, sp, 96);
  const uint64_t state = calls.Call(kSlotState, frame, lr[0], 0);
  if (int32_t(state) == 4) {
    return state;
  }
  if (uint32_t(calls.Call(kSlotDevice, frame, lr[1], 0)) != 0) {
    SetDialog(base, s, 0, 2);
    return 0;
  }
  SetDialog(base, s, no_device, 0);
  return no_device;
}

// The leave step (41 or 42): first pass the dialog's animations finished,
// the close played, Options::SetUpCategoryPage; every pass UiMovie::Run, and
// once it answers 0 the page block to step 5 with the categories after and
// SaveBuffers::Destroy. 0.
uint64_t Leave(uint8_t* base, const schema::OptionsLeaveStarted& step, uint32_t sp, const Calls& calls, uint64_t first,
               uint64_t close, const uint32_t (&lr)[6]) {
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t m = FindMovie(base, frame);
  if (step.started() == 0) {
    const uint64_t cursor = DialogMenu(base).cursor();
    calls.Call(kFinishNamed, frame, lr[0], m, first);
    calls.Call(kFinishNamed, frame, lr[1], m, cursor + first + 2);
    calls.Call(kPlay, frame, lr[2], m, kMinusOne64, close, 0);
    SetUpCategoryPage(base, frame, calls);
    step.set_started(1);
  }
  if (int32_t(calls.Call(kRun, frame, lr[3], m, 0)) == 0) {
    const PageObject p = Pages(base);
    const uint32_t current = p.page();
    if (current != 0) {
      p.set_previous_page(current);
    }
    p.set_queued_step(kMinusOne);
    p.set_next_page(0);
    p.set_step(5);
    p.set_page(kMinusOne);
    Pages(base).set_step_started(0);
    calls.Call(kSaveBuffersDestroy, frame, lr[4]);
  }
  return 0;
}

}  // namespace

uint64_t LoadPageSetDialog(uint8_t* base, uint64_t kind, uint64_t next) {
  SetDialog(base, kLoad, uint32_t(kind), uint32_t(next));
  return kind;
}

uint64_t SavePageSetDialog(uint8_t* base, uint64_t kind, uint64_t next) {
  SetDialog(base, kSave, uint32_t(kind), uint32_t(next));
  return kind;
}

void LoadPageClose(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821B9FB0 LoadPage::Close");
  ClosePage(base, kLoad);
}

void SavePageClose(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821BA508 SavePage::Close");
  ClosePage(base, kSave);
}

void SavePagesCancel(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821B9FF8 OptionsSavePages::Cancel");
  Settings(base).set_save_page_closed(1);
}

uint64_t LoadPageCheckDevice(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BBC10 LoadPage::CheckDevice");
  static constexpr uint32_t kLr[2] = {0x821BBC24, 0x821BBC34};
  return CheckDevice(base, kLoad, sp, 7, kLr, calls);
}

uint64_t SavePageCheckDevice(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BBCC0 SavePage::CheckDevice");
  static constexpr uint32_t kLr[2] = {0x821BBCD4, 0x821BBCE4};
  return CheckDevice(base, kSave, sp, 11, kLr, calls);
}

uint64_t LoadPageDeviceAnswer(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BBC68 LoadPage::DeviceAnswer");
  const uint32_t frame = Frame(base, sp, 96);
  const uint64_t answer = calls.Call(kLookUp, frame, 0x821BBC7C, 0);
  uint32_t kind;
  switch (int32_t(answer)) {
    case 3:
      calls.Call(kRequest980, frame, 0x821BBCA4, 0);
      kind = 10;
      break;
    case 7: kind = 10; break;
    case 8: kind = 11; break;
    default: return answer;
  }
  SetDialog(base, kLoad, kind, 1);
  return kind;
}

uint64_t LoadPageAnswer(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BC7B0 LoadPage::Answer");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kMenuStep, frame, 0x821BC7D0, DialogMenu(base).address);
  const FrontEndMenu menu = DialogMenu(base);
  const uint32_t moved = menu.moved();
  const int32_t cursor = int32_t(menu.cursor());
  if (moved != 0) {
    return MoveCursor(base, kLoad, frame, 0x821BC820, calls);
  }
  const uint64_t accept = calls.Call(kMenuPressed, frame, 0x821BC82C, menu.address, 159);
  if (int32_t(accept) == 0) {
    return calls.Call(kMenuPressed, frame, 0x821BC928, DialogMenu(base).address, 158);
  }
  uint64_t r3 = calls.Call(kMenuSoundPlay, frame, 0x821BC83C, 0);
  const int32_t kind = int32_t(Kind(base, kLoad));
  const SubPageBlock pages = SubPages(base, kLoad);
  if (kind < 0) {
    return r3;
  }
  if (kind <= 1) {
    if (cursor == 0) {
      calls.Call(kGameDataLoad, frame, 0x821BC910, 0);
      SetDialog(base, kLoad, 8, 2);
      return 8;
    }
    SetDialog(base, kLoad, 6, 0);
    return 6;
  }
  if (kind == 4) {
    if (cursor == 0) {
      r3 = calls.Call(kRequestDevice, frame, 0x821BC8A4, 0);
      SetSubPage(SubPages(base, kLoad), 0, 0);
      return r3;
    }
    SetDialog(base, kLoad, 6, 0);
    return 6;
  }
  if (kind < 4) {
    return r3;
  }
  if (kind <= 6) {
    if (cursor == 0) {
      SetSubPage(pages, 4, 0);
      return r3;
    }
    SetDialog(base, kLoad, 0, 2);
    return 0;
  }
  if (kind != 7) {
    return r3;
  }
  if (cursor == 0) {
    SetSubPage(pages, 4, 0);
    return r3;
  }
  r3 = calls.Call(kRequestDevice, frame, 0x821BC8A4, 0);
  SetSubPage(SubPages(base, kLoad), 0, 0);
  return r3;
}

uint64_t SavePageAnswer(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BCA60 SavePage::Answer");
  const uint32_t frame = Frame(base, sp, 96);
  calls.Call(kMenuStep, frame, 0x821BCA7C, DialogMenu(base).address);
  const FrontEndMenu menu = DialogMenu(base);
  if (menu.moved() != 0) {
    return MoveCursor(base, kSave, frame, 0x821BCAC8, calls);
  }
  const uint64_t accept = calls.Call(kMenuPressed, frame, 0x821BCAD4, menu.address, 159);
  if (int32_t(accept) == 0) {
    return calls.Call(kMenuPressed, frame, 0x821BCBF8, DialogMenu(base).address, 158);
  }
  const int32_t cursor = int32_t(DialogMenu(base).cursor());
  uint64_t r3 = calls.Call(kMenuSoundPlay, frame, 0x821BCAEC, 0);
  const uint32_t kind = Kind(base, kSave);
  // Sub-page 0 then SaveSlots::RequestDevice(0).
  const auto device = [&]() {
    SetSubPage(SubPages(base, kSave), 0, 0);
    return calls.Call(kRequestDevice, frame, 0x821BCB80, 0);
  };
  if (kind < 2 || kind == 5) {
    if (cursor == 0) {
      SetDialog(base, kSave, 8, 2);
      return calls.Call(kGameDataSave, frame, 0x821BCBD8, 8, 2);
    }
    SetDialog(base, kSave, 6, 0);
    return 6;
  }
  if (kind == 4) {
    if (cursor == 0) {
      return device();
    }
    SetDialog(base, kSave, 6, 0);
    return 6;
  }
  if (kind == 6) {
    if (cursor == 0) {
      SetSubPage(SubPages(base, kSave), 4, 0);
      return r3;
    }
    SetDialog(base, kSave, 0, 2);
    return 0;
  }
  if (kind == 11) {
    if (cursor == 0) {
      SetSubPage(SubPages(base, kSave), 4, 0);
      return r3;
    }
    return device();
  }
  return r3;
}

namespace {

// A note (sub-page 2): Menu::Step, the counter up by one; from 45 with the
// kind 3, SaveSlots::RequestDevice(0) and sub-page 0; else on accept sound 0
// and `on_kind` with the kind.
template <typename OnKind>
uint64_t Note(uint8_t* base, const Side& s, uint32_t sp, const uint32_t (&lr)[4],
              const Calls& calls, OnKind on_kind) {
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kMenuStep, frame, lr[0], DialogMenu(base).address);
  const OptionsSettings o = Settings(base);
  (o.*s.set_counter)((o.*s.counter)() + 1);
  const OptionsSettings o2 = Settings(base);
  const auto device = [&]() {
    const uint64_t r3 = calls.Call(kRequestDevice, frame, lr[1], 0);
    SetSubPage(SubPages(base, s), 0, 1);
    return r3;
  };
  if (int32_t((o2.*s.counter)()) >= 45 && int32_t((o2.*s.kind)()) == 3) {
    return device();
  }
  const uint64_t accept = calls.Call(kMenuPressed, frame, lr[2], DialogMenu(base).address, 159);
  if (int32_t(accept) == 0) {
    return accept;
  }
  const uint64_t r3 = calls.Call(kMenuSoundPlay, frame, lr[3], 0);
  return on_kind(Kind(base, s), r3, device);
}

}  // namespace

uint64_t LoadPageNote(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BC940 LoadPage::Note");
  static constexpr uint32_t kLr[4] = {0x821BC960, 0x821BC998, 0x821BC9C4, 0x821BC9D4};
  return Note(base, kLoad, sp, kLr, calls, [&](uint32_t kind, uint64_t r3, auto device) {
    switch (kind) {
      case 3: return device();
      case 4:
      case 5:
      case 10:
      case 11:
        SetDialog(base, kLoad, 6, 0);
        return uint64_t(6);
      case 0:
      case 1:
      case 2:
      case 6:
      case 7:
      case 8:
      case 9:
      case 12:
        SetSubPage(SubPages(base, kLoad), 4, 1);
        return r3;
      default: return r3;
    }
  });
}

uint64_t SavePageNote(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BCC10 SavePage::Note");
  static constexpr uint32_t kLr[4] = {0x821BCC30, 0x821BCC68, 0x821BCC94, 0x821BCCA4};
  return Note(base, kSave, sp, kLr, calls, [&](uint32_t kind, uint64_t r3, auto device) {
    switch (int32_t(kind)) {
      case 2:
      case 9:
        SetSubPage(SubPages(base, kSave), 4, 1);
        return r3;
      case 3: return device();
      case 7:
      case 10:
        SetDialog(base, kSave, 6, 0);
        return uint64_t(6);
      default: return r3;
    }
  });
}

uint64_t LoadPageWait(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C2620 LoadPage::Wait");
  const uint32_t frame = Frame(base, sp, 96);
  const uint64_t state = calls.Call(kSlotState, frame, 0x821C2638, 0);
  const int32_t s = int32_t(state);
  if (s == 1) {
    SetDialog(base, kLoad, 4, 0);
    return 4;
  }
  const OptionsSettings o = Settings(base);
  (o.*kLoad.set_counter)((o.*kLoad.counter)() + 1);
  const OptionsSettings o2 = Settings(base);
  if (int32_t((o2.*kLoad.counter)()) < 90) {
    return state;
  }
  const uint32_t kind = (o2.*kLoad.kind)();
  if (kind == 0) {
    const uint64_t check = calls.Call(kGameDataCheck, frame, 0x821C2738, 0);
    switch (uint32_t(check)) {
      case 0:
      case 2:
        SetDialog(base, kLoad, 5, 1);
        return 5;
      case 1:
        SetDialog(base, kLoad, 1, 0);
        return 1;
      default: return check;
    }
  }
  if (kind >= 9 || s == 8) {
    return state;
  }
  if (s == 2) {
    const uint64_t word = calls.Call(kWord992, frame, 0x821C269C, 0);
    const uint64_t r3 = calls.Call(kRequest980, frame, 0x821C26A8, 0);
    if (int32_t(word) == 4) {
      const SubPageBlock pages = SubPages(base, kLoad);
      const uint32_t current = pages.page();
      if (current != 6) {
        pages.set_previous_page(current);
      }
      pages.set_step(kMinusOne);
      pages.set_page(6);
      pages.set_queued_step(kMinusOne);
    }
    return r3;
  }
  if (s != 3) {
    return state;
  }
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t buffer = schema::SaveInterfaceGlobals::save_buffers(base).game_data();
  if (schema::OptionsLoadedGameData(base, buffer).garage_count() == 0) {
    SetDialog(base, kLoad, 12, 1);
    return 12;
  }
  if (!native::Buffers_Memcpy(base, save, buffer, kGameDataBytes, 0x821C2714)) {
    calls.Call(kMemcpy, frame, 0x821C2714, save, buffer, kGameDataBytes);
  }
  SetDialog(base, kLoad, 9, 1);
  return SettingsReload(base, frame, calls);
}

uint64_t SavePageWait(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BBD18 SavePage::Wait");
  const uint32_t frame = Frame(base, sp, 96);
  const uint64_t state = calls.Call(kSlotState, frame, 0x821BBD30, 0);
  const int32_t s = int32_t(state);
  if (s == 1) {
    SetDialog(base, kSave, 4, 0);
    return 4;
  }
  const OptionsSettings o = Settings(base);
  (o.*kSave.set_counter)((o.*kSave.counter)() + 1);
  const OptionsSettings o2 = Settings(base);
  if (int32_t((o2.*kSave.counter)()) < 90) {
    return state;
  }
  const int32_t kind = int32_t((o2.*kSave.kind)());
  if (kind == 0) {
    const uint64_t check = calls.Call(kGameDataCheck, frame, 0x821BBEB8, 0);
    switch (uint32_t(check)) {
      case 0:
        SetDialog(base, kSave, 1, 0);
        return 1;
      case 1:
        SetDialog(base, kSave, 5, 0);
        return 5;
      case 2:
        SetDialog(base, kSave, 7, 1);
        return 7;
      default: return check;
    }
  }
  if (kind != 8 || s == 7) {
    return state;
  }
  // The note (sub-page 2) of `note` written inline: the kind, the next state
  // 1, the sub-pages' block, then p to steps 47 and 46 with page 10 after
  // (`p_order` 0: +4, +0, +12, +16; 1: +0, +12, +16, +4).
  const auto inline_note = [&](uint32_t note, int p_order) {
    SetKind(base, kSave, note);
    SetNext(base, kSave, 1);
    SetSubPage(SubPages(base, kSave), 2, 0);
    const PageObject p = Pages(base);
    const uint32_t current = p.page();
    if (current != 10) {
      p.set_previous_page(current);
    }
    if (p_order == 0) {
      p.set_next_page(10);
      p.set_page(kMinusOne);
      p.set_step(47);
      p.set_queued_step(46);
    } else {
      p.set_page(kMinusOne);
      p.set_step(47);
      p.set_queued_step(46);
      p.set_next_page(10);
    }
  };
  if (s == 2) {
    const uint64_t word = calls.Call(kWord992, frame, 0x821BBD90, 0);
    if (int32_t(word) == 2) {
      inline_note(10, 0);
    } else if (int32_t(word) == 3) {
      SetDialog(base, kSave, 7, 1);
    }
    return calls.Call(kRequest980, frame, 0x821BBE2C, 0);
  }
  if (s == 3) {
    inline_note(9, 1);
  }
  return state;
}

uint64_t LoadPageLeave(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C6468 LoadPage::Leave");
  static constexpr uint32_t kLr[6] = {0x821C64B8, 0x821C64C4, 0x821C64E4,
                                      0x821C64FC, 0x821C6544, 0};
  return Leave(base, schema::OptionsLeaveStarted(base, step), sp, calls, 841, 10, kLr);
}

uint64_t SavePageLeave(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C6550 SavePage::Leave");
  static constexpr uint32_t kLr[6] = {0x821C65A0, 0x821C65AC, 0x821C65CC,
                                      0x821C65E4, 0x821C662C, 0};
  return Leave(base, schema::OptionsLeaveStarted(base, step), sp, calls, 857, 11, kLr);
}

}  // namespace options_screen
