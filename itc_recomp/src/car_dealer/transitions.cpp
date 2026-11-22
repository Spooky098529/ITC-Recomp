// The car dealer's page transitions (docs/functions/car-dealer.md): see
// car_dealer.h. Each is called by CarDealer::Update with the page object's
// +24 (the started word) and answers whether it's done.

#include "car_dealer/car_dealer.h"

#include "base/log.h"
#include "car_dealer/internal.h"
#include "guest.h"
#include "schema/mode_tick.h"
#include "schema/race_menu.h"
#include "ui_movies/game.h"

namespace car_dealer {

namespace {

// The screen fades' counts are mode-tick.toml's; FadeOut's passes word
// (0x82001BF0) race-menu.toml's.
using Ticks = schema::ModeTickGlobals;
using Fades = schema::RaceMenuGlobals;
constexpr uint32_t kNone = 0xFFFFFFFFu;

// The named animation calls on movie 4 (UiMovie::PlayNamed with r5, r6 to
// r8 0; LoopNamed; FinishNamed) and UiMovie::Run(movie, 0).
uint64_t Play(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t id,
              uint64_t r5 = 0) {
  return calls.Call(kPlayNamed, frame, lr, movie, id, r5, 0);
}
uint64_t Loop(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t id) {
  return calls.Call(kLoopNamed, frame, lr, movie, id);
}
uint64_t Finish(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t id) {
  return calls.Call(kFinishNamed, frame, lr, movie, id);
}
uint64_t Run(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie) {
  return calls.Call(kRun, frame, lr, movie, 0);
}
uint64_t Selection(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t lr) {
  return calls.Call(kListSelection, frame, lr, G::list_menu(base).address);
}

// Transition0's and Transition5's end: 75 and 77 played and looped.
void Rest(const Calls& calls, uint32_t frame, uint32_t movie, const uint32_t lr[4]) {
  Play(calls, frame, lr[0], movie, 75);
  Loop(calls, frame, lr[1], movie, 75);
  Play(calls, frame, lr[2], movie, 77);
  Loop(calls, frame, lr[3], movie, 77);
}

}  // namespace

uint64_t Transition3(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82181D68 CarDealer::Transition3");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  if (Load(base, step) == 0) {
    calls.Call(kFadeOut, frame, 0x82181DC4, Fades::fade_out_passes(base), 0, 0, 0);
    Store(base, step, 1);
  }
  calls.Call(kRun, frame, 0x82181DD8, movie, 0);
  return (Ticks::fade_out_count_0(base) | Ticks::fade_in_count_0(base)) == 0 ? 1 : 0;
}

uint64_t Transition0(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82184C70 CarDealer::Transition0");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  if (Load(base, step) == 0) {
    G::data(base).set_kind_selection(0);
    OpenListMenu(base, frame, calls);
    SetHeader(base, 0, 0, frame);
    Play(calls, frame, 0x82184CEC, movie, 149, 1);
    Play(calls, frame, 0x82184D08, movie, 83, 1);
    Loop(calls, frame, 0x82184D14, movie, 83);
    Store(base, step, 1);
  }
  const bool done = uint32_t(Run(calls, frame, 0x82184D28, movie)) == 0;
  if (done) {
    const uint32_t lr[4] = {0x82184D50, 0x82184D5C, 0x82184D78, 0x82184D84};
    Rest(calls, frame, movie, lr);
  }
  return done ? 1 : 0;
}

uint64_t Transition4(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82184DA0 CarDealer::Transition4");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  if (Load(base, step) == 0) {
    const uint64_t selection = Selection(base, calls, frame, 0x82184DE4);
    G::data(base).set_car_selection(0);
    G::data(base).set_car_scroll(0);
    OpenCarList(base, frame, calls);
    SetHeader(base, 0, 1, frame);
    Finish(calls, frame, 0x82184E1C, movie, selection + 37);
    Play(calls, frame, 0x82184E38, movie, 150);
    Store(base, step, 1);
  }
  return uint32_t(Run(calls, frame, 0x82184E4C, movie)) == 0 ? 1 : 0;
}

