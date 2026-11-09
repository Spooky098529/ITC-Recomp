// The garage screen's car menu, view and plate transitions never seen in
// play, with the actions' and the options' set-ups
// (docs/functions/garage-screen.md): see garage_screen.h. Transition11 and
// Transition15 stay the originals (their finish stores UiMovie::Run's r8
// leftover into the page object's started word: live state native code
// can't reproduce, the user's call for BuildScreen::BackToBuilds).

#include "garage_screen/garage_screen.h"

#include "base/log.h"
#include "car_specs/car_specs.h"
#include "garage_screen/internal.h"
#include "race_screens/race_screens.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace garage_screen {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint64_t kAll = 0xFFFFFFFFFFFFFFFFull;
constexpr uint32_t kCurrentCar = 0x19920;  // the save's current car index

uint64_t PlayNamed(uint32_t movie, uint64_t named, uint32_t frame, uint32_t lr,
                   const Calls& calls, uint64_t r5 = 0, uint64_t r6 = 0) {
  return calls.CallArgs(kPlayNamed, frame, lr, {movie, named, r5, r6, 0, 0, 0, 0});
}
uint64_t FinishNamed(uint32_t movie, uint64_t named, uint32_t frame, uint32_t lr,
                     const Calls& calls) {
  return calls.Call(kFinishNamed, frame, lr, movie, named);
}
uint64_t LoopNamed(uint32_t movie, uint64_t named, uint32_t frame, uint32_t lr,
                   const Calls& calls) {
  return calls.Call(kLoopNamed, frame, lr, movie, named);
}
uint64_t Run(uint32_t movie, uint32_t frame, uint32_t lr, const Calls& calls) {
  return calls.Call(kRun, frame, lr, movie, 0);
}

uint32_t Selection(uint8_t* base, uint32_t global) {
  return race_screens::ListSelection(base, Load(base, global));
}
// MenuList::GetPreviousSelection (sub_8220BC28, the list's +20), inline.
uint32_t PreviousSelection(uint8_t* base, uint32_t global) {
  return Load(base, Load(base, global) + 20);
}

// Whether the selected garage car (the data's +44 + +48) is the save's
// current one (+0x19920), compared signed.
bool Current(uint8_t* base) {
  const uint32_t data = Data(base);
  const uint32_t current = Load(base, Load(base, data + 36) + kCurrentCar);
  return int32_t(Load(base, data + 48) + Load(base, data + 44)) == int32_t(current);
}

// The actions' help line: the two five-entry tables in the frame (+96 the
// current car's, +128 another's, stored in the original's order), the
// action `selection` (no bounds check), SetHelpLine (its plain C++).
uint64_t ActionHelpLine(uint8_t* base, uint32_t frame, uint32_t selection, const Calls& calls) {
  Store(base, frame + 128, 559);
  Store(base, frame + 96, 585);
  Store(base, frame + 136, 561);
  Store(base, frame + 104, 561);
  Store(base, frame + 132, 560);
  Store(base, frame + 100, 586);
  Store(base, frame + 144, 563);
  Store(base, frame + 112, 563);
  Store(base, frame + 140, 553);
  Store(base, frame + 108, 553);
  const uint32_t table = Current(base) ? frame + 96 : frame + 128;
  return SetHelpLine(base, Load(base, table + (selection << 2)), frame, calls);
}

// The view options' help line: the two four-entry tables (+96 for a car
// whose spec's byte +771 is 1, `other` for the rest), the selected garage
// car copied to `car` (CopyGarageCar, from `frame`) for its spec.
uint64_t OptionHelpLine(uint8_t* base, uint32_t frame, uint32_t other, uint32_t car,
                        uint32_t selection, const Calls& calls) {
  Store(base, frame + other, 554);
  Store(base, frame + 96, 554);
  Store(base, frame + other + 4, 555);
  Store(base, frame + 100, 590);
  Store(base, frame + other + 8, 556);
  Store(base, frame + 104, 589);
  Store(base, frame + other + 12, 557);
  Store(base, frame + 108, 557);
  const uint32_t data = Data(base);
  CopyGarageCar(base, car, Load(base, data + 36) + 6120,
                Load(base, data + 48) + Load(base, data + 44), frame, calls);
  const uint32_t spec = car_specs::GetCarSpec(base, Load(base, car + 12));
  const uint32_t table = guest::LoadU8(base, spec + 771) == 1 ? frame + 96 : frame + other;
  return SetHelpLine(base, Load(base, table + (selection << 2)), frame, calls);
}

