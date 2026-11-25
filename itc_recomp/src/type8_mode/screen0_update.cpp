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

#include "base/float_mode.h"
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
#include "schema/car_makers.h"
#include "schema/menu_helpers.h"
#include "schema/race_screens.h"
#include "schema/stand_models.h"
#include "schema/tuning_screen.h"
#include "schema/type6_type7_modes.h"
#include "schema/type8_mode.h"
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

using S = schema::Type8Screen0;
using Stand = schema::Type8StandBlock;
using StandEntry = schema::Type8StandEntry;
using GarageSlot = schema::GarageCarSlot;
using Pages = schema::PageObject;
using G = schema::Type8ModeGlobals;

constexpr uint32_t kMinusOne = 0xFFFFFFFF;
constexpr uint32_t kFinishNamed = 0x8211DF20, kPlayNamed = 0x8211DAC8, kLoopNamed = 0x8211DDE0;
constexpr uint32_t kRun = 0x8211E1B8, kFinishAll = 0x8211DFF8, kResetPose = 0x8211ABA8;
constexpr uint32_t kScrollText = 0x8211C570, kResetScroll = 0x8211C5F8;
constexpr uint32_t kIsBusy = 0x82179BE0, kReloadModel = 0x8217F880, kSetCar = 0x8217FD58;
constexpr uint32_t kCommand = 0x82180108, kSetDrawWord = 0x82178B40;
constexpr uint32_t kRefresh = 0x820C4998, kMemcpy = 0x825A7410;
constexpr uint32_t kGarageCars = G::garage_cars_address();  // 1,972 bytes a player
constexpr uint32_t kStepTable = G::step_table_address();

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(frame, sp);
  return frame;
}

// (The id as a value: the original's slot at the frame's +80 only the find
// read; E5 step 6.)
uint32_t MovieAt([[maybe_unused]] uint32_t frame, uint32_t id) {
  return LoadU32(ui_movies::PairsFindKey(ui_movies::kMovieTable, id));
}

// The same with the id stored at the frame's +80, for the callers that hand
// that slot's address on (a guest call's r4).
uint32_t MovieAtSlot(uint32_t frame, uint32_t id) {
  StoreU32(frame + 80, id);
  return MovieAt(frame, id);
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
bool FadesDone() {
  return (LoadU32(mode_tick::kFadeOutCounts) | LoadU32(mode_tick::kFadeInCounts)) ==
         0;
}

// The page block's "the page now saved when it isn't `keep`".
void SavePage(const Pages& pages, uint32_t keep, bool when_nonzero) {
  const uint32_t page = pages.page();
  if (when_nonzero ? page != 0 : page != keep) {
    pages.set_previous_page(page);
  }
}

// Flush-to-zero off, as the car's plain C++ expects at entry.
void FlushOff() {
  using Fpscr = ::base::FloatMode;
  Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask));
}

}  // namespace

uint64_t Screen0MenuPage(uint64_t r3, uint64_t r4, uint32_t sp,
                         const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223C6B8 Type8Screen0::MenuPage");
  const S s{uint32_t(r3)};
  const Pages pages(s.pages_address());
  const uint32_t frame = Frame(sp, 144);
  if (s.menu_input() == 0) {
    return r3;
  }
  const uint64_t list64 = r3 + S(0).menu_list_address();
  const uint32_t list = uint32_t(list64);
  pg::Call1(calls, rs::kListUpdate, list64, frame, 0x8223C6E4);
  bool moved = false;
  if (int32_t(uint32_t(pg::Call1(calls, rs::kListAccepted, list64, frame, 0x8223C6F4))) != 0) {
    pg::PlaySound(0, frame, calls);
    if (rs::ListSelection(list) >= 4) {
      Screen0Confirm(r3, uint32_t(r4), frame, calls);
      SavePage(pages, 1, false);
      pages.set_queued_step(kMinusOne);
      pages.set_page(1);
      pages.set_step(kMinusOne);
    } else {
      SavePage(pages, 3, false);
      pages.set_step(3);
      pages.set_queued_step(kMinusOne);
      pages.set_next_page(3);
      pages.set_page(kMinusOne);
    }
  } else if (int32_t(uint32_t(pg::Call1(calls, rs::kListCancelled, list64, frame,
                                        0x8223C778))) != 0) {
    pg::PlaySound(2, frame, calls);
    if (rs::ListSelection(list) != 4) {
      rs::ListSetSelection(list, 4);
      SavePage(pages, 0, true);
      pages.set_next_page(0);
      pages.set_queued_step(1);
      pages.set_page(kMinusOne);
      pages.set_step(2);
    }
  } else if (schema::MenuList(list).move() != 0) {
    pg::PlaySound(4, frame, calls);
    SavePage(pages, 0, true);
    moved = true;
    pages.set_next_page(0);
    pages.set_queued_step(1);
    pages.set_page(kMinusOne);
    pages.set_step(2);
  }
  const uint32_t movie = MovieAt(frame, 24);
  const uint32_t selection = rs::ListSelection(list);
  calls.set_text(frame, 0x8223C850, movie, 5, 0, selection + 914);
  const uint64_t scroll[6] = {movie, 5, 0, 1, 0, 0};
  uint64_t result = calls.call_set(kScrollText, scroll, 4, frame, 0x8223C864);
  if (moved) {
    const uint64_t reset[6] = {movie, 5, 0, 0, 0, 0};
    result = calls.call_set(kResetScroll, reset, 3, frame, 0x8223C87C);
  }
  return result;
}

