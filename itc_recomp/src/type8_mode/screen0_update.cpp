// The type 8 mode's first screen, its menu page, steps, update and show
// (docs/functions/type8-mode.md): Type8Screen0::MenuPage, the steps its
// Update's table names (OpeningStep, ItemStep, CursorStep, FadeOutStep),
// Update with the two players' stand (UpdateStand), Show with its texts
// (SetTexts) and the car cards (CarCard). See type8_mode.h. UiMovies::Find,
// MenuList::Selection and SetSelection, UiMovie::IsNamedActive, FindNode and
// Child, ScreenFades, MenuSound::Play, Sound::Play, CarModels::BlockOf,
// Params::GetCarSpec, Car::Init, memcpy, Car::Refresh, GarageStage::SetPlates
// and GarageStages::DrawAll through their plain C++; UiMovie::SetText through
// the calls' set_text; the UI movies' named animations, Run, FinishAll,
// ResetPose, ScrollText and its reset, SetNodeText, the menu list's Update,
// Accepted and Cancelled, the stand's GarageStage calls and the sound
// manager's slots as guest calls to their overrides.

#include "type8_mode/type8_mode.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "car_list/car_list.h"
#include "car_makers/car_makers.h"
#include "car_specs/car_specs.h"
#include "guest.h"
#include "menu_sounds/menu_sounds.h"
#include "mode_tick/mode_tick.h"
#include "race_screens/race_screens.h"
#include "race_setup/race_setup.h"
#include "type67_screens/list_pages.h"
#include "type8_screens/part_helpers.h"
#include "type8_screens/type8_screens.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace type8_mode {

namespace {

using guest::LoadU32;
using guest::StoreU32;
namespace pg = type67_screens::pages;
namespace rs = race_screens;

constexpr uint32_t kMinusOne = 0xFFFFFFFF;
constexpr uint32_t kList = 9680;
constexpr uint32_t kFinishNamed = 0x8211DF20, kPlayNamed = 0x8211DAC8, kLoopNamed = 0x8211DDE0;
constexpr uint32_t kRun = 0x8211E1B8, kFinishAll = 0x8211DFF8, kResetPose = 0x8211ABA8;
constexpr uint32_t kScrollText = 0x8211C570, kResetScroll = 0x8211C5F8;
constexpr uint32_t kIsBusy = 0x82179BE0, kReloadModel = 0x8217F880, kSetCar = 0x8217FD58;
constexpr uint32_t kCommand = 0x82180108, kSetDrawWord = 0x82178B40;
constexpr uint32_t kRefresh = 0x820C4998, kMemcpy = 0x825A7410;
constexpr uint32_t kStandHolder = 0x826D83E0;
constexpr uint32_t kStandTable = 0x82636FDC;  // three words a player
constexpr uint32_t kGarageCars = 0x82814EB8;  // 1,972 bytes a player
constexpr uint32_t kStepTable = 0x82814E70, kStepGuard = 0x82814E90;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(base, frame, sp);
  return frame;
}

uint32_t MovieAt(uint8_t* base, uint32_t frame, uint32_t id) {
  StoreU32(base, frame + 80, id);
  return LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
}

uint64_t Named(const ScreenCalls& calls, uint32_t function, uint32_t movie, uint64_t named,
               uint32_t frame, uint32_t lr) {
  return calls.call(function, frame, lr, movie, named);
}

// UiMovie::PlayNamed(movie, named, chained, 0, 0, 0), a guest call.
uint64_t Play(const ScreenCalls& calls, uint32_t movie, uint64_t named, uint64_t chained,
              uint32_t frame, uint32_t lr) {
  const uint64_t args[6] = {movie, named, chained, 0, 0, 0};
  return calls.call_set(kPlayNamed, args, 6, frame, lr);
}

uint64_t Run(const ScreenCalls& calls, uint32_t movie, uint32_t frame, uint32_t lr) {
  return calls.call(kRun, frame, lr, movie, 0);
}

uint64_t SoundThunk(void* user, uint32_t function, const uint64_t args[4], int set, uint32_t r1,
                    uint32_t lr) {
  const uint64_t six[6] = {args[0], args[1], args[2], args[3], 0, 0};
  return static_cast<const ScreenCalls*>(user)->call_set(function, six, set, r1, lr);
}