// The ring of 26 and 27 the view options' moves play for options 1 and 2
// when the data's +80 is set; else `named` played and looped.
void OptionHighlight(uint8_t* base, uint32_t movie, uint32_t selection, uint64_t named,
                     uint32_t frame, const uint32_t lrs[4], const uint32_t loop_lrs[2],
                     const Calls& calls) {
  if (Load(base, Data(base) + 80) != 0 && (int32_t(selection) == 1 || int32_t(selection) == 2)) {
    PlayNamed(movie, 26, frame, lrs[0], calls);
    FinishNamed(movie, 26, frame, lrs[1], calls);
    PlayNamed(movie, 27, frame, lrs[2], calls, 1, 1);
    FinishNamed(movie, 27, frame, lrs[3], calls);
    return;
  }
  PlayNamed(movie, named, frame, loop_lrs[0], calls);
  LoopNamed(movie, named, frame, loop_lrs[1], calls);
}

// Run's answer as the originals return it: 1 when it's 0.
uint64_t Done(uint64_t run) { return uint32_t(run) == 0 ? 1 : 0; }

// The garage car record `steps` links from the head of the save's garage
// list (a 32-bit count down to 0, as the originals walk it).
uint32_t RecordAfter(uint8_t* base, uint32_t steps) {
  const uint32_t garage = Load(base, Data(base) + 36) + 6120;
  uint32_t link = garage + 4 * uint32_t(int32_t(int16_t(guest::LoadU16(base, garage + 204))));
  for (; steps != 0; steps--) {
    link = garage + 4 * uint32_t(int32_t(int16_t(guest::LoadU16(base, link))));
  }
  return garage + 1968 * uint32_t(int32_t(int16_t(guest::LoadU16(base, link + 2))));
}

}  // namespace

uint64_t Transition2(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82194F30 GarageScreen::Transition2");
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  if (Load(base, started) == 0) {
    const uint32_t selection = Selection(base, kList);
    const uint32_t previous = PreviousSelection(base, kList);
    const uint32_t scroll = Load(base, Load(base, kList) + 4);
    const int32_t moved = int32_t(scroll - Load(base, Load(base, kList) + 8));
    const uint32_t marker = ui_movies::FindNode(base, movie, 6, 0);
    FinishNamed(movie, uint64_t(previous) + 67, frame, 0x82194FB4, calls);
    PlayNamed(movie, uint64_t(selection) + 67, frame, 0x82194FD4, calls);
    LoopNamed(movie, uint64_t(selection) + 67, frame, 0x82194FE0, calls);
    if (moved != 0) {
      const uint32_t count =
          uint32_t(int32_t(int16_t(guest::LoadU16(base, Load(base, Data(base) + 36) + 6328))));
      const uint32_t row = ui_movies::FindNode(base, movie, 6, 9);
      FillRows(base, frame, calls);
      FinishNamed(movie, 72, frame, 0x8219501C, calls);
      FinishNamed(movie, 73, frame, 0x82195028, calls);
      FinishNamed(movie, 75, frame, 0x82195034, calls);
      FinishNamed(movie, 76, frame, 0x82195040, calls);
      // The current car's marker one row on, as the list scrolls.
      const uint32_t current = Load(base, Load(base, Data(base) + 36) + kCurrentCar);
      if (Load(base, Load(base, kList) + 32) != 0) {
        // The row's y in 64 bits (the words zero-extended, as the original
        // adds them), through the frame's +80 (std, lfd, fcfid, frsp).
        const int64_t row_index = (moved > 0 ? 1 : -1) - int64_t(scroll) + int64_t(current);
        const int64_t y = row_index * 84 + 232;
        Store64(base, frame + 80, uint64_t(y));
        Stfs(base, marker + 16, double(float(double(y))));
      }
      // The row scrolled in: the car at scroll + 4 (up) or scroll - 1
      // (down) in row 9, then the scroll's animations.
      uint64_t second;
      if (moved < 0) {
        SetCarRow(base, row, RecordAfter(base, scroll + 4) + 212, frame, calls);
        PlayNamed(movie, 73, frame, 0x82195104, calls, 1);
        second = 76;
      } else {
        SetCarRow(base, row, RecordAfter(base, scroll - 1) + 212, frame, calls);
        PlayNamed(movie, 72, frame, 0x8219515C, calls, 1);
        second = 75;
      }
      PlayNamed(movie, second, frame, 0x82195178, calls, 1);
      SetScrollArrows(base, scroll, count, frame);
    }
    Store(base, started, 1);
  }
  Run(movie, frame, 0x82195198, calls);
  return 1;
}

