// The garage screen's pages never seen in play (the car's menu, the view and
// its options) and the machine's reply handlers
// (docs/functions/garage-screen.md): see garage_screen.h. OnYes stays the
// original: it calls Garage::Sell (sub_82116D88, career.md), recompiled.

#include "garage_screen/garage_screen.h"

#include "base/log.h"
#include "garage_screen/internal.h"
#include "race_screens/race_screens.h"
#include "schema/racer_controllers.h"
#include "type67_modes/type67_modes.h"

namespace garage_screen {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kHeldSlot = 32;   // the mapper's vtable slot 8 (ActionMapper::Held)
constexpr uint32_t kFiredSlot = 48;  // slot 12 (ActionMapper::Fired)

// The player's action mapper entry (0x826BAB80 + 8 x [0x826BABA4]).
uint32_t Entry() {
  return race_screens::kMapperEntries + (Load(race_screens::kControllerEntries) << 3);
}

// The mapper's slot on `action` through the guest (from `frame`, back to
// `lr`), unless the entry's +4 is 1 (then not asked: false, r3 kept). Its r3
// into `*r3`; whether its low byte is set.
bool Ask(uint32_t slot, uint32_t action, uint32_t frame, uint32_t lr,
         const Calls& calls, uint64_t* r3) {
  const schema::PlayerMapperEntry entry(Entry());
  if (int32_t(entry.unread()) == 1) {
    return false;
  }
  const uint32_t mapper = entry.mapper<schema::View>().address;
  *r3 = calls.Call(Load(Load(mapper) + slot), frame, lr, mapper, action);
  return (*r3 & 0xFF) != 0;
}

// The page object's move the pages make: +8 the page before unless it is
// `unless`, then the words in the original's order (stores of -1 written
// for kNone too).
PageObject Pages(uint32_t unless) {
  const PageObject pages = PagesOf();
  const uint32_t page = pages.page();
  if (page != unless) {
    pages.set_previous_page(page);
  }
  return pages;
}

uint64_t Sound(uint32_t id, uint32_t frame, uint32_t lr, const Calls& calls) {
  return calls.Call(kMenuSoundPlay, frame, lr, id);
}

// The view's shared buttons (both pages, in order): 163 and 164 the
// camera's commands 4 and 5, 173 and 174 the turns, 171 and 172 the camera
// presets, each by its own hold (slot 8). r3 as the last call leaves it.
struct ViewLrs {
  uint32_t ask[6], call[6];
};
void ViewButtons(uint32_t frame, const ViewLrs& lrs, const Calls& calls,
                 uint64_t* r3) {
  static constexpr uint32_t kActions[6] = {163, 164, 173, 174, 171, 172};
  for (uint32_t k = 0; k < 6; k++) {
    if (!Ask(kHeldSlot, kActions[k], frame, lrs.ask[k], calls, r3)) {
      continue;
    }
    switch (k) {
      case 0:
      case 1:
        *r3 = type67_modes::GarageStageCameraCommand(Holder(), 4 + k, frame);
        break;
      case 2: *r3 = TurnLeft(frame, calls); break;
      case 3: *r3 = TurnRight(frame, calls); break;
      case 4: *r3 = CameraA(frame, calls); break;
      default: *r3 = CameraB(frame, calls); break;
    }
  }
}

}  // namespace