uint64_t Screen0OpeningStep(uint64_t r3, uint32_t sp, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223C1A0 Type8Screen0::OpeningStep");
  const S s{uint32_t(r3)};
  const Pages pages(s.pages_address());
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = MovieAt(frame, 24);
  if (pages.step_started() == 0) {
    Play(calls, movie, 55, 0, frame, 0x8223C1F4);
    if (ui_movies::IsNamedActive(movie, 39) == 0) {
      Play(calls, movie, 39, 1, frame, 0x8223C224);
      Named(calls, kLoopNamed, movie, 39, frame, 0x8223C230);
    }
    const uint32_t selection = rs::ListSelection(s.menu_list_address());
    Named(calls, kFinishNamed, movie, 2, frame, 0x8223C248);
    Named(calls, kFinishNamed, movie, 12, frame, 0x8223C254);
    Named(calls, kFinishNamed, movie, 13, frame, 0x8223C260);
    Screen0PlaceCursor(selection, frame);
    Screen0ShowPage(1, 0, frame);
    mode_tick::ScreenFadesFadeIn(G::fade_in_passes(), 0, 0, 0, 0);
    pages.set_step_started(1);
  }
  const uint64_t run = Run(calls, movie, frame, 0x8223C2AC);
  const bool done = uint32_t(run) == 0 && FadesDone();
  if (done) {
    Named(calls, kFinishNamed, movie, 43, frame, 0x8223C2F8);
    Play(calls, movie, 43, 1, frame, 0x8223C314);
    Named(calls, kLoopNamed, movie, 43, frame, 0x8223C320);
  }
  return done ? 1 : 0;
}

uint64_t Screen0ItemStep(uint64_t r3, uint32_t sp, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223BBC8 Type8Screen0::ItemStep");
  const S s{uint32_t(r3)};
  const Pages pages(s.pages_address());
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = MovieAt(frame, 24);
  if (pages.step_started() == 0) {
    const uint64_t named = uint64_t(rs::ListSelection(s.menu_list_address())) + 2;
    Named(calls, kFinishNamed, movie, named, frame, 0x8223BC1C);
    Play(calls, movie, named, 1, frame, 0x8223BC38);
    Named(calls, kLoopNamed, movie, named, frame, 0x8223BC44);
    pages.set_step_started(1);
  }
  Run(calls, movie, frame, 0x8223BC58);
  return 1;
}

uint64_t Screen0CursorStep(uint64_t r3, uint32_t sp, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223C330 Type8Screen0::CursorStep");
  const S s{uint32_t(r3)};
  const Pages pages(s.pages_address());
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = MovieAt(frame, 24);
  if (pages.step_started() == 0) {
    const uint32_t list = s.menu_list_address();
    const uint32_t selection = rs::ListSelection(list);
    const uint32_t previous = schema::MenuList(list).selection_before();  // GetPreviousSelection
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
      Screen0PlaceCursor(selection, frame);
    }
    pages.set_step_started(1);
  }
  Run(calls, movie, frame, 0x8223C420);
  return 1;
}