uint64_t Transition5(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82184E60 CarDealer::Transition5");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  if (Load(base, step) == 0) {
    Selection(base, calls, frame, 0x82184EAC);
    G::data(base).set_car_selection(0);
    G::data(base).set_car_scroll(0);
    {
      const Data data = G::data(base);
      data.set_shown_list(data.first_list_address());
    }
    G::data(base).set_list_shown(0);
    OpenCarList(base, frame, calls);
    SetHeader(base, 1, 1, frame);
    Play(calls, frame, 0x82184F04, movie, 159);
    Play(calls, frame, 0x82184F20, movie, 83);
    Loop(calls, frame, 0x82184F2C, movie, 83);
    Store(base, step, 1);
  }
  const bool done = uint32_t(Run(calls, frame, 0x82184F40, movie)) == 0;
  if (done) {
    const uint32_t lr[4] = {0x82184F68, 0x82184F74, 0x82184F90, 0x82184F9C};
    Rest(calls, frame, movie, lr);
  }
  return done ? 1 : 0;
}

uint64_t Transition8(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82184FB8 CarDealer::Transition8");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  if (Load(base, step) == 0) {
    const uint64_t selection = Selection(base, calls, frame, 0x82185000);
    Finish(calls, frame, 0x82185010, movie, selection + 24);
    Finish(calls, frame, 0x8218501C, movie, 34);
    Play(calls, frame, 0x82185038, movie, 153);
    {
      const Data data = G::data(base);
      data.set_kind_selection(data.list_shown());
    }
    OpenListMenu(base, frame, calls);
    SetHeader(base, 1, 0, frame);
    Store(base, step, 1);
  }
  if (int32_t(uint32_t(Run(calls, frame, 0x82185068, movie))) == 0) {
    // Done: transition 1 to page 0, none queued, the started word 0.
    const Pages pages = G::pages(base);
    const uint32_t page = pages.page();
    if (page != 0) {
      pages.set_previous_page(page);
    }
    pages.set_step(1);
    pages.set_queued_step(kNone);
    pages.set_next_page(0);
    pages.set_page(kNone);
    G::pages(base).set_step_started(0);
  }
  return 0;
}

uint64_t Transition9(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821850B8 CarDealer::Transition9");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  if (Load(base, step) == 0) {
    const uint64_t selection = Selection(base, calls, frame, 0x82185104);
    Finish(calls, frame, 0x82185114, movie, selection + 24);
    Finish(calls, frame, 0x82185120, movie, 34);
    Play(calls, frame, 0x8218513C, movie, 151);
    ReloadStandModel(base, frame, calls);
    OpenCar(base, frame, calls);
    Store(base, step, 1);
  }
  return uint32_t(Run(calls, frame, 0x82185158, movie)) == 0 ? 1 : 0;
}

uint64_t Transition1(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82181C38 CarDealer::Transition1");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  if (Load(base, step) == 0) {
    const uint32_t selection = uint32_t(Selection(base, calls, frame, 0x82181C7C));
    if (selection < 1) {
      if (List(base, G::data(base).first_list_address()).count() > 0) {
        Play(calls, frame, 0x82181CEC, movie, 37);
        Loop(calls, frame, 0x82181CF8, movie, 37);
      }
    } else if (selection == 1) {
      if (List(base, G::data(base).second_list_address()).count() > 0) {
        Play(calls, frame, 0x82181CB8, movie, 38);
        Loop(calls, frame, 0x82181CF8, movie, 38);
      }
    }
    // The two items colored 0, or 2 for an empty list.
    calls.Call(kSetNodeColor, frame, 0x82181D20, movie, 8, 5,
               List(base, G::data(base).first_list_address()).count() > 0 ? 0 : 2);
    calls.Call(kSetNodeColor, frame, 0x82181D48, movie, 8, 6,
               List(base, G::data(base).second_list_address()).count() > 0 ? 0 : 2);
    Store(base, step, 1);
  }
  Run(calls, frame, 0x82181D5C, movie);
  return 1;
}