uint64_t ActionsPage(uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821941A8 GarageScreen::ActionsPage");
  const uint32_t frame = Frame(sp, 144);
  calls.Call(kListUpdate, frame, 0x821941C4, List());
  calls.Call(kListUpdate, frame, 0x821941D0, List2());
  const uint32_t list = List();
  auto moved = [](uint32_t l) {
    const MenuList m(l);
    return m.move() != 0 || int32_t(m.scroll() - m.scroll_before()) != 0;
  };
  auto go = [](uint32_t unless, uint32_t transition, uint32_t page) {
    const PageObject pages = Pages(unless);
    pages.set_page(kNone);
    pages.set_next_page(page);
    pages.set_queued_step(kNone);
    pages.set_step(transition);
  };
  if (moved(list)) {
    // The action moved.
    go(1, 6, 1);
    return Sound(4, frame, 0x82194678, calls);
  }
  if (moved(List2())) {
    // Another car: transition 7 then 5.
    const PageObject pages = Pages(1);
    pages.set_step(7);
    pages.set_queued_step(5);
    pages.set_next_page(1);
    pages.set_page(kNone);
    return Sound(4, frame, 0x82194678, calls);
  }
  if (uint32_t(calls.Call(kListAccepted, frame, 0x82194214, list)) == 0) {
    const uint64_t cancelled = calls.Call(kListCancelled, frame, 0x821945CC, List());
    if (uint32_t(cancelled) == 0) {
      return cancelled;
    }
    // Back to the list.
    go(0, 8, 0);
    return Sound(2, frame, 0x82194678, calls);
  }
  const uint32_t action = race_screens::ListSelection(List());
  const ScreenData data = Data();
  const uint32_t index = data.scroll() + data.selection();
  const uint32_t car = GarageRecord(data.save(), index) + 212;
  const bool current =
      int32_t(index) == int32_t(data.save().selected_car());
  uint32_t prompts[6] = {0, 0, kNone, kNone, kNone, kNone};
  if (action == 0 || action == 1) {
    // Make it the current car, or sell it: a question (the dialog's message
    // 34 or 36), not for the current car (sound 3).
    if (current) {
      return Sound(3, frame, 0x82194678, calls);
    }
    if (action == 0) {
      SetUseQuestion(car, frame, calls);
    } else {
      SetSaleQuestion(car, frame, calls);
      prompts[2] = prompts[3] = 1;
    }
    const PageObject pages = Pages(kNone);
    pages.set_queued_step(kNone);
    pages.set_step(kNone);
    pages.set_page(kNone);
    Post(uint32_t(mode), action == 0 ? 34 : 36);
    Data().set_question(action == 0 ? 1 : 2);
  } else if (action == 2) {
    // The plate editor (message 28), transition 16.
    SetUpPlateEditor(car, frame, calls);
    Post(uint32_t(mode), 28);
    const PageObject pages = Pages(kNone);
    pages.set_step(16);
    pages.set_next_page(kNone);
    pages.set_queued_step(kNone);
    pages.set_page(kNone);
    prompts[2] = prompts[3] = 1;
  } else if (action == 3) {
    // The view: transition 9, page 2.
    go(2, 9, 2);
    return Sound(0, frame, 0x82194678, calls);
  } else if (action == 4) {
    // The spec sheet (message 24).
    SetUpSpecSheet(car, frame, calls);
    Post(uint32_t(mode), 24);
    const PageObject pages = Pages(kNone);
    pages.set_step(kNone);
    pages.set_queued_step(kNone);
    pages.set_page(kNone);
    prompts[1] = 1;
  } else {
    return action;
  }
  SetPrompts(prompts, frame);
  return Sound(0, frame, 0x82194678, calls);
}

uint64_t ViewPage(uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82191DE0 GarageScreen::ViewPage");
  const uint32_t frame = Frame(sp, 112);
  uint64_t r3 = mode;
  if (Ask(kFiredSlot, 158, frame, 0x82191E30, calls, &r3)) {
    // Back to the car's menu: transition 11, page 1.
    const PageObject pages = Pages(1);
    pages.set_queued_step(kNone);
    pages.set_step(11);
    pages.set_page(kNone);
    pages.set_next_page(1);
    r3 = Sound(2, frame, 0x82191EDC, calls);
  } else if (Ask(kFiredSlot, 157, frame, 0x82191E94, calls, &r3)) {
    // The view's options: transitions 12 then 13, page 3.
    const PageObject pages = Pages(3);
    pages.set_step(12);
    pages.set_queued_step(13);
    pages.set_page(kNone);
    pages.set_next_page(3);
    r3 = Sound(0, frame, 0x82191EDC, calls);
  }
  static constexpr ViewLrs kLrs = {
      {0x82191F04, 0x82191F40, 0x82191F7C, 0x82191FAC, 0x82191FDC, 0x8219200C},
      {0x82191F1C, 0x82191F58, 0x82191F88, 0x82191FB8, 0x82191FE8, 0x82192018}};
  ViewButtons(frame, kLrs, calls, &r3);
  return r3;
}