uint64_t Screen0FadeOutStep(uint64_t r3, uint32_t sp, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223BAF0 Type8Screen0::FadeOutStep");
  const S s{uint32_t(r3)};
  const Pages pages(s.pages_address());
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = MovieAt(frame, 24);
  if (s.menu_input() == 0) {
    Run(calls, movie, frame, 0x8223BB3C);
    return 0;
  }
  if (pages.step_started() == 0) {
    mode_tick::ScreenFadesFadeOut(G::fade_out_passes(), 0, 0, 0, 0);
    pages.set_step_started(1);
  }
  Run(calls, movie, frame, 0x8223BB80);
  const bool done = FadesDone();
  if (done) {
    calls.call(kFinishAll, frame, 0x8223BBA8, movie, 0);
  }
  return done ? 1 : 0;
}

uint64_t Screen0UpdateStand(uint64_t r3, uint32_t sp, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223B808 Type8Screen0::UpdateStand");
  const S s{uint32_t(r3)};
  const uint32_t frame = Frame(sp, 4096);
  const uint32_t movie = MovieAt(frame, 24);
  const auto slot_of = [](const StandEntry& entry) {
    return LoadU32(G::stand_holder().stage_block_0_address() + (entry.stage_index() << 2));
  };
  const auto stand_of = [&](uint32_t p) {
    return Stand(s.stand_0_address() + Stand::kSize * p);
  };
  const auto entry_of = [](uint32_t p) {
    return StandEntry(G::stand_table_address() + StandEntry::kSize * p);
  };
  bool reloaded = false;
  for (uint32_t p = 0; p < 2; p++) {
    const Stand stand = stand_of(p);
    const StandEntry entry = entry_of(p);
    const GarageSlot garage(kGarageCars + GarageSlot::kSize * p);
    const uint32_t car = schema::GarageCar(garage.car_address()).car_head_address();
    const schema::CarMakersCar head(car);
    const uint32_t slot = slot_of(entry);
    if (stand.model_made() != 0 || reloaded) {
      continue;
    }
    const uint32_t block = car_makers::BlockOf(head.car_id());
    const uint64_t answer = calls.call(kReloadModel, frame, 0x8223B894, slot,
                                       uint64_t(int64_t(int32_t(block))));
    reloaded = true;
    stand.set_model_made(uint32_t(answer));
    if (int32_t(uint32_t(answer)) == 0) {
      continue;
    }
    // The car object in the frame (+96), the garage car copied into it.
    const uint32_t object = frame + 96;
    FlushOff();
    race_setup::CarInit(object);
    if (!c_runtime::Memcpy(object, car, 1884)) {
      const uint64_t args[6] = {object, car, 1884, 0, 0, 0};
      calls.call_set(kMemcpy, args, 3, frame, 0x8223B8BC);
    }
    StoreU32(frame + 3984, entry.car_flag() | LoadU32(frame + 3984));
    FlushOff();
    uint32_t refreshed = 0;
    if (!car_specs::CarRefresh(object, &refreshed)) {
      calls.call(kRefresh, frame, 0x8223B8D4, object, 0);
    }
    FlushOff();
    const uint32_t plates = garage.unknown_1968();  // the plates
    type67_modes::GarageStageSetPlates(slot_of(entry), plates);
    const int64_t halfword = int16_t(head.paint_pick());
    const uint64_t args[6] = {slot_of(entry), object, uint64_t(halfword), 0, 0, 0};
    calls.call_set(kSetCar, args, 3, frame, 0x8223B908);
  }
  for (uint32_t p = 0; p < 2; p++) {
    const Stand b = stand_of(p);
    const StandEntry entry = entry_of(p);
    const uint32_t slot = slot_of(entry);
    const uint64_t busy = calls.call(kIsBusy, frame, 0x8223B940, slot, 0);
    uint64_t draw = 0;
    if (int32_t(uint32_t(busy)) == 0 && !reloaded) {
      if (b.loading_shown() != 0) {
        Named(calls, kFinishNamed, movie, b.loading_loop_anim(), frame, 0x8223B968);
        Named(calls, kFinishNamed, movie, b.loaded_anim(), frame, 0x8223B974);
        Play(calls, movie, b.loaded_anim(), 0, frame, 0x8223B990);
        b.set_loading_shown(0);
      }
      schema::StandSlot(slot).stage().set_reflection(0);
      calls.call(kCommand, frame, 0x8223B9A8, slot, entry.command());
      draw = 1;
    } else if (b.loading_shown() == 0) {
      Named(calls, kFinishNamed, movie, b.loading_start_anim(), frame, 0x8223B9C8);
      Play(calls, movie, b.loading_start_anim(), 0, frame, 0x8223B9E4);
      Named(calls, kFinishNamed, movie, b.loading_loop_anim(), frame, 0x8223B9F0);
      Play(calls, movie, b.loading_loop_anim(), 0, frame, 0x8223BA0C);
      Named(calls, kLoopNamed, movie, b.loading_loop_anim(), frame, 0x8223BA18);
      b.set_loading_shown(1);
    }
    calls.call(kSetDrawWord, frame, 0x8223BA2C, slot, draw);
  }
  return car_list::DrawAll(frame, type8_screens::CarListCalls(calls));
}