// Both screen fades' counts 0.
bool FadesDone(uint8_t* base) {
  return (LoadU32(base, mode_tick::kFadeOutCounts) | LoadU32(base, mode_tick::kFadeInCounts)) ==
         0;
}

// The page block's "the page now saved when it isn't `keep`" (+9652 to
// +9660).
void SavePage(uint8_t* base, uint32_t screen, uint32_t keep, bool when_nonzero) {
  const uint32_t page = LoadU32(base, screen + 9652);
  if (when_nonzero ? page != 0 : page != keep) {
    StoreU32(base, screen + 9660, page);
  }
}

// Flush-to-zero off, as the car's plain C++ expects at entry.
void FlushOff() {
  using Fpscr = rex::platform::FPSCRPlatform;
  Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask));
}

}  // namespace

uint64_t Screen0MenuPage(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                         const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223C6B8 Type8Screen0::MenuPage");
  const uint32_t screen = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 144);
  if (LoadU32(base, screen + 9644) == 0) {
    return r3;
  }
  const uint64_t list64 = r3 + kList;
  const uint32_t list = uint32_t(list64);
  pg::Call1(calls, rs::kListUpdate, list64, frame, 0x8223C6E4);
  bool moved = false;
  if (int32_t(uint32_t(pg::Call1(calls, rs::kListAccepted, list64, frame, 0x8223C6F4))) != 0) {
    pg::PlaySound(base, 0, frame, calls);
    if (rs::ListSelection(base, list) >= 4) {
      Screen0Confirm(base, r3, uint32_t(r4), frame, calls);
      SavePage(base, screen, 1, false);
      StoreU32(base, screen + 9668, kMinusOne);
      StoreU32(base, screen + 9652, 1);
      StoreU32(base, screen + 9664, kMinusOne);
    } else {
      SavePage(base, screen, 3, false);
      StoreU32(base, screen + 9664, 3);
      StoreU32(base, screen + 9668, kMinusOne);
      StoreU32(base, screen + 9656, 3);
      StoreU32(base, screen + 9652, kMinusOne);
    }
  } else if (int32_t(uint32_t(pg::Call1(calls, rs::kListCancelled, list64, frame,
                                        0x8223C778))) != 0) {
    pg::PlaySound(base, 2, frame, calls);
    if (rs::ListSelection(base, list) != 4) {
      rs::ListSetSelection(base, list, 4);
      SavePage(base, screen, 0, true);
      StoreU32(base, screen + 9656, 0);
      StoreU32(base, screen + 9668, 1);
      StoreU32(base, screen + 9652, kMinusOne);
      StoreU32(base, screen + 9664, 2);
    }
  } else if (LoadU32(base, screen + 9704) != 0) {
    pg::PlaySound(base, 4, frame, calls);
    SavePage(base, screen, 0, true);
    moved = true;
    StoreU32(base, screen + 9656, 0);
    StoreU32(base, screen + 9668, 1);
    StoreU32(base, screen + 9652, kMinusOne);
    StoreU32(base, screen + 9664, 2);
  }
  const uint32_t movie = MovieAt(base, frame, 24);
  const uint32_t selection = rs::ListSelection(base, list);
  calls.set_text(frame, 0x8223C850, movie, 5, 0, selection + 914);
  const uint64_t scroll[6] = {movie, 5, 0, 1, 0, 0};
  uint64_t result = calls.call_set(kScrollText, scroll, 4, frame, 0x8223C864);
  if (moved) {
    const uint64_t reset[6] = {movie, 5, 0, 0, 0, 0};
    result = calls.call_set(kResetScroll, reset, 3, frame, 0x8223C87C);
  }
  return result;
}