uint64_t ViewOptionsPage(uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82192030 GarageScreen::ViewOptionsPage");
  const uint32_t frame = Frame(sp, 112);
  uint64_t r3 = calls.Call(kListUpdate, frame, 0x82192068, List());
  // The move to the page: transition and page, then the sound.
  auto go = [&](uint32_t unless, uint32_t transition, uint32_t page, uint32_t sound) {
    const PageObject pages = Pages(unless);
    pages.set_step(transition);
    pages.set_queued_step(kNone);
    pages.set_next_page(page);
    pages.set_page(kNone);
    r3 = Sound(sound, frame, 0x82192220, calls);
  };
  if (ListView().move() != 0) {
    go(3, 14, 3, 4);  // the option moved
  } else if (Ask(kFiredSlot, 159, frame, 0x821920C4, calls, &r3)) {
    const uint32_t option = race_screens::ListSelection(List());
    r3 = option;
    if (option == 0) {
      Sound(0, frame, 0x8219215C, calls);
      r3 = ToggleTurn(frame, 0x82192160, calls);
    } else if (option == 1 || option == 2) {
      // The doors: not for a car whose spec says so (the data's +80; sound
      // 3).
      uint32_t sound = 3;
      if (Data().toggles_shown() == 0) {
        if (option == 1) {
          ToggleDoors(frame, 0x82192150, calls);
        } else {
          ToggleDoors2(frame, 0x82192128, calls);
        }
        sound = 0;
      }
      r3 = Sound(sound, frame, 0x82192220, calls);
    } else if (option == 3) {
      go(1, 15, 1, 0);  // back to the car's menu
    }
  } else if (Ask(kHeldSlot, 158, frame, 0x82192188, calls, &r3)) {
    go(1, 15, 1, 2);  // back to the car's menu
  } else if (Ask(kHeldSlot, 157, frame, 0x821921DC, calls, &r3)) {
    go(2, 10, 2, 0);  // back to the view
  }
  static constexpr ViewLrs kLrs = {
      {0x82192248, 0x82192284, 0x821922C0, 0x821922F0, 0x82192320, 0x82192350},
      {0x82192260, 0x8219229C, 0x821922CC, 0x821922FC, 0x8219232C, 0x8219235C}};
  ViewButtons(frame, kLrs, calls, &r3);
  return r3;
}

uint64_t OnSpecSheetClosed(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82193E38 GarageScreen::OnSpecSheetClosed");
  const PageObject pages = Pages(1);
  pages.set_step(kNone);
  pages.set_page(1);
  pages.set_queued_step(kNone);
  static constexpr uint32_t kPrompts[6] = {0, 0, 1, 1, kNone, kNone};
  return SetPrompts(kPrompts, sp);
}

void OnPlateCancelled(uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_821928F8 GarageScreen::OnPlateCancelled");
  Data().set_plate_pending(0);
  Post(mode, 32);
}

uint64_t OnPlateDone(uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82193E80 GarageScreen::OnPlateDone");
  const uint32_t frame = Frame(sp, 112);
  Data().set_plate_pending(1);
  const uint64_t r3 = SetPlateQuestion(frame, calls);
  Post(mode, 34);
  Data().set_question(0);
  return r3;
}

uint64_t OnPlateEditorLeft(uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82193F18 GarageScreen::OnPlateEditorLeft");
  if (Data().plate_pending() != 0) {
    return r3;
  }
  const PageObject pages = Pages(1);
  pages.set_next_page(1);
  pages.set_queued_step(kNone);
  pages.set_step(17);
  pages.set_page(kNone);
  static constexpr uint32_t kPrompts[6] = {0, 0, 1, 1, kNone, kNone};
  return SetPrompts(kPrompts, sp);
}

void OnNo(uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82192BA8 GarageScreen::OnNo");
  Data().set_answered_yes(0);
  if (Data().question() == 0) {
    Post(mode, 32);
  }
}

uint64_t OnDialogDone(uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82193F80 GarageScreen::OnDialogDone");
  const uint32_t frame = Frame(sp, 96);
  auto go = [](uint32_t unless, uint32_t transition, uint32_t page) {
    const PageObject pages = Pages(unless);
    pages.set_page(kNone);
    pages.set_next_page(page);
    pages.set_queued_step(kNone);
    pages.set_step(transition);
  };
  if (Data().answered_yes() != 0) {
    go(0, 8, 0);  // answered yes: back to the list
    return r3;
  }
  static constexpr uint32_t kPrompts[6] = {0, 0, 1, 1, kNone, kNone};
  r3 = SetPrompts(kPrompts, frame);
  const uint32_t question = Data().question();
  if (question == 0) {
    go(1, 17, 1);  // the plate's: back from the plate editor
  } else if (question < 3) {
    const PageObject pages = Pages(1);
    pages.set_step(kNone);
    pages.set_page(1);
    pages.set_queued_step(kNone);
  }
  return r3;
}

}  // namespace garage_screen