uint64_t Screen0Update(uint64_t r3, uint32_t sp, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223C588 Type8Screen0::Update");
  const Pages pages(S(uint32_t(r3)).pages_address());
  const uint32_t frame = Frame(sp, 160);
  const uint32_t guard = G::step_table_guard();
  if ((guard & 1) == 0) {
    // The steps' member functors (the original builds them at the frame's
    // +80 to +108; a C++ array since E5 step 6): OpeningStep, ItemStep,
    // CursorStep, FadeOutStep, each with adjustment 0.
    static constexpr uint32_t kSteps[8] = {0x8223C1A0, 0, 0x8223BBC8, 0,
                                           0x8223C330, 0, 0x8223BAF0, 0};
    G::set_step_table_guard(guard | 1);
    for (uint32_t i = 0; i < 8; i++) {
      StoreU32(kStepTable + 4 * i, kSteps[i]);
    }
  }
  // The id at +80 (a guest step gets its address).
  const uint32_t movie = MovieAtSlot(frame, 24);
  const uint32_t step = pages.step();
  if (step != kMinusOne) {
    const uint32_t target = LoadU32(kStepTable + (step << 3));
    const uint32_t adjustment = LoadU32(kStepTable + 4 + (step << 3));
    const uint64_t object = uint64_t(adjustment) + r3;
    uint64_t answer;
    switch (target) {
      case 0x8223C1A0:
        answer = Screen0OpeningStep(object, frame, calls);
        break;
      case 0x8223BBC8:
        answer = Screen0ItemStep(object, frame, calls);
        break;
      case 0x8223C330:
        answer = Screen0CursorStep(object, frame, calls);
        break;
      case 0x8223BAF0:
        answer = Screen0FadeOutStep(object, frame, calls);
        break;
      default:
        answer = calls.call(target, frame, 0x8223C664, object, frame + 80);
        break;
    }
    if (int32_t(uint32_t(answer)) != 0) {
      const uint32_t next = pages.queued_step();
      if (next == kMinusOne) {
        const uint32_t after = pages.next_page();
        pages.set_step(kMinusOne);
        pages.set_page(after);
      } else {
        pages.set_unknown_20(0);
        pages.set_step(next);
        pages.set_queued_step(kMinusOne);
      }
      pages.set_step_started(0);
    }
  } else {
    Run(calls, movie, frame, 0x8223C6A8);
  }
  return Screen0UpdateStand(r3, frame, calls);
}

uint64_t Screen0SetTexts(uint32_t sp, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223B760 Type8Screen0::SetTexts");
  const uint32_t frame = Frame(sp, 160);
  // The strings (the original's table at the frame's +96: a C++ array since
  // E5 step 6).
  static constexpr uint32_t kStrings[5] = {121, 122, 110, 113, 114};
  const uint32_t movie = MovieAtSlot(frame, 24);  // ResetPose gets +80's address
  calls.call(kResetPose, frame, 0x8223B7B8, movie, frame + 80);
  for (uint32_t i = 0; i < 5; i++) {
    calls.set_text(frame, 0x8223B7D4, movie, 0, 5 + i, kStrings[i]);
  }
  uint64_t result = 0;
  for (uint32_t i = 0; i < 5; i++) {
    result = Named(calls, kFinishNamed, movie, i + 2, frame, 0x8223B7F4);
  }
  return result;
}