uint64_t Screen0OpeningStep(uint8_t* base, uint64_t r3, uint32_t sp, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223C1A0 Type8Screen0::OpeningStep");
  const uint32_t screen = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = MovieAt(base, frame, 24);
  if (LoadU32(base, screen + 9676) == 0) {
    Play(calls, movie, 55, 0, frame, 0x8223C1F4);
    if (ui_movies::IsNamedActive(base, movie, 39) == 0) {
      Play(calls, movie, 39, 1, frame, 0x8223C224);
      Named(calls, kLoopNamed, movie, 39, frame, 0x8223C230);
    }
    const uint32_t selection = rs::ListSelection(base, screen + kList);
    Named(calls, kFinishNamed, movie, 2, frame, 0x8223C248);
    Named(calls, kFinishNamed, movie, 12, frame, 0x8223C254);
    Named(calls, kFinishNamed, movie, 13, frame, 0x8223C260);
    Screen0PlaceCursor(base, selection, frame);
    Screen0ShowPage(base, 1, 0, frame);
    mode_tick::ScreenFadesFadeIn(base, LoadU32(base, 0x82001BEC), 0, 0, 0, 0);
    StoreU32(base, screen + 9676, 1);
  }
  const uint64_t run = Run(calls, movie, frame, 0x8223C2AC);
  const bool done = uint32_t(run) == 0 && FadesDone(base);
  if (done) {
    Named(calls, kFinishNamed, movie, 43, frame, 0x8223C2F8);
    Play(calls, movie, 43, 1, frame, 0x8223C314);
    Named(calls, kLoopNamed, movie, 43, frame, 0x8223C320);
  }
  return done ? 1 : 0;
}

uint64_t Screen0ItemStep(uint8_t* base, uint64_t r3, uint32_t sp, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223BBC8 Type8Screen0::ItemStep");
  const uint32_t screen = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = MovieAt(base, frame, 24);
  if (LoadU32(base, screen + 9676) == 0) {
    const uint64_t named = uint64_t(rs::ListSelection(base, screen + kList)) + 2;
    Named(calls, kFinishNamed, movie, named, frame, 0x8223BC1C);
    Play(calls, movie, named, 1, frame, 0x8223BC38);
    Named(calls, kLoopNamed, movie, named, frame, 0x8223BC44);
    StoreU32(base, screen + 9676, 1);
  }
  Run(calls, movie, frame, 0x8223BC58);
  return 1;
}

uint64_t Screen0CursorStep(uint8_t* base, uint64_t r3, uint32_t sp, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223C330 Type8Screen0::CursorStep");
  const uint32_t screen = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = MovieAt(base, frame, 24);
  if (LoadU32(base, screen + 9676) == 0) {
    const uint32_t list = screen + kList;
    const uint32_t selection = rs::ListSelection(base, list);
    const uint32_t previous = LoadU32(base, list + 20);  // GetPreviousSelection
    const int32_t moved = int32_t(selection - previous);
    Named(calls, kFinishNamed, movie, uint64_t(previous) + 2, frame, 0x8223C394);
    Named(calls, kFinishNamed, movie, 12, frame, 0x8223C3A0);
    Named(calls, kFinishNamed, movie, 13, frame, 0x8223C3AC);
    // srawi, xor and subf: the magnitude (INT_MIN stays itself).
    const uint32_t sign = uint32_t(moved >> 31);
    if (int32_t((uint32_t(moved) ^ sign) - sign) == 1) {
      const uint64_t named = moved > 0 ? 13 : 12;
      Named(calls, kFinishNamed, movie, named, frame, 0x8223C3E0);
      Play(calls, movie, named, 0, frame, 0x8223C3FC);
    } else {
      Screen0PlaceCursor(base, selection, frame);
    }
    StoreU32(base, screen + 9676, 1);
  }
  Run(calls, movie, frame, 0x8223C420);
  return 1;
}

uint64_t Screen0FadeOutStep(uint8_t* base, uint64_t r3, uint32_t sp, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223BAF0 Type8Screen0::FadeOutStep");
  const uint32_t screen = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = MovieAt(base, frame, 24);
  if (LoadU32(base, screen + 9644) == 0) {
    Run(calls, movie, frame, 0x8223BB3C);
    return 0;
  }
  if (LoadU32(base, screen + 9676) == 0) {
    mode_tick::ScreenFadesFadeOut(base, LoadU32(base, 0x82001BF0), 0, 0, 0, 0);
    StoreU32(base, screen + 9676, 1);
  }
  Run(calls, movie, frame, 0x8223BB80);
  const bool done = FadesDone(base);
  if (done) {
    calls.call(kFinishAll, frame, 0x8223BBA8, movie, 0);
  }
  return done ? 1 : 0;
}