uint64_t Transition4(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82195D10 GarageScreen::Transition4");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  if (Load(base, started) == 0) {
    FinishNamed(movie, uint64_t(Selection(base, kList)) + 67, frame, 0x82195D68, calls);
    FinishNamed(movie, 77, frame, 0x82195D74, calls);
    PlayNamed(movie, 132, frame, 0x82195D90, calls);
    Store(base, Data(base) + 52, 0);
    Store(base, Data(base) + 56, 0);
    SetUpActions(base, frame, calls);
    Store(base, started, 1);
  }
  const uint64_t done = Done(Run(movie, frame, 0x82195DB8, calls));
  if (done != 0) {
    const uint32_t color = Current(base) ? 2 : 0;
    ui_movies::SetColor(base, ui_movies::FindNode(base, movie, 7, 5), color);
    ui_movies::SetColor(base, ui_movies::FindNode(base, movie, 7, 6), color);
  }
  return done;
}

uint64_t Transition5(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82192EF8 GarageScreen::Transition5");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  if (Load(base, started) == 0) {
    const uint32_t action = Selection(base, kList);
    // No highlight for actions 0 and 1 (make current, sell) on the current
    // car.
    if (!Current(base) || (int32_t(action) != 0 && int32_t(action) != 1)) {
      PlayNamed(movie, uint64_t(action) + 80, frame, 0x82192F98, calls);
      LoopNamed(movie, uint64_t(action) + 80, frame, 0x82192FA4, calls);
    }
    Store(base, Data(base) + 72, 1);
    Store(base, started, 1);
  }
  Run(movie, frame, 0x82192FC0, calls);
  return 1;
}

uint64_t Transition6(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82194680 GarageScreen::Transition6");
  const uint32_t frame = Frame(base, sp, 224);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  if (Load(base, started) == 0) {
    const uint32_t action = Selection(base, kList);
    FinishNamed(movie, uint64_t(PreviousSelection(base, kList)) + 80, frame, 0x821946E0, calls);
    if (Current(base) && (int32_t(action) == 0 || int32_t(action) == 1)) {
      PlayNamed(movie, 104, frame, 0x82194738, calls);
      FinishNamed(movie, 104, frame, 0x82194744, calls);
      PlayNamed(movie, 105, frame, 0x82194760, calls, 1, 1);
      FinishNamed(movie, 105, frame, 0x8219476C, calls);
    } else {
      PlayNamed(movie, uint64_t(action) + 80, frame, 0x82194790, calls);
      LoopNamed(movie, uint64_t(action) + 80, frame, 0x8219479C, calls);
    }
    FinishNamed(movie, 90, frame, 0x821947A8, calls);
    FinishNamed(movie, 91, frame, 0x821947B4, calls);
    const uint32_t list = Load(base, kList);
    if (Load(base, list + 32) != 0) {
      SetActionMarker(base, action * 40, frame);
    } else {
      const int32_t move = int32_t(Load(base, Load(base, kList) + 24));
      if (move == 1) {
        PlayNamed(movie, 91, frame, 0x8219480C, calls);
      } else if (move == -1) {
        PlayNamed(movie, 90, frame, 0x8219480C, calls);
      }
    }
    ActionHelpLine(base, frame, Selection(base, kList), calls);
    Store(base, started, 1);
  }
  Run(movie, frame, 0x821948A0, calls);
  return 1;
}