// Transition6, Transition11 and Transition16: on the first call the
// animation `offset` + the list menu's selection played and looped, then
// the extra ones (Transition11: the data's +28 = 1, the stand drawn); then
// Run. Always 1. 128-byte frames.
namespace {
struct Extra {
  uint32_t id, play_lr, loop_lr;
};
uint64_t ShowSelected(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls,
                      const uint32_t lr[4], uint32_t offset, const Extra* extra,
                      uint32_t extras, bool draw_stand) {
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  if (Load(base, step) == 0) {
    const uint64_t id = Selection(base, calls, frame, lr[0]) + offset;
    Play(calls, frame, lr[1], movie, id);
    Loop(calls, frame, lr[2], movie, id);
    for (uint32_t k = 0; k < extras; k++) {
      Play(calls, frame, extra[k].play_lr, movie, extra[k].id);
      Loop(calls, frame, extra[k].loop_lr, movie, extra[k].id);
    }
    if (draw_stand) {
      G::data(base).set_car_shown(1);
    }
    Store(base, step, 1);
  }
  Run(calls, frame, lr[3], movie);
  return 1;
}
}  // namespace

uint64_t Transition6(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82181E10 CarDealer::Transition6");
  static constexpr uint32_t kLr[4] = {0x82181E54, 0x82181E78, 0x82181E84, 0x82181EC0};
  static constexpr Extra kExtra[1] = {{34, 0x82181EA0, 0x82181EAC}};
  return ShowSelected(base, step, sp, calls, kLr, 24, kExtra, 1, false);
}

uint64_t Transition11(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82181ED0 CarDealer::Transition11");
  static constexpr uint32_t kLr[4] = {0x82181F14, 0x82181F38, 0x82181F44, 0x82181FB4};
  static constexpr Extra kExtra[2] = {{86, 0x82181F60, 0x82181F6C}, {87, 0x82181F88, 0x82181F94}};
  return ShowSelected(base, step, sp, calls, kLr, 37, kExtra, 2, true);
}

uint64_t Transition16(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821820C0 CarDealer::Transition16");
  static constexpr uint32_t kLr[4] = {0x82182104, 0x82182128, 0x82182134, 0x82182148};
  return ShowSelected(base, step, sp, calls, kLr, 134, nullptr, 0, false);
}

uint64_t Transition19(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821841B0 CarDealer::Transition19");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  if (Load(base, step) == 0) {
    const uint64_t selection = Selection(base, calls, frame, 0x821841FC);
    Finish(calls, frame, 0x8218420C, movie, selection + 134);
    SetHeader(base, 2, 3, frame);
    Play(calls, frame, 0x82184234, movie, 156);
    Store(base, step, 1);
  }
  return uint32_t(Run(calls, frame, 0x82184248, movie)) == 0 ? 1 : 0;
}

