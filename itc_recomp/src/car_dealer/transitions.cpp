// The car dealer's page transitions (docs/functions/car-dealer.md): see
// car_dealer.h. Each is called by CarDealer::Update with the page object's
// +24 (the started word) and answers whether it's done.

#include "car_dealer/car_dealer.h"

#include "base/log.h"
#include "car_dealer/internal.h"
#include "guest.h"
#include "ui_movies/game.h"

namespace car_dealer {

namespace {

// The screen fades' counts (mode-tick.md).
constexpr uint32_t kFadeOutCount = 0x826A50E4;
constexpr uint32_t kFadeInCount = 0x826A50DC;
// ScreenFades::FadeOut's layer word.
constexpr uint32_t kFadeOutLayer = 0x82001BF0;
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
  return calls.Call(kListSelection, frame, lr, Load(base, kListMenu));
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
    calls.Call(kFadeOut, frame, 0x82181DC4, Load(base, kFadeOutLayer), 0, 0, 0);
    Store(base, step, 1);
  }
  calls.Call(kRun, frame, 0x82181DD8, movie, 0);
  return (Load(base, kFadeOutCount) | Load(base, kFadeInCount)) == 0 ? 1 : 0;
}

uint64_t Transition0(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82184C70 CarDealer::Transition0");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  if (Load(base, step) == 0) {
    Store(base, Load(base, kData) + 40, 0);
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
    Store(base, Load(base, kData) + 48, 0);
    Store(base, Load(base, kData) + 52, 0);
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
    Store(base, Load(base, kData) + 48, 0);
    Store(base, Load(base, kData) + 52, 0);
    {
      const uint32_t data = Load(base, kData);
      Store(base, data + 1136, data + 104);
    }
    Store(base, Load(base, kData) + 44, 0);
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
      const uint32_t data = Load(base, kData);
      Store(base, data + 40, Load(base, data + 44));
    }
    OpenListMenu(base, frame, calls);
    SetHeader(base, 1, 0, frame);
    Store(base, step, 1);
  }
  if (int32_t(uint32_t(Run(calls, frame, 0x82185068, movie))) == 0) {
    // Done: transition 1 to page 0, none queued, the started word 0.
    const uint32_t pages = Load(base, kPages);
    const uint32_t page = Load(base, pages + 0);
    if (page != 0) {
      Store(base, pages + 8, page);
    }
    Store(base, pages + 12, 1);
    Store(base, pages + 16, kNone);
    Store(base, pages + 4, 0);
    Store(base, pages + 0, kNone);
    Store(base, Load(base, kPages) + 24, 0);
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
      if (Load(base, Load(base, kData) + 616) > 0) {
        Play(calls, frame, 0x82181CEC, movie, 37);
        Loop(calls, frame, 0x82181CF8, movie, 37);
      }
    } else if (selection == 1) {
      if (Load(base, Load(base, kData) + 1132) > 0) {
        Play(calls, frame, 0x82181CB8, movie, 38);
        Loop(calls, frame, 0x82181CF8, movie, 38);
      }
    }
    // The two items colored 0, or 2 for an empty list.
    calls.Call(kSetNodeColor, frame, 0x82181D20, movie, 8, 5,
               Load(base, Load(base, kData) + 616) > 0 ? 0 : 2);
    calls.Call(kSetNodeColor, frame, 0x82181D48, movie, 8, 6,
               Load(base, Load(base, kData) + 1132) > 0 ? 0 : 2);
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
      Store(base, Load(base, kData) + 28, 1);
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
        calls.Call(kListSelectionBefore, frame, 0x82183E30, Load(base, kListMenu));
    const uint32_t list = Load(base, kListMenu);
    const uint32_t scroll = Load(base, list + 4);
    const int32_t moved = int32_t(scroll - Load(base, list + 8));
    Finish(calls, frame, 0x82183E50, movie, before + 24);
    Play(calls, frame, 0x82183E70, movie, selection + 24);
    Loop(calls, frame, 0x82183E7C, movie, selection + 24);
    if (moved != 0) {
      // The list scrolled: the cards refilled, the one coming in (after the
      // shown four, or before them) filled into (7, 9) and slid in.
      const uint32_t n = Load(base, Load(base, Load(base, kData) + 1136) + 512);
      const uint32_t card = ui_movies::FindNode(base, movie, 7, 9);
      calls.Call(kCarCardsFillAll, frame, 0x82183EAC);
      Finish(calls, frame, 0x82183EB8, movie, 29);
      Finish(calls, frame, 0x82183EC4, movie, 30);
      const uint32_t shown = Load(base, Load(base, kData) + 1136);
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
    const uint32_t entry = Load(base, index + Load(base, Load(base, kData) + 1136));
    HelpLineSet(base, uint64_t(Load(base, entry + 8432)) + 1862, frame, calls);
    Store(base, step, 1);
  }
  Run(calls, frame, 0x82183F5C, movie);
  return 1;
}

}  // namespace car_dealer