uint64_t Transition7(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821951A8 GarageScreen::Transition7");
  const uint32_t frame = Frame(base, sp, 2192);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  if (Load(base, started) == 0) {
    // Another car: the second list's selection and scroll.
    const uint32_t selection = Selection(base, kList2);
    const uint32_t scroll = Load(base, Load(base, kList2) + 4);
    const uint32_t action = Selection(base, kList);
    Store(base, Data(base) + 44, selection);
    Store(base, Data(base) + 48, scroll);
    FinishNamed(movie, uint64_t(action) + 80, frame, 0x82195228, calls);
    PlayNamed(movie, 104, frame, 0x82195244, calls);
    FinishNamed(movie, 104, frame, 0x82195250, calls);
    PlayNamed(movie, 105, frame, 0x8219526C, calls, 1, 1);
    FinishNamed(movie, 105, frame, 0x82195278, calls);
    ReloadStandCar(base, frame, calls);
    const uint32_t record = RecordAfter(base, scroll + selection);
    Memcpy(base, calls, frame + 160, record + 212, 1968, frame, 0x821952C4);
    ShowCarSpecs(base, frame + 160, frame, calls);
    const uint32_t mark = ui_movies::FindNode(base, movie, 0xFFFFFFFFu, 5);
    Store(base, mark + 80, Current(base) ? 1 : 0);
    const uint32_t color = Current(base) ? 2 : 0;
    ui_movies::SetColor(base, ui_movies::FindNode(base, movie, 7, 5), color);
    ui_movies::SetColor(base, ui_movies::FindNode(base, movie, 7, 6), color);
    ActionHelpLine(base, frame, Selection(base, kList), calls);
    Store(base, started, 1);
  }
  Run(movie, frame, 0x82195400, calls);
  return 1;
}

uint64_t Transition8(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82195E30 GarageScreen::Transition8");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  if (Load(base, started) == 0) {
    FinishNamed(movie, uint64_t(Selection(base, kList)) + 80, frame, 0x82195E8C, calls);
    PlayNamed(movie, 133, frame, 0x82195EA8, calls);
    Store(base, Data(base) + 72, 0);
    SetUpLists(base, frame, calls);
    Store(base, started, 1);
  }
  if (uint32_t(Run(movie, frame, 0x82195EC8, calls)) == 0) {
    // Back on the list: transition 1 running now, page 0 after it.
    const uint32_t pages = Load(base, kPages);
    const uint32_t page = Load(base, pages + 0);
    if (page != 0) {
      Store(base, pages + 8, page);
    }
    Store(base, pages + 12, 1);
    Store(base, pages + 4, 0);
    Store(base, pages + 16, kNone);
    Store(base, pages + 0, kNone);
    Store(base, Load(base, kPages) + 24, 0);
  }
  return 0;
}

uint64_t Transition9(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82195410 GarageScreen::Transition9");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  if (Load(base, started) == 0) {
    FinishNamed(movie, uint64_t(Selection(base, kList)) + 80, frame, 0x8219546C, calls);
    PlayNamed(movie, 134, frame, 0x82195488, calls, 1);
    PlayNamed(movie, 135, frame, 0x821954A4, calls, 1);
    PlayNamed(movie, 136, frame, 0x821954C0, calls, 1);
    PlayNamed(movie, 137, frame, 0x821954DC, calls);
    PlayNamed(movie, 141, frame, 0x821954F8, calls, 1);
    SetUpView(base, frame, calls);
    Store(base, started, 1);
  }
  return Done(Run(movie, frame, 0x82195510, calls));
}

uint64_t Transition10(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82195530 GarageScreen::Transition10");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  if (Load(base, started) == 0) {
    FinishNamed(movie, uint64_t(Selection(base, kList)) + 2, frame, 0x8219558C, calls);
    PlayNamed(movie, 144, frame, 0x821955A8, calls);
    SetUpView(base, frame, calls);
    Store(base, started, 1);
  }
  return Done(Run(movie, frame, 0x821955C0, calls));
}

uint64_t Transition12(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821955E0 GarageScreen::Transition12");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  if (Load(base, started) == 0) {
    PlayNamed(movie, 143, frame, 0x82195634, calls);
    SetUpOptions(base, frame, calls);
    Store(base, started, 1);
  }
  const uint64_t done = Done(Run(movie, frame, 0x8219564C, calls));
  if (done != 0 && Load(base, Data(base) + 80) != 0) {
    // The two options a car of this spec can't take dimmed (color 2).
    const uint32_t node = ui_movies::FindNode(base, movie, 2, 1);
    ui_movies::SetColor(base, ui_movies::Child(base, node, 6), 2);
    ui_movies::SetColor(base, ui_movies::Child(base, node, 7), 2);
  }
  return done;
}