uint64_t Transition7(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82183DE0 CarDealer::Transition7");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  if (Load(base, step) == 0) {
    const uint64_t selection = Selection(base, calls, frame, 0x82183E24);
    const uint64_t before =
        calls.Call(kListSelectionBefore, frame, 0x82183E30, G::list_menu(base).address);
    const MenuList list = G::list_menu(base);
    const uint32_t scroll = list.scroll();
    const int32_t moved = int32_t(scroll - list.scroll_before());
    Finish(calls, frame, 0x82183E50, movie, before + 24);
    Play(calls, frame, 0x82183E70, movie, selection + 24);
    Loop(calls, frame, 0x82183E7C, movie, selection + 24);
    if (moved != 0) {
      // The list scrolled: the cards refilled, the one coming in (after the
      // shown four, or before them) filled into (7, 9) and slid in.
      const uint32_t n = G::data(base).shown_list().count();
      const uint32_t card = ui_movies::FindNode(base, movie, 7, 9);
      calls.Call(kCarCardsFillAll, frame, 0x82183EAC);
      Finish(calls, frame, 0x82183EB8, movie, 29);
      Finish(calls, frame, 0x82183EC4, movie, 30);
      const uint32_t shown = G::data(base).shown_list().entries_address();
      uint32_t slide;
      if (moved < 0) {
        calls.Call(kCarCardFill, frame, 0x82183EE8, card, Load(base, ((scroll + 4) << 2) + shown));
        slide = 30;
      } else {
        calls.Call(kCarCardFill, frame, 0x82183F00, card, Load(base, shown + (scroll << 2) - 4));
        slide = 29;
      }
      Play(calls, frame, 0x82183F1C, movie, slide);
      SetScrollArrows(base, scroll, n, frame);
    }
    const uint32_t index = (scroll + uint32_t(selection)) << 2;
    const Entry entry(base, Load(base, index + G::data(base).shown_list().address));
    HelpLineSet(base, uint64_t(entry.car_id()) + 1862, frame, calls);
    Store(base, step, 1);
  }
  Run(calls, frame, 0x82183F5C, movie);
  return 1;
}

uint64_t Transition15(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82185310 CarDealer::Transition15");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  if (Load(base, step) == 0) {
    const uint64_t selection = Selection(base, calls, frame, 0x8218535C);
    Finish(calls, frame, 0x8218536C, movie, selection + 37);
    Finish(calls, frame, 0x82185378, movie, 86);
    Finish(calls, frame, 0x82185384, movie, 87);
    Play(calls, frame, 0x821853A0, movie, 152);
    G::data(base).set_color_selection(0);
    G::data(base).set_color_scroll(0);
    OpenColors(base, frame, calls);
    SetHeader(base, 1, 2, frame);
    Store(base, step, 1);
  }
  return uint32_t(Run(calls, frame, 0x821853DC, movie)) == 0 ? 1 : 0;
}

uint64_t Transition17(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82184018 CarDealer::Transition17");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  if (Load(base, step) == 0) {
    const uint64_t selection = Selection(base, calls, frame, 0x8218405C);
    const uint64_t before =
        calls.Call(kListSelectionBefore, frame, 0x82184068, G::list_menu(base).address);
    const MenuList list = G::list_menu(base);
    const uint32_t scroll = list.scroll();
    const int32_t moved = int32_t(scroll - list.scroll_before());
    G::data(base).set_color_selection(uint32_t(selection));
    G::data(base).set_color_scroll(scroll);
    Finish(calls, frame, 0x8218409C, movie, before + 134);
    Play(calls, frame, 0x821840BC, movie, selection + 134);
    Loop(calls, frame, 0x821840C8, movie, selection + 134);
    Finish(calls, frame, 0x821840D4, movie, 143);
    Finish(calls, frame, 0x821840E0, movie, 144);
    calls.Call(kShowCarByIdWithClass, frame, 0x821840E8, uint64_t(scroll) + selection);
    const MenuList menu = G::list_menu(base);
    if (menu.wrapped() == 0) {
      // The move's arrow by its sign (none when 0).
      const int32_t move = int32_t(menu.move());
      if (move > 0) {
        Play(calls, frame, 0x82184130, movie, 144);
      } else if (move < 0) {
        Play(calls, frame, 0x82184130, movie, 143);
      }
    } else {
      SetColorBar(base, uint64_t(uint32_t(selection) << 5) + 3, frame);
    }
    if (moved != 0) {
      ShowSwatches(base, frame);
      Finish(calls, frame, 0x82184158, movie, 29);
      Finish(calls, frame, 0x82184164, movie, 30);
      Play(calls, frame, 0x8218418C, movie, moved < 0 ? 146 : 145);
    }
    Store(base, step, 1);
  }
  Run(calls, frame, 0x821841A0, movie);
  return 1;
}

}  // namespace car_dealer
