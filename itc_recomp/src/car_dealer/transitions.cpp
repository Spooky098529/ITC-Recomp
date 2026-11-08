// The car dealer's page transitions (docs/functions/car-dealer.md): see
// car_dealer.h. Each is called by CarDealer::Update with the page object's
// +24 (the started word) and answers whether it's done.

#include "car_dealer/car_dealer.h"

#include "base/log.h"
#include "car_dealer/internal.h"
#include "guest.h"

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

}  // namespace car_dealer