uint64_t Transition13(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82192FD0 GarageScreen::Transition13");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  if (Load(base, started) == 0) {
    const uint32_t option = Selection(base, kList);
    static constexpr uint32_t kLrs[4] = {0x82193054, 0x82193060, 0x8219307C, 0x82193088};
    static constexpr uint32_t kLoop[2] = {0x821930AC, 0x821930B8};
    OptionHighlight(base, movie, option, uint64_t(option) + 2, frame, kLrs, kLoop, calls);
    Store(base, started, 1);
  }
  Run(movie, frame, 0x821930CC, calls);
  return 1;
}

uint64_t Transition14(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821948B0 GarageScreen::Transition14");
  const uint32_t frame = Frame(base, sp, 2160);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  if (Load(base, started) == 0) {
    const uint32_t option = Selection(base, kList);
    const uint32_t previous = PreviousSelection(base, kList);
    const uint32_t marked = Load(base, Load(base, kList) + 32);
    FinishNamed(movie, uint64_t(previous) + 2, frame, 0x82194918, calls);
    static constexpr uint32_t kLrs[4] = {0x82194958, 0x82194964, 0x82194980, 0x8219498C};
    static constexpr uint32_t kLoop[2] = {0x821949B0, 0x821949BC};
    OptionHighlight(base, movie, option, uint64_t(option) + 2, frame, kLrs, kLoop, calls);
    FinishNamed(movie, 12, frame, 0x821949C8, calls);
    FinishNamed(movie, 13, frame, 0x821949D4, calls);
    if (marked != 0) {
      SetOptionMarker(base, option * 40, frame);
    } else {
      const int32_t move = int32_t(Load(base, Load(base, kList) + 24));
      PlayNamed(movie, move > 0 ? 13 : 12, frame, 0x82194A18, calls);
    }
    OptionHelpLine(base, frame, 112, frame + 128, Selection(base, kList), calls);
    Store(base, started, 1);
  }
  Run(movie, frame, 0x82194AB8, calls);
  return 1;
}

uint64_t Transition16(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821930D8 GarageScreen::Transition16");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  if (Load(base, started) == 0) {
    PlayNamed(movie, 146, frame, 0x8219312C, calls);
    const uint32_t a = ui_movies::FindNode(base, movie, 3, 1);
    const uint32_t b = ui_movies::FindNode(base, movie, 3, 2);
    Store(base, a + 108, 1);
    Store(base, b + 108, 3);
    Store(base, started, 1);
  }
  return Done(Run(movie, frame, 0x82193170, calls));
}

uint64_t Transition17(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82196170 GarageScreen::Transition17");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  if (Load(base, started) == 0) {
    PlayNamed(movie, 147, frame, 0x821961C4, calls);
    const uint32_t a = ui_movies::FindNode(base, movie, 3, 1);
    const uint32_t b = ui_movies::FindNode(base, movie, 3, 2);
    Store(base, a + 108, 3);
    Store(base, b + 108, 1);
    Store(base, Data(base) + 52, 2);
    SetUpActions(base, frame, calls);
    Store(base, started, 1);
  }
  return Done(Run(movie, frame, 0x8219621C, calls));
}

bool RunTransition(uint8_t* base, uint32_t target, uint32_t started, uint32_t sp,
                   const Calls& calls, uint64_t* r3) {
  using Fn = uint64_t (*)(uint8_t*, uint32_t, uint32_t, const Calls&);
  static constexpr struct {
    uint32_t address;
    Fn fn;
  } kNative[] = {
      {0x82195BA0, Transition0},  {0x82192D60, Transition1},  {0x82194F30, Transition2},
      {0x82192E20, Transition3},  {0x82195D10, Transition4},  {0x82192EF8, Transition5},
      {0x82194680, Transition6},  {0x821951A8, Transition7},  {0x82195E30, Transition8},
      {0x82195410, Transition9},  {0x82195530, Transition10}, {0x821955E0, Transition12},
      {0x82192FD0, Transition13}, {0x821948B0, Transition14}, {0x821930D8, Transition16},
      {0x82196170, Transition17}};
  for (const auto& entry : kNative) {
    if (entry.address == target) {
      *r3 = entry.fn(base, started, sp, calls);
      return true;
    }
  }
  return false;
}