uint64_t Screen0UpdateStand(uint8_t* base, uint64_t r3, uint32_t sp, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223B808 Type8Screen0::UpdateStand");
  const uint32_t screen = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 4096);
  const uint32_t movie = MovieAt(base, frame, 24);
  const auto slot_of = [base](uint32_t entry) {
    return LoadU32(base, LoadU32(base, kStandHolder) + (LoadU32(base, entry) << 2));
  };
  bool reloaded = false;
  for (uint32_t p = 0; p < 2; p++) {
    const uint32_t made = screen + 9768 + 20 * p;
    const uint32_t entry = kStandTable + 12 * p;
    const uint32_t car = kGarageCars + 1972 * p;
    const uint32_t slot = slot_of(entry);
    if (LoadU32(base, made) != 0 || reloaded) {
      continue;
    }
    const uint32_t block = car_makers::BlockOf(LoadU32(base, car + 12));
    const uint64_t answer = calls.call(kReloadModel, frame, 0x8223B894, slot,
                                       uint64_t(int64_t(int32_t(block))));
    reloaded = true;
    StoreU32(base, made, uint32_t(answer));
    if (int32_t(uint32_t(answer)) == 0) {
      continue;
    }
    // The car object in the frame (+96), the garage car copied into it.
    const uint32_t object = frame + 96;
    FlushOff();
    race_setup::CarInit(base, object);
    if (!c_runtime::Memcpy(base, object, car, 1884)) {
      const uint64_t args[6] = {object, car, 1884, 0, 0, 0};
      calls.call_set(kMemcpy, args, 3, frame, 0x8223B8BC);
    }
    StoreU32(base, frame + 3984, LoadU32(base, entry + 4) | LoadU32(base, frame + 3984));
    FlushOff();
    uint32_t refreshed = 0;
    if (!car_specs::CarRefresh(base, object, &refreshed)) {
      calls.call(kRefresh, frame, 0x8223B8D4, object, 0);
    }
    FlushOff();
    const uint32_t plates = LoadU32(base, car + 1968);
    type67_modes::GarageStageSetPlates(base, slot_of(entry), plates);
    const int64_t halfword = int16_t(guest::LoadU16(base, car));
    const uint64_t args[6] = {slot_of(entry), object, uint64_t(halfword), 0, 0, 0};
    calls.call_set(kSetCar, args, 3, frame, 0x8223B908);
  }
  for (uint32_t p = 0; p < 2; p++) {
    const uint32_t b = screen + 9752 + 20 * p;
    const uint32_t entry = kStandTable + 12 * p;
    const uint32_t slot = slot_of(entry);
    const uint64_t busy = calls.call(kIsBusy, frame, 0x8223B940, slot, 0);
    uint64_t draw = 0;
    if (int32_t(uint32_t(busy)) == 0 && !reloaded) {
      if (LoadU32(base, b) != 0) {
        Named(calls, kFinishNamed, movie, LoadU32(base, b + 8), frame, 0x8223B968);
        Named(calls, kFinishNamed, movie, LoadU32(base, b + 12), frame, 0x8223B974);
        Play(calls, movie, LoadU32(base, b + 12), 0, frame, 0x8223B990);
        StoreU32(base, b, 0);
      }
      StoreU32(base, LoadU32(base, slot) + 6052, 0);
      calls.call(kCommand, frame, 0x8223B9A8, slot, LoadU32(base, entry + 8));
      draw = 1;
    } else if (LoadU32(base, b) == 0) {
      Named(calls, kFinishNamed, movie, LoadU32(base, b + 4), frame, 0x8223B9C8);
      Play(calls, movie, LoadU32(base, b + 4), 0, frame, 0x8223B9E4);
      Named(calls, kFinishNamed, movie, LoadU32(base, b + 8), frame, 0x8223B9F0);
      Play(calls, movie, LoadU32(base, b + 8), 0, frame, 0x8223BA0C);
      Named(calls, kLoopNamed, movie, LoadU32(base, b + 8), frame, 0x8223BA18);
      StoreU32(base, b, 1);
    }
    calls.call(kSetDrawWord, frame, 0x8223BA2C, slot, draw);
  }
  return car_list::DrawAll(base, frame, type8_screens::CarListCalls(calls));
}

