// The location screen's transitions that move the course view's camera
// (Transition4 and Transition5: the location's camera animation evaluated
// into the screen's view), the transitions back to the location's menu
// (Transition8, Transition24, Transition34) and the screen's start
// (docs/functions/location-screen.md). The text setters through their plain
// C++ (the adapters' check: hooks::LocationScreenTextsNative), the stops and
// runs through `calls`.

#include "location_screen/location_screen.h"

#include "base/log.h"
#include "camera_modes/camera_modes.h"
#include "car_specs/car_specs.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "location_screen/internal.h"
#include "mode_tick/mode_tick.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace location_screen {

using namespace detail;

namespace {

// The location's first camera animation (0x82636518 + 16 x the location)
// evaluated at frame `time` into the frame's +128 (CameraAnims::Evaluate,
// its frame 144 bytes below `frame`; EvalCamera's original through `calls`
// where its plain C++ can't), after the original's set-up: two vectors of
// (0, 0, 0, 1) at +96 and +112 copied to +128 and +144 (their w words
// stored again first), +160 and +164 0.0. Then the screen's +592 = the
// frame's +144 (16 bytes), +608 = +128, +664 and +668 = +160 and +164
// (through a float). The floats go through a float register as lfs and
// stfs leave them.
void EvaluateView(uint8_t* base, uint32_t frame, uint32_t time, const Calls& calls) {
  const uint32_t zero = ThroughFloat(base, 0x82000A34);
  const uint32_t one = ThroughFloat(base, 0x82000A30);
  for (uint32_t v : {96u, 112u}) {
    guest::StoreU32(base, frame + v + 0, zero);
    guest::StoreU32(base, frame + v + 4, zero);
    guest::StoreU32(base, frame + v + 8, zero);
    guest::StoreU32(base, frame + v + 12, one);
  }
  guest::StoreU32(base, frame + 140, one);
  guest::StoreU32(base, frame + 156, one);
  for (uint32_t i = 0; i < 32; i += 4) {
    guest::StoreU32(base, frame + 128 + i, guest::LoadU32(base, frame + 96 + i));
  }
  guest::StoreU32(base, frame + 160, zero);
  guest::StoreU32(base, frame + 164, zero);
  const uint32_t location = guest::LoadU32(base, Screen(base) + 852);
  const uint32_t id = guest::LoadU32(base, ((location << 4) & 0xFFFFFFF0u) + 0x82636518);
  const uint32_t out = frame + 128;
  std::optional<double> f1;
  camera_modes::AnimsEvaluate(
      base, out, id, time,
      [&](uint32_t object) {
        calls.Call(0x82150350, frame - 144, 0x821509C4, object, object, out, time);
      },
      &f1);
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(base, Screen(base) + 592 + i, guest::LoadU32(base, frame + 144 + i));
  }
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(base, Screen(base) + 608 + i, guest::LoadU32(base, frame + 128 + i));
  }
  const uint32_t screen = Screen(base);
  guest::StoreU32(base, screen + 664, ThroughFloat(base, frame + 160));
  guest::StoreU32(base, screen + 668, ThroughFloat(base, frame + 164));
}

}  // namespace

uint32_t Transition4(uint8_t* base, uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222D828 LocationScreen::Transition4");
  const uint32_t frame = sp - 208;
  guest::StoreU32(base, frame, sp);
  const uint32_t movie = Movie(base, frame, kMenusMovie);
  if (guest::LoadU32(base, step) == 0) {
    ui_movies::ResetPose(base, movie);
    calls.Call(kFinishAll, frame, 0x8222D870, movie);
    ShowPictures(base, frame);
    ui_movies::Play(base, movie, 0xFFFFFFFFu, 1, 0, 0, 0, 0);
    PlayNamed(base, movie, 103);
    LoopLast(base, frame, movie);
    ShowListEntry(base, frame, calls);
    PlayNamed(base, movie, guest::LoadU32(base, guest::LoadU32(base, kListMenu)) + 52);
    LoopLast(base, frame, movie);
    SetText(base, calls, frame, movie, 0, 0, 578);
    Required(ui_movies::ResetTextScroll(base, movie, 0, 0), "sub_8211C5F8");
    EvaluateView(base, frame, 1, calls);
    guest::StoreU32(base, step, 1);
  }
  return uint32_t(Run(calls, frame, 0x8222DA00, movie)) == 0 ? 1 : 0;
}