uint64_t SetUpActions(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821958D0 GarageScreen::SetUpActions");
  const uint32_t frame = Frame(base, sp, 2192);
  const uint32_t movie = FindMovie(base, kMovie, frame + 116);
  // The five actions (MenuList::Init: accept 159 and back 158 at the
  // frame's +84 and +92).
  Store(base, frame + 92, 158);
  Store(base, frame + 84, 159);
  calls.CallArgs(kListInit, frame, 0x82195934, {Load(base, kList), 5, 5, 1, 154, 153, 155, 156});
  calls.Call(kListSetSelection, frame, 0x8219594C, Load(base, kList2),
             Load(base, Data(base) + 44));
  Store(base, Load(base, kList2) + 4, Load(base, Data(base) + 48));
  calls.Call(kListFit, frame, 0x82195968, Load(base, kList2), 1);
  calls.Call(kListSetSelection, frame, 0x82195978, Load(base, kList),
             Load(base, Data(base) + 52));
  SetActionMarker(base, Selection(base, kList) * 40, frame);
  static constexpr uint32_t kTexts[5] = {134, 135, 136, 137, 112};
  for (uint32_t i = 0; i < 5; i++) {
    Store(base, frame + 96 + 4 * i, kTexts[i]);
  }
  for (uint32_t i = 0; i < 5; i++) {
    SetText(calls, frame, 0x821959D0, movie, 7, 5 + i, Load(base, frame + 96 + 4 * i));
  }
  const uint32_t data = Data(base);
  const uint32_t record = RecordAfter(base, Load(base, data + 48) + Load(base, data + 44));
  Memcpy(base, calls, frame + 160, record + 212, 1968, frame, 0x82195A30);
  ShowCarSpecs(base, frame + 160, frame, calls);
  static constexpr uint32_t kPrompts[6] = {0, 0, 1, 1, kNone, kNone};
  SetPrompts(base, kPrompts, frame);
  const uint32_t mark = ui_movies::FindNode(base, movie, 0xFFFFFFFFu, 5);
  Store(base, mark + 80, Current(base) ? 1 : 0);
  Store(base, Load(base, Holder(base)) + 6052, 1);
  ActionHelpLine(base, frame, Selection(base, kList), calls);
  // The list's arrows and the other-car buttons hidden with one car.
  const uint32_t count = guest::LoadU16(base, Load(base, Data(base) + 36) + 6328);
  const uint32_t a = ui_movies::FindNode(base, movie, 0xFFFFFFFFu, 0);
  const uint32_t b = ui_movies::FindNode(base, movie, 0xFFFFFFFFu, 13);
  const uint32_t c = ui_movies::FindNode(base, movie, 0xFFFFFFFFu, 14);
  const uint32_t shown = count == 1 ? 0 : 1;
  Store(base, a + 80, shown);
  Store(base, b + 80, shown);
  Store(base, c + 80, shown);
  return c;
}

uint64_t SetUpOptions(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82194B20 GarageScreen::SetUpOptions");
  const uint32_t frame = Frame(base, sp, 2160);
  const uint32_t movie = FindMovie(base, kMovie, frame + 112);
  Store(base, frame + 92, 158);
  Store(base, frame + 84, 159);
  calls.CallArgs(kListInit, frame, 0x82194B84, {Load(base, kList), 4, 4, 1, 154, 153, 155, 156});
  SetOptionMarker(base, Selection(base, kList) * 40, frame);
  static constexpr uint32_t kTexts[4] = {131, 132, 133, 115};
  for (uint32_t i = 0; i < 4; i++) {
    Store(base, frame + 96 + 4 * i, kTexts[i]);
  }
  const uint32_t node = ui_movies::FindNode(base, movie, 2, 1);
  for (uint32_t i = 0; i < 4; i++) {
    calls.Call(kSetNodeText, frame, 0x82194BEC, movie, ui_movies::Child(base, node, i + 5),
               Load(base, frame + 96 + 4 * i));
  }
  // The data's +80: the car's spec's byte +771 is 1 (its options 1 and 2
  // dimmed by Transition12).
  const uint32_t data = Data(base);
  CopyGarageCar(base, frame + 144, Load(base, data + 36) + 6120,
                Load(base, data + 48) + Load(base, data + 44), frame, calls);
  const uint32_t spec = car_specs::GetCarSpec(base, Load(base, frame + 156));
  Store(base, Data(base) + 80, guest::LoadU8(base, spec + 771) == 1 ? 1 : 0);
  return OptionHelpLine(base, frame, 128, frame + 144, Selection(base, kList), calls);
}

}  // namespace garage_screen
