// The car dealer's update, start and the dialogs' end
// (docs/functions/car-dealer.md): see car_dealer.h.

#include "car_dealer/car_dealer.h"

#include "base/log.h"
#include "car_dealer/internal.h"
#include "guest.h"
#include "schema/race_menu.h"

namespace car_dealer {

namespace {

// The transitions' handlers, in CarDealer::Update's table (its frame's +96
// on).
constexpr uint32_t kTransitions[22] = {
    0x82184C70, 0x82181C38, 0x82183BD8, 0x82181D68, 0x82184DA0, 0x82184E60,
    0x82181E10, 0x82183DE0, 0x82184FB8, 0x821850B8, 0x82185178, 0x82181ED0,
    0x82181FC0, 0x82183F68, 0x82185220, 0x82185310, 0x821820C0, 0x82184018,
    0x82185400, 0x821841B0, 0x82185518, 0x82184268};

// The named animation calls (r5 on 0).
uint64_t Play(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint32_t id) {
  return calls.Call(kPlayNamed, frame, lr, movie, id, 0, 0);
}

}  // namespace

uint64_t ShowBusy(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82181558 CarDealer::ShowBusy");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  Play(calls, frame, 0x821815A0, movie, 80);
  Play(calls, frame, 0x821815BC, movie, 79);
  return calls.Call(kLoopLast, frame, 0x821815C4, movie);
}

uint64_t HideBusy(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821815D8 CarDealer::HideBusy");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  Play(calls, frame, 0x82181620, movie, 81);
  return calls.Call(kFinishNamed, frame, 0x8218162C, movie, 79);
}

uint64_t HideBusyNow(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82181640 CarDealer::HideBusyNow");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  Play(calls, frame, 0x82181688, movie, 81);
  calls.Call(kFinishNamed, frame, 0x82181694, movie, 81);
  return calls.Call(kFinishNamed, frame, 0x821816A0, movie, 79);
}

uint64_t Update(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82185600 CarDealer::Update");
  const uint32_t frame = Frame(base, sp, 320);
  calls.Call(kEmpty, frame, 0x82185624, 100, 30, 0, 0x82009F10);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  Pages pages = G::pages(base);
  const uint32_t running = pages.step();
  for (uint32_t k = 0; k < 22; k++) {
    Store(base, frame + 96 + 4 * k, kTransitions[k]);
  }
  uint64_t r3 = movie;
  if (running == 0xFFFFFFFFu) {
    if (pages.page() != 4) {
      r3 = calls.Call(kRun, frame, 0x821857F4, movie, 0);
      pages = G::pages(base);
    }
  } else {
    bool start_next = true;
    if (int32_t(running) >= 0) {
      const uint32_t handler = Load(base, (running << 2) + frame + 96);
      const uint64_t args[8] = {pages.step_started_address(), kTransitions[15], kTransitions[16],
                                kTransitions[17], kTransitions[18], kTransitions[19],
                                kTransitions[20], uint64_t(running << 2)};
      r3 = calls.CallArgs(handler, frame, 0x82185780, args);
      pages = G::pages(base);
      start_next = int32_t(uint32_t(r3)) != 0;
    }
    if (start_next) {
      const uint32_t queued = pages.queued_step();
      if (queued != 0xFFFFFFFFu) {
        pages.set_queued_step(0xFFFFFFFFu);
        pages.set_unknown_20(0);
        pages.set_step(queued);
      } else if (int32_t(running) >= 0) {
        pages.set_page(pages.next_page());
        pages.set_step(queued);
      } else {
        const uint32_t page = pages.next_page();
        pages.set_step(0xFFFFFFFFu);
        pages.set_page(page);
      }
      pages.set_step_started(0);
      pages = G::pages(base);
    }
  }
  Data data = G::data(base);
  if (data.car_shown() == 0 || pages.page() == 4) {
    if (data.busy_shown() != 0) {
      r3 = HideBusyNow(base, frame, calls);
      G::data(base).set_busy_shown(0);
    }
    return r3;
  }
  r3 = calls.Call(kStageIsBusy, frame, 0x82185828, Stage(base));
  data = G::data(base);
  const uint32_t shown = data.busy_shown();
  if (int32_t(uint32_t(r3)) != 0) {
    if (shown == 0) {
      r3 = ShowBusy(base, frame, calls);
      G::data(base).set_busy_shown(1);
      data = G::data(base);
    }
  } else if (shown != 0) {
    r3 = HideBusy(base, frame, calls);
    G::data(base).set_busy_shown(0);
    data = G::data(base);
  }
  if (data.stand_model() != 0) {
    return calls.Call(kStageDraw, frame, 0x821858F8, Stage(base), 0, 1);
  }
  const uint32_t id = SelectedEntry(base).car_id();
  const uint64_t block = calls.Call(kCarModelsBlockOf, frame, 0x82185894, id);
  r3 = calls.Call(kStageReloadModel, frame, 0x821858A4, Stage(base), block);
  G::data(base).set_stand_model(uint32_t(r3));
  if (G::data(base).stand_model() == 0) {
    return r3;
  }
  calls.Call(kStageCommand, frame, 0x821858CC, Stage(base), 55);
  data = G::data(base);
  return calls.Call(kShowCarById, frame, 0x821858E0,
                    uint64_t(data.color_scroll()) + data.color_selection());
}

uint64_t Start(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82184B80 CarDealer::Start");
  const uint32_t frame = Frame(base, sp, 96);
  BuildLists(base, frame, calls);
  FillStatus(base, frame, calls);
  const bool first = G::data(base).first_car() != 0;
  const Pages pages = G::pages(base);
  const uint32_t page = pages.page();
  uint32_t running, queued;
  if (first) {
    if (page != 1) {
      pages.set_previous_page(page);
    }
    pages.set_next_page(1);
    pages.set_page(0xFFFFFFFFu);
    running = 5;
    queued = 6;
  } else {
    if (page != 0) {
      pages.set_previous_page(page);
    }
    pages.set_next_page(0);
    pages.set_page(0xFFFFFFFFu);
    running = 0;
    queued = 1;
  }
  pages.set_queued_step(queued);
  pages.set_step(running);
  calls.Call(kStageSetDrawWord, frame, 0x82184C1C, Stage(base), 0);
  calls.Call(kStageDraw, frame, 0x82184C30, Stage(base), 0, 1);
  calls.Call(kStageSetDrawWord, frame, 0x82184C40, Stage(base), 1);
  return calls.Call(kFadeIn, frame, 0x82184C5C, schema::RaceMenuGlobals::fade_in_passes(base), 0, 0,
                    0);
}

uint64_t OnDialogDone(uint8_t* base, uint64_t r3, uint32_t mode, uint32_t sp,
                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82183160 CarDealer::OnDialogDone");
  const uint32_t frame = Frame(base, sp, 112);
  const Data data = G::data(base);
  const uint32_t step = data.buy_step();
  // Step 3 (and step 2's yes with an empty garage): transition 3 to page 4,
  // message 32, the purchase.
  auto buy = [&]() {
    const Pages pages = G::pages(base);
    const uint32_t page = pages.page();
    if (page != 4) {
      pages.set_previous_page(page);
    }
    pages.set_step(3);
    pages.set_queued_step(0xFFFFFFFFu);
    pages.set_next_page(4);
    pages.set_page(0xFFFFFFFFu);
    Post(base, mode, 32);
    return Buy(base, frame, calls);
  };
  switch (step) {
    case 0: {
      const uint32_t answer = data.answer();
      const Pages pages = G::pages(base);
      const uint32_t page = pages.page();
      if (answer != 0) {
        if (page != 5) {
          pages.set_previous_page(page);
        }
        pages.set_queued_step(0xFFFFFFFFu);
        pages.set_step(3);
        pages.set_next_page(5);
        pages.set_page(0xFFFFFFFFu);
        G::data(base).set_leaving_bought(1);
      } else {
        if (page != 1) {
          pages.set_previous_page(page);
        }
        pages.set_step(0xFFFFFFFFu);
        pages.set_page(1);
        pages.set_queued_step(0xFFFFFFFFu);
      }
      return r3;
    }
    case 1: {
      const Pages pages = G::pages(base);
      const uint32_t page = pages.page();
      if (page != 1) {
        pages.set_previous_page(page);
      }
      pages.set_next_page(1);
      pages.set_queued_step(0xFFFFFFFFu);
      pages.set_step(14);
      pages.set_page(0xFFFFFFFFu);
      const uint32_t prompts[6] = {0, 0, 1, 1, 0xFFFFFFFFu, 0xFFFFFFFFu};
      r3 = SetPrompts(base, prompts, frame);
      G::data(base).set_car_shown(0);
      return r3;
    }
    case 2: {
      if (data.answer() != 0) {
        if (data.first_car() != 0) {
          return buy();
        }
        r3 = calls.Call(kShowDriveDialog, frame, 0x82183258);
        Post(base, mode, 34);
        G::data(base).set_buy_step(3);
        return r3;
      }
      Post(base, mode, 32);
      const Pages pages = G::pages(base);
      const uint32_t page = pages.page();
      if (page != 2) {
        pages.set_previous_page(page);
      }
      pages.set_step(10);
      pages.set_queued_step(11);
      pages.set_next_page(2);
      pages.set_page(0xFFFFFFFFu);
      return r3;
    }
    case 3:
      data.set_select_bought(data.answer() != 0 ? 1 : 0);
      return buy();
    default:
      return r3;
  }
}

}  // namespace car_dealer