uint64_t Screen0Update(uint8_t* base, uint64_t r3, uint32_t sp, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223C588 Type8Screen0::Update");
  const uint32_t screen = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t guard = LoadU32(base, kStepGuard);
  if ((guard & 1) == 0) {
    StoreU32(base, frame + 84, 0);
    StoreU32(base, frame + 92, 0);
    StoreU32(base, frame + 100, 0);
    StoreU32(base, frame + 108, 0);
    StoreU32(base, frame + 104, 0x8223BAF0);  // FadeOutStep
    StoreU32(base, frame + 80, 0x8223C1A0);   // OpeningStep
    StoreU32(base, frame + 88, 0x8223BBC8);   // ItemStep
    StoreU32(base, kStepGuard, guard | 1);
    StoreU32(base, frame + 96, 0x8223C330);   // CursorStep
    for (uint32_t i = 0; i < 8; i++) {
      StoreU32(base, kStepTable + 4 * i, LoadU32(base, frame + 80 + 4 * i));
    }
  }
  const uint32_t movie = MovieAt(base, frame, 24);
  const uint32_t step = LoadU32(base, screen + 9664);
  if (step != kMinusOne) {
    const uint32_t target = LoadU32(base, kStepTable + (step << 3));
    const uint32_t adjustment = LoadU32(base, kStepTable + 4 + (step << 3));
    const uint64_t object = uint64_t(adjustment) + r3;
    uint64_t answer;
    switch (target) {
      case 0x8223C1A0:
        answer = Screen0OpeningStep(base, object, frame, calls);
        break;
      case 0x8223BBC8:
        answer = Screen0ItemStep(base, object, frame, calls);
        break;
      case 0x8223C330:
        answer = Screen0CursorStep(base, object, frame, calls);
        break;
      case 0x8223BAF0:
        answer = Screen0FadeOutStep(base, object, frame, calls);
        break;
      default:
        answer = calls.call(target, frame, 0x8223C664, object, frame + 80);
        break;
    }
    if (int32_t(uint32_t(answer)) != 0) {
      const uint32_t pages = screen + 9652;
      const uint32_t next = LoadU32(base, pages + 16);
      if (next == kMinusOne) {
        const uint32_t after = LoadU32(base, pages + 4);
        StoreU32(base, pages + 12, kMinusOne);
        StoreU32(base, pages + 0, after);
      } else {
        StoreU32(base, pages + 20, 0);
        StoreU32(base, pages + 12, next);
        StoreU32(base, pages + 16, kMinusOne);
      }
      StoreU32(base, pages + 24, 0);
    }
  } else {
    Run(calls, movie, frame, 0x8223C6A8);
  }
  return Screen0UpdateStand(base, r3, frame, calls);
}

uint64_t Screen0SetTexts(uint8_t* base, uint32_t sp, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223B760 Type8Screen0::SetTexts");
  const uint32_t frame = Frame(base, sp, 160);
  static constexpr uint32_t kStrings[5] = {121, 122, 110, 113, 114};
  for (uint32_t i = 0; i < 5; i++) StoreU32(base, frame + 96 + 4 * i, kStrings[i]);
  const uint32_t movie = MovieAt(base, frame, 24);
  calls.call(kResetPose, frame, 0x8223B7B8, movie, frame + 80);
  for (uint32_t i = 0; i < 5; i++) {
    calls.set_text(frame, 0x8223B7D4, movie, 0, 5 + i, LoadU32(base, frame + 96 + 4 * i));
  }
  uint64_t result = 0;
  for (uint32_t i = 0; i < 5; i++) {
    result = Named(calls, kFinishNamed, movie, i + 2, frame, 0x8223B7F4);
  }
  return result;
}