uint32_t Transition5(uint8_t* base, uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222DA10 LocationScreen::Transition5");
  const uint32_t frame = sp - 224;
  guest::StoreU32(base, frame, sp);
  const uint32_t movie = Movie(base, frame, kMenusMovie);
  if (guest::LoadU32(base, step) == 0) {
    ResetMovie(base, frame, calls);
    HidePictures(base, frame);
    SetText(base, calls, frame, movie, 0, 0, 573);
    Required(ui_movies::ResetTextScroll(base, movie, 0, 0), "sub_8211C5F8");
    ui_movies::Play(base, movie, 0xFFFFFFFFu, 0, 0, 0, 0, 0);
    EvaluateView(base, frame, 0, calls);
    guest::StoreU32(base, step, 1);
  }
  const uint32_t done = uint32_t(Run(calls, frame, 0x8222DB84, movie)) == 0 ? 1 : 0;
  if (done != 0) {
    ListNode(base, frame, 0);
    // The list's menu reset: its count (the location record's, +812) less 1
    // at +16, +20 = 1, the rest 0 (the original's store order).
    const uint32_t count = guest::LoadU32(base, Screen(base) + 812) - 1;
    const uint32_t menu = guest::LoadU32(base, kListMenu);
    guest::StoreU32(base, menu + 12, 0);
    guest::StoreU32(base, menu + 20, 1);
    for (uint32_t offset : {0u, 4u, 8u, 40u, 48u}) {
      guest::StoreU32(base, menu + offset, 0);
    }
    guest::StoreU32(base, menu + 16, count);
    for (uint32_t offset : {52u, 36u, 64u, 24u, 28u, 32u}) {
      guest::StoreU32(base, menu + offset, 0);
    }
    FinishNamed(calls, frame, 0x8222DBF4, movie, 103);
    SetCarColor(base, frame);
  }
  return done;
}

uint32_t Transition8(uint8_t* base, uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222DC08 LocationScreen::Transition8");
  const uint32_t frame = sp - 112;
  guest::StoreU32(base, frame, sp);
  const uint32_t movie = Movie(base, frame, kMenusMovie);
  if (guest::LoadU32(base, step) == 0) {
    ui_movies::ResetNode(base, movie, 0xFFFFFFFFu, 11);
    ShowListEntry(base, frame, calls);
    ui_movies::Play(base, movie, 0xFFFFFFFFu, 10, 0, 0, 0, 0);
    PlayNamed(base, movie, guest::LoadU32(base, guest::LoadU32(base, kListMenu)) + 52);
    LoopLast(base, frame, movie);
    ShowPictures(base, frame);
    guest::StoreU32(base, step, 1);
  }
  return uint32_t(Run(calls, frame, 0x8222DCC4, movie)) == 0 ? 1 : 0;
}

uint32_t Transition24(uint8_t* base, uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222D2E8 LocationScreen::Transition24");
  const uint32_t frame = sp - 112;
  guest::StoreU32(base, frame, sp);
  const uint32_t movie = Movie(base, frame, kMenusMovie);
  if (guest::LoadU32(base, step) == 0) {
    mode_tick::ScreenFadesFadeOut(base, 60, 0, 0, 0, 0);
    PlayNamed(base, movie, 98);
    guest::StoreU32(base, step, 1);
  }
  const uint32_t done = Fading(base) ? 0 : 1;
  Run(calls, frame, 0x8222D38C, movie);
  if (done != 0) {
    ResetMovie(base, frame, calls);
    SetCarColor(base, frame);
  }
  return done;
}

