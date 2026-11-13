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
#include "course_scenery/course_scenery.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "location_screen/internal.h"
#include "mode_tick/mode_tick.h"
#include "race_event/race_event.h"
#include "race_setup/race_setup.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace location_screen {

using namespace detail;

namespace {

// The location's camera animation `which` (0x82636518 + 16 x the location +
// 4 x which) evaluated at frame `time` into the frame's +128 (CameraAnims::Evaluate,
// its frame 144 bytes below `frame`; EvalCamera's original through `calls`
// where its plain C++ can't), after the original's set-up: two vectors of
// (0, 0, 0, 1) at +96 and +112 copied to +128 and +144 (their w words
// stored again first), +160 and +164 0.0. Then the screen's +592 = the
// frame's +144 (16 bytes), +608 = +128, +664 and +668 = +160 and +164
// (through a float). The floats go through a float register as lfs and
// stfs leave them.
uint32_t EvaluateView(uint8_t* base, uint32_t frame, uint32_t which, uint32_t time,
                      const Calls& calls) {
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
  const uint32_t id =
      guest::LoadU32(base, ((location << 4) & 0xFFFFFFF0u) + 0x82636518 + 4 * which);
  const uint32_t out = frame + 128;
  std::optional<double> f1;
  const uint32_t evaluated = camera_modes::AnimsEvaluate(
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
  return evaluated;
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
    EvaluateView(base, frame, 0, 1, calls);
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
    EvaluateView(base, frame, 0, 0, calls);
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

uint32_t ShowOpponentSpecs(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222C188 LocationScreen::ShowOpponentSpecs");
  const uint32_t frame = sp - kShowOpponentSpecsFrame;
  guest::StoreU32(base, frame, sp);
  const uint32_t movie = Movie(base, frame, kMenusMovie);
  guest::StoreU32(base, frame + 80, guest::LoadU32(base, Screen(base) + 800));
  const uint32_t event = race_setup::EventCurrent(base);
  const uint32_t item = race_event::GroupItem(base, event, guest::LoadU32(base, Screen(base) + 804),
                                              guest::LoadU32(base, frame + 80));
  Copy(base, calls, frame, 0x8222C1E4, frame + 96, item, 3008);
  // The item's car build's car id (+160 + 2788).
  const uint32_t id = guest::LoadU32(base, frame + 3044);
  const uint32_t spec = car_specs::GetCarSpec(base, id);
  const uint32_t n9 = ui_movies::FindNode(base, movie, 12, 9);
  const uint32_t n10 = ui_movies::FindNode(base, movie, 12, 10);
  const uint32_t n11 = ui_movies::FindNode(base, movie, 12, 11);
  SetNodeText(base, calls, frame, movie, n9, guest::LoadU8(base, spec + 769) + 1934);
  SetNodeText(base, calls, frame, movie, n10, id + 1646);
  return SetNodeText(base, calls, frame, movie, n11, id + 1718);
}

uint32_t Transition9(uint8_t* base, uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222DEE0 LocationScreen::Transition9");
  const uint32_t frame = sp - 240;
  guest::StoreU32(base, frame, sp);
  const uint32_t movie = Movie(base, frame, kMenusMovie);
  if (guest::LoadU32(base, step) == 0) {
    ResetMovie(base, frame, calls);
    if (guest::LoadU32(base, Screen(base) + 888) != 0) {
      HidePictures(base, frame);
    } else {
      const uint32_t menu = guest::LoadU32(base, kOpponentMenu);
      const uint32_t cursor = guest::LoadU32(base, menu);
      guest::StoreU32(base, menu + 0, 0);
      guest::StoreU32(base, menu + 4, cursor);
    }
    for (uint32_t i = 0; i < 10; i++) {
      FinishNamed(calls, frame, 0x8222DF60, movie, 13 + i);
    }
    for (uint32_t i = 0; i < guest::LoadU32(base, Screen(base) + 808); i++) {
      FinishNamed(calls, frame, 0x8222DF8C, movie, 84 + i);
    }
    HideNode(base, movie, 0xFFFFFFFFu, 18);
    HideNode(base, movie, 0xFFFFFFFFu, 17);
    ui_movies::Play(base, movie, 0xFFFFFFFFu, 2, 0, 0, 0, 0);
    PlayNamed(base, movie, guest::LoadU32(base, Screen(base) + 800) + 84);
    LoopLast(base, frame, movie);
    ShowOpponentInfo(base, frame, calls);
    // The location's second camera animation at the picked opponent's time
    // (0x82010780 + 4 x +800).
    const uint32_t picked = guest::LoadU32(base, Screen(base) + 800);
    EvaluateView(base, frame, 1, guest::LoadU32(base, ((picked << 2) & 0xFFFFFFFCu) + 0x82010780),
                 calls);
    ShowOpponentSpecs(base, frame, calls);
    ShowOpponentPictures(base, frame, calls);
    guest::StoreU32(base, step, 1);
  }
  const uint32_t done = uint32_t(Run(calls, frame, 0x8222E118, movie)) == 0 ? 1 : 0;
  if (done != 0) {
    ShowArrows(base, frame);
    ui_movies::Play(base, movie, 0xFFFFFFFFu, 13, 0, 0, 0, 0);
    LoopLast(base, frame, movie);
    // The opponents' menu reset: 6 items (+16 = 6), the rest 0 (the
    // original's store order).
    const uint32_t menu = guest::LoadU32(base, kOpponentMenu);
    guest::StoreU32(base, menu + 12, 0);
    guest::StoreU32(base, menu + 16, 6);
    for (uint32_t offset : {20u, 0u, 4u, 8u, 40u, 48u, 52u, 36u, 64u, 24u, 28u, 32u}) {
      guest::StoreU32(base, menu + offset, 0);
    }
    SetCarColor(base, frame);
  }
  return done;
}

namespace {

// The course scenery's calls (course_scenery::Calls) through `calls`.
course_scenery::Calls SceneryCalls(const Calls& calls) {
  return {const_cast<Calls*>(&calls),
          [](void* user, uint32_t function, const uint32_t* args, int count, uint32_t frame,
             uint32_t lr) {
            const Calls& c = *static_cast<const Calls*>(user);
            uint64_t registers[4] = {};
            for (int i = 0; i < count && i < 4; i++) {
              registers[i] = args[i];
            }
            return uint32_t(c.call(c.user, function, registers, frame, lr));
          }};
}

// The location's camera animation `which`'s id (0x82636518 + 16 x the
// location + 4 x which).
uint32_t AnimId(uint8_t* base, uint32_t which) {
  const uint32_t location = guest::LoadU32(base, Screen(base) + 852);
  return guest::LoadU32(base, ((location << 4) & 0xFFFFFFF0u) + 0x82636518 + 4 * which);
}

constexpr uint32_t kCarScene = 0x826C2E44;
constexpr uint32_t kCarSceneUpdate = 0x820C9608;
constexpr uint32_t kCarSceneDraw = 0x820C5DE8;
constexpr uint32_t kSceneModelPollLoad = 0x820DA318;
constexpr uint32_t kSceneryIsLoaded = 0x820F1F68;

}  // namespace

uint64_t PollLoad(uint8_t* base, uint32_t sp, uint32_t machine, const Calls& calls,
                  bool native_scenery, bool native_queue) {
  ITC_NATIVE_RUNNING("sub_82229238 LocationScreen::PollLoad");
  const uint32_t frame = sp - 336;
  guest::StoreU32(base, frame, sp);
  if (camera_modes::AnimsFind(base, AnimId(base, 0)) == 0) {
    return 0;
  }
  const uint32_t evaluated = EvaluateView(base, frame, 0, 0, calls);
  const uint32_t scene = guest::LoadU32(base, kCarScene);
  calls.Call(kCarSceneUpdate, frame, 0x82229350, evaluated);
  uint64_t r3 = calls.Call(kCarSceneDraw, frame, 0x8222935C, scene, 0);
  for (uint32_t i = 0; i < guest::LoadU32(base, Screen(base) + 808); i++) {
    const uint32_t index = guest::LoadU32(base, Screen(base) + 824 + 4 * i);
    const uint32_t model = vehicle_accessors::ScenePoolsGetB(base, scene, 7, index);
    if (camera_modes::AnimsFind(base, AnimId(base, 2)) == 0) {
      return 0;
    }
    // The identity the original builds in vector registers and stores
    // (stvx: each row's address rounded down to 16), then copies to the
    // model's matrix (+4304) from the unrounded address.
    for (uint32_t row = 0; row < 4; row++) {
      const uint32_t at = (frame + 176 + 16 * row) & ~15u;
      for (uint32_t column = 0; column < 4; column++) {
        guest::StoreU32(base, at + 4 * column, row == column ? 0x3F800000u : 0);
      }
    }
    const uint32_t matrix = model + 4304;
    Copy(base, calls, frame, 0x82229404, matrix, frame + 176, 64);
    // The third animation's object at the opponent's time (0x82010780 + 4i).
    const uint32_t time = guest::LoadU32(base, 0x82010780 + 4 * i);
    if ((frame & 0xF) != 0) {
      calls.Call(0x82150A10, frame, 0x82229424, matrix, AnimId(base, 2), time);
    } else {
      const uint32_t inner = frame - 96 - 320;
      camera_modes::AnimsObjectMatrix(base, matrix, AnimId(base, 2), time, inner,
                                      [&](uint32_t to, uint32_t from, uint32_t size) {
                                        calls.Call(kMemcpy, inner, 0x82150600, to, from, size);
                                      });
    }
    if (!PoseCar(base, frame, model + 4288, i, calls)) {
      calls.Call(0x82228588, frame, 0x82229430, model + 4288, i);
    }
    r3 = calls.Call(kSceneModelPollLoad, frame, 0x82229438, model);
    if (uint32_t(r3) == 0) {
      return r3;
    }
  }
  // Every picture of list A loaded (its +0).
  const uint32_t screen = Screen(base);
  for (uint32_t i = 0; i < guest::LoadU32(base, screen + 136); i++) {
    if (guest::LoadU32(base, guest::LoadU32(base, screen + 8 + 4 * i)) == 0) {
      return r3;
    }
  }
  uint32_t movies = 0;
  r3 = ui_movies::IsLoaded(base, 21);
  if (uint32_t(r3) != 0) {
    r3 = ui_movies::IsLoaded(base, 30);
    if (uint32_t(r3) != 0) {
      movies = 1;
    }
  }
  const uint32_t manager = guest::LoadU32(base, Screen(base) + 552);
  if (native_scenery) {
    r3 = course_scenery::SceneryIsLoaded(base, SceneryCalls(calls), frame, manager, native_queue);
  } else {
    r3 = calls.Call(kSceneryIsLoaded, frame, 0x822294C0, manager);
  }
  if (uint32_t(r3) == 0 || movies == 0) {
    return r3;
  }
  if (guest::LoadU32(base, Screen(base) + 896) == 0) {
    mode_tick::ScreenFadesFadeOut(base, 30, 0, 0, 0, 0);
    guest::StoreU32(base, Screen(base) + 896, 1);
  }
  uint32_t movie = Movie(base, frame, kMenusMovie);
  uint32_t picture = ui_movies::FindNode(base, movie, 6, 1);
  guest::StoreU32(base, Screen(base) + 544,
                  guest::LoadU32(base, ui_movies::PictureTextureSlot(base, movie, picture)));
  movie = Movie(base, frame, kMenusMovie);
  picture = ui_movies::FindNode(base, movie, 6, 0);
  const uint32_t texture = guest::LoadU32(base, ui_movies::PictureTextureSlot(base, movie, picture));
  guest::StoreU32(base, Screen(base) + 548, texture);
  guest::StoreU32(base, machine + 9608, 1);
  guest::StoreU32(base, machine + 9604, 4);
  return texture;
}

}  // namespace location_screen