uint64_t Screen0CarCard(uint8_t* base, uint64_t player, uint64_t model, uint64_t maker,
                        uint32_t sp, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223BC68 Type8Screen0::CarCard");
  const uint32_t frame = Frame(base, sp, 144);
  StoreU32(base, frame + 88, 16);
  StoreU32(base, frame + 92, 14);
  const uint32_t movie = MovieAt(base, frame, 24);
  const uint32_t which = LoadU32(base, frame + 88 + (uint32_t(player) << 2));
  const uint32_t node = ui_movies::FindNode(base, movie, kMinusOne, which);
  namespace pd = type8_screens::part_detail;
  pd::SetNodeText(movie, ui_movies::Child(base, node, 6), uint32_t(maker + 1934), frame,
                  0x8223BCE0, calls);
  pd::SetNodeText(movie, ui_movies::Child(base, node, 10), uint32_t(model + 1646), frame,
                  0x8223BCFC, calls);
  return pd::SetNodeText(movie, ui_movies::Child(base, node, 8), uint32_t(model + 1718), frame,
                         0x8223BD18, calls);
}

uint64_t Screen0Show(uint8_t* base, uint64_t r3, uint32_t sp, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223BF18 Type8Screen0::Show");
  const uint32_t screen = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 176);
  Screen0SetTexts(base, frame, calls);
  const uint32_t page = LoadU32(base, screen + 9652);
  if (page != 0) {
    StoreU32(base, screen + 9660, page);
  }
  StoreU32(base, screen + 9664, 0);
  StoreU32(base, screen + 9656, 0);
  StoreU32(base, screen + 9652, kMinusOne);
  StoreU32(base, screen + 9668, 1);
  const uint32_t movie = MovieAt(base, frame, 24);
  for (uint32_t p = 0; p < 2; p++) {
    const uint32_t at = screen + 9756 + 20 * p;
    StoreU32(base, at + 12, 0);
    StoreU32(base, at - 4, 0);
    Named(calls, kFinishNamed, movie, LoadU32(base, at), frame, 0x8223BF94);
    Named(calls, kFinishNamed, movie, LoadU32(base, at + 4), frame, 0x8223BFA0);
    Named(calls, kFinishNamed, movie, LoadU32(base, at + 8), frame, 0x8223BFAC);
  }
  for (uint32_t i = 0; i < 2; i++) {
    const uint32_t car = kGarageCars + 1972 * i;
    const uint32_t spec = car_specs::GetCarSpec(base, LoadU32(base, car + 12));
    Screen0CarCard(base, i, LoadU32(base, car + 12), guest::LoadU8(base, spec + 769), frame,
                   calls);
  }
  // The music: the sound manager's IsCuePlaying (slot 14) for bank 1's cue
  // 0 and, when playing, its StopCue (slot 13); slot 14 for bank 1's cue 2
  // and, when it isn't playing, Sound::Play (the menu music).
  auto slot = [&](uint32_t offset, uint32_t bank, uint32_t cue, uint32_t lr) {
    const uint32_t manager = LoadU32(base, menu_sounds::kSoundManager);
    const uint64_t args[6] = {manager, bank, cue, 0, 0, 0};
    return calls.call_set(LoadU32(base, LoadU32(base, manager) + offset), args, 3, frame, lr);
  };
  StoreU32(base, frame + 84, 0);
  StoreU32(base, frame + 88, 1);
  uint64_t result = slot(56, frame + 88, frame + 84, 0x8223C024);
  if ((result & 0xFF) != 0) {
    StoreU32(base, frame + 92, 0);
    StoreU32(base, frame + 96, 1);
    slot(52, frame + 96, frame + 92, 0x8223C050);
  }
  StoreU32(base, frame + 100, 2);
  StoreU32(base, frame + 104, 1);
  result = slot(56, frame + 104, frame + 100, 0x8223C074);
  if ((result & 0xFF) != 0) {
    return result;
  }
  StoreU32(base, frame + 80, 2);
  StoreU32(base, frame + 108, 1);
  return menu_sounds::PlaySound(base, frame + 108, frame + 80, frame,
                                {const_cast<ScreenCalls*>(&calls), &SoundThunk});
}

}  // namespace type8_mode