uint64_t Screen0CarCard(uint64_t player, uint64_t model, uint64_t maker,
                        uint32_t sp, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223BC68 Type8Screen0::CarCard");
  const uint32_t frame = Frame(sp, 144);
  const uint32_t movie = MovieAt(frame, 24);
  // The players' nodes (the original's table at the frame's +88: a C++ array
  // since E5 step 6), read unchecked: a player outside it reads the stack
  // word the original reads there.
  static constexpr uint32_t kNodes[2] = {16, 14};
  const uint32_t offset = uint32_t(player) << 2;
  const uint32_t which = offset < 8 ? kNodes[offset >> 2] : LoadU32(frame + 88 + offset);
  const uint32_t node = ui_movies::FindNode(movie, kMinusOne, which);
  namespace pd = type8_screens::part_detail;
  pd::SetNodeText(movie, ui_movies::Child(node, 6), uint32_t(maker + 1934), frame,
                  0x8223BCE0, calls);
  pd::SetNodeText(movie, ui_movies::Child(node, 10), uint32_t(model + 1646), frame,
                  0x8223BCFC, calls);
  return pd::SetNodeText(movie, ui_movies::Child(node, 8), uint32_t(model + 1718), frame,
                         0x8223BD18, calls);
}

uint64_t Screen0Show(uint64_t r3, uint32_t sp, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223BF18 Type8Screen0::Show");
  const S s{uint32_t(r3)};
  const Pages pages(s.pages_address());
  const uint32_t frame = Frame(sp, 176);
  Screen0SetTexts(frame, calls);
  const uint32_t page = pages.page();
  if (page != 0) {
    pages.set_previous_page(page);
  }
  pages.set_step(0);
  pages.set_next_page(0);
  pages.set_page(kMinusOne);
  pages.set_queued_step(1);
  const uint32_t movie = MovieAt(frame, 24);
  for (uint32_t p = 0; p < 2; p++) {
    const Stand stand(s.stand_0_address() + Stand::kSize * p);
    stand.set_model_made(0);
    stand.set_loading_shown(0);
    Named(calls, kFinishNamed, movie, stand.loading_start_anim(), frame, 0x8223BF94);
    Named(calls, kFinishNamed, movie, stand.loading_loop_anim(), frame, 0x8223BFA0);
    Named(calls, kFinishNamed, movie, stand.loaded_anim(), frame, 0x8223BFAC);
  }
  for (uint32_t i = 0; i < 2; i++) {
    const GarageSlot garage(kGarageCars + GarageSlot::kSize * i);
    const schema::CarMakersCar head(garage.car_address());
    const uint32_t spec = car_specs::GetCarSpec(head.car_id());
    Screen0CarCard(i, head.car_id(), guest::LoadU8(spec + 769), frame,
                   calls);
  }
  // The music: the sound manager's IsCuePlaying (slot 14) for bank 1's cue
  // 0 and, when playing, its StopCue (slot 13); slot 14 for bank 1's cue 2
  // and, when it isn't playing, Sound::Play (the menu music).
  auto slot = [&](uint32_t offset, uint32_t bank, uint32_t cue, uint32_t lr) {
    const uint32_t manager = LoadU32(menu_sounds::kSoundManager);
    const uint64_t args[6] = {manager, bank, cue, 0, 0, 0};
    return calls.call_set(LoadU32(LoadU32(manager) + offset), args, 3, frame, lr);
  };
  StoreU32(frame + 84, 0);
  StoreU32(frame + 88, 1);
  uint64_t result = slot(56, frame + 88, frame + 84, 0x8223C024);
  if ((result & 0xFF) != 0) {
    StoreU32(frame + 92, 0);
    StoreU32(frame + 96, 1);
    slot(52, frame + 96, frame + 92, 0x8223C050);
  }
  StoreU32(frame + 100, 2);
  StoreU32(frame + 104, 1);
  result = slot(56, frame + 104, frame + 100, 0x8223C074);
  if ((result & 0xFF) != 0) {
    return result;
  }
  StoreU32(frame + 80, 2);
  StoreU32(frame + 108, 1);
  return menu_sounds::PlaySound(frame + 108, frame + 80, frame,
                                {const_cast<ScreenCalls*>(&calls), &SoundThunk});
}

}  // namespace type8_mode