uint32_t Transition34(uint8_t* base, uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222D108 LocationScreen::Transition34");
  const uint32_t frame = sp - 112;
  guest::StoreU32(base, frame, sp);
  const uint32_t movie = Movie(base, frame, kMenusMovie);
  if (guest::LoadU32(base, step) == 0) {
    PlayNamed(base, movie, 98);
    guest::StoreU32(base, step, 1);
  }
  const uint32_t done = uint32_t(Run(calls, frame, 0x8222D178, movie)) == 0 ? 1 : 0;
  if (done != 0) {
    ResetMovie(base, frame, calls);
  }
  return done;
}

uint32_t Start(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222F0B0 LocationScreen::Start");
  const uint32_t frame = sp - 128;
  guest::StoreU32(base, frame, sp);
  // The garage's selected car: its list walked from the halfword at +6324
  // (the entries 4 bytes each from +6120, each a next and a slot) by the
  // count at +0x19920; the slot's car (1,968 bytes each from +6120) +224
  // the spec's id.
  uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t garage = save + 6120;
  uint32_t count = guest::LoadU32(base, save + 0x19920);
  uint32_t entry =
      (uint32_t(int32_t(int16_t(guest::LoadU16(base, garage + 204)))) << 2) + garage;
  for (; count != 0; count--) {
    entry = (uint32_t(int32_t(int16_t(guest::LoadU16(base, entry)))) << 2) + garage;
  }
  const uint32_t slot = uint32_t(int32_t(int16_t(guest::LoadU16(base, entry + 2))));
  const uint32_t spec = car_specs::GetCarSpec(base, guest::LoadU32(base, slot * 1968 + garage + 224));
  guest::StoreU32(base, Screen(base) + 892, guest::LoadU8(base, spec + 771) == 1 ? 1 : 0);
  ResetMovie(base, frame, calls);
  save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t unlocks = save + 0x1C778;
  const uint32_t race = guest::LoadU32(base, kRaceData);
  const uint32_t record = vehicle_accessors::RacerRecordsAt(base, race + kRacerRecords, 0xFFFFFFFFu);
  const uint32_t pages = guest::LoadU32(base, kPages);
  uint32_t page, transition;
  if (vehicle_accessors::StagesWord808(base, race + 0x59620) != 0 && record != 0 &&
      guest::LoadU32(base, Screen(base) + 816) == 0) {
    // After a race: page 12 through transition 23.
    data_tables::Item(base, guest::LoadU32(base, record + 980));
    guest::StoreU32(base, Screen(base) + 872, 0);
    guest::StoreU32(base, Screen(base) + 876, 0);
    page = 12;
    transition = 23;
  } else if (guest::LoadU32(base, unlocks + 768) != 0) {
    // An unlock to show: page 13 through transition 25.
    page = 13;
    transition = 25;
  } else {
    // Page 0, set directly.
    const uint32_t current = guest::LoadU32(base, pages);
    if (current != 0) {
      guest::StoreU32(base, pages + 8, current);
    }
    guest::StoreU32(base, pages + 12, 0);
    guest::StoreU32(base, pages + 4, 0);
    guest::StoreU32(base, pages + 16, 0xFFFFFFFFu);
    guest::StoreU32(base, pages + 0, 0xFFFFFFFFu);
    mode_tick::ScreenFadesFadeIn(base, 30, 0, 0, 0, 0);
    return 30;
  }
  const uint32_t current = guest::LoadU32(base, pages);
  if (current != page) {
    guest::StoreU32(base, pages + 8, current);
  }
  guest::StoreU32(base, pages + 0, 0xFFFFFFFFu);
  guest::StoreU32(base, pages + 4, page);
  guest::StoreU32(base, pages + 16, 0xFFFFFFFFu);
  guest::StoreU32(base, pages + 12, transition);
  mode_tick::ScreenFadesFadeIn(base, 30, 0, 0, 0, 0);
  return 30;
}

}  // namespace location_screen
