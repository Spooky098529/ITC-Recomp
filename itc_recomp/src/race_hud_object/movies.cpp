// The race HUD object's movie calls and its set-up
// (docs/functions/race-hud-object.md): see race_hud_object.h.

#include "race_hud_object/race_hud_object.h"

#include "alt_hud/alt_hud.h"
#include "base/log.h"
#include "camera_modes/steps.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "helpers/debug_output.h"
#include "memory/heap.h"
#include "race_screens/race_screens.h"
#include "schema/car_physics.h"
#include "schema/alt_hud.h"
#include "schema/menu_sounds.h"
#include "schema/race_event.h"
#include "schema/race_hud_machine.h"
#include "schema/race_hud_object.h"
#include "schema/race_screens.h"
#include "schema/race_screens_unseen.h"
#include "schema/race_states.h"
#include "schema/strings_and_font.h"
#include "schema/ui_movies.h"
#include "schema/ui_movies_game.h"
#include "schema/vehicle.h"
#include "menu_sounds/menu_sounds.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>
#include <optional>

namespace race_hud_object {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;  // a sub-scene index of -1

using Hud = schema::RaceHud;
using Racers = schema::RaceHudRacers;
using G = schema::RaceHudObjectGlobals;

// The HUD's movies (each pointer loaded at the call).
uint32_t RacersMovie(uint8_t* base, uint32_t hud) { return Hud(base, hud).racers_movie().address; }
uint32_t Dashboard(uint8_t* base, uint32_t hud) { return Hud(base, hud).dashboard().address; }
uint32_t Countdown(uint8_t* base, uint32_t hud) { return Hud(base, hud).countdown().address; }
uint32_t Warning(uint8_t* base, uint32_t hud) { return Hud(base, hud).warning().address; }
uint32_t Timer(uint8_t* base, uint32_t hud) { return Hud(base, hud).timer().address; }
uint32_t Movie1(uint8_t* base, uint32_t hud) { return Hud(base, hud).movie1().address; }

// UiMovie::LoopNamed's or UiMovie::Loop's r3 (their adapters' MovedR3):
// the slot moved to, else the copy in their 176-byte frame below `frame`
// (+80) when the list had slots, or the movie.
uint64_t MovedR3(const ui_movies::Moved& moved, uint32_t frame, uint32_t movie) {
  if (moved.slot) {
    return *moved.slot;
  }
  return moved.looked ? frame - 176 + 80 : movie;
}

}  // namespace

uint32_t StartCountdown(uint8_t* base, uint32_t hud) {
  ITC_NATIVE_RUNNING("sub_82212270 RaceHud::StartCountdown");
  const uint32_t movie = Countdown(base, hud);
  Hud(base, hud).set_countdown_running(1);
  return ui_movies::Play(base, movie, kNone, 0, 0, 0, 0, 0);
}

uint64_t StartMovie1(uint8_t* base, uint32_t hud, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82212718 RaceHud::StartMovie1");
  const uint32_t frame = sp - 96;
  guest::StoreU32(base, frame, sp);
  ui_movies::PlayNamed(base, Movie1(base, hud), 0, 0, 0, 0, 0);
  const uint32_t movie = Movie1(base, hud);
  return MovedR3(ui_movies::MoveNamedAnimation(base, movie, 0, true), frame, movie);
}

uint64_t FinishMovie1(uint8_t* base, uint32_t hud, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82212878 RaceHud::FinishMovie1");
  return calls.Call(kFinishNamed, sp, lr, Movie1(base, hud), 0);
}

uint64_t DrawWarning(uint8_t* base, uint32_t hud, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82212AC8 RaceHud::DrawWarning");
  if (Hud(base, hud).warning_shown() == 0) {
    return hud;
  }
  return calls.Call(kRun, sp, lr, Warning(base, hud), 1);
}

uint64_t RunMovie1(uint8_t* base, uint32_t hud, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82212AF8 RaceHud::RunMovie1");
  return calls.Call(kRun, sp, lr, Movie1(base, hud), 0);
}

uint64_t RunItem(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82212B08 RaceHud::RunItem");
  const uint32_t frame = sp - 96;
  guest::StoreU32(base, frame, sp);
  if (int32_t(Hud(base, hud).item_shown()) == -1) {
    return hud;
  }
  calls.Call(kRun, frame, 0x82212B34, RacersMovie(base, hud), 0);
  // sub_82106540(200, 500, 0, 0x8200F710, +460): empty (audio.md), so
  // nothing but its r3, the 200 it was handed.
  return 200;
}

uint64_t Draw(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822128A8 RaceHud::Draw");
  const uint32_t frame = sp - 96;
  guest::StoreU32(base, frame, sp);
  uint64_t r3 = calls.Call(kRun, frame, 0x822128C8, Dashboard(base, hud), 0);
  const Hud h(base, hud);
  if (h.view_dashboard() == 0 || h.option_mirror() == 0) {
    return r3;
  }
  const uint32_t mirror = ui_movies::FindNode(base, Dashboard(base, hud), kNone, 19);
  return calls.Call(kDrawMirror, frame, 0x822128F4, mirror);
}

uint64_t UpdateWarning(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82212C50 RaceHud::UpdateWarning");
  const uint32_t frame = sp - 96;
  guest::StoreU32(base, frame, sp);
  const Hud h(base, hud);
  const schema::Vehicle car = h.vehicle();
  const uint32_t wrong_way = schema::VehicleStatus(base, car.status_address()).wrong_way();
  if (wrong_way == 0) {
    if (h.warning_shown() == 0) {
      return hud;
    }
    const uint64_t r3 =
        calls.Call(kFinish, frame, 0x82212C90, Warning(base, hud), kNone, 0);
    h.set_warning_shown(0);
    return r3;
  }
  if (h.warning_shown() != 0) {
    return hud;
  }
  calls.Call(kFinish, frame, 0x82212CBC, Warning(base, hud), kNone, 0);
  ui_movies::Play(base, Warning(base, hud), kNone, 0, 0, 0, 0, 0);
  const uint32_t movie = Warning(base, hud);
  const uint64_t r3 =
      MovedR3(ui_movies::MoveAnimation(base, movie, kNone, 0, true), frame, movie);
  h.set_warning_shown(1);
  return r3;
}

uint64_t UpdateRecordingFull(uint8_t* base, uint32_t hud, int32_t room, uint32_t sp,
                             const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82212E60 RaceHud::UpdateRecordingFull");
  const uint32_t frame = sp - 96;
  guest::StoreU32(base, frame, sp);
  const Hud h(base, hud);
  if (room == 0) {
    if (h.recording_full() != 0) {
      return hud;
    }
    const uint32_t dashboard = Dashboard(base, hud);
    h.set_recording_full(1);
    calls.Call(kFinishNamed, frame, 0x82212E9C, dashboard, 6);
    return ui_movies::PlayNamed(base, Dashboard(base, hud), 6, 0, 0, 0, 0);
  }
  if (room != 1) {
    return hud;
  }
  const uint32_t dashboard = Dashboard(base, hud);
  h.set_recording_full(0);
  const uint32_t node = ui_movies::FindNode(base, dashboard, kNone, 5);
  camera_modes::steps::Stfs(base, node + 68, camera_modes::steps::Lfs(base, kZero));
  return node;
}

uint64_t ShowItem(uint8_t* base, uint32_t hud, int32_t id, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82213638 RaceHud::ShowItem");
  const uint32_t frame = sp - 128;
  guest::StoreU32(base, frame, sp);
  constexpr uint32_t kTagPrm = 0x50524D00;  // "PRM\0"
  constexpr uint32_t kTagPic = 0x50494300;  // "PIC\0"
  constexpr uint32_t kTagTxt = 0x54585400;  // "TXT\0"
  const uint32_t shown = id == -1 ? 1 : 0;
  uint32_t chunk = schema::UiMovie(base, RacersMovie(base, hud)).live_scene();
  while (schema::UiMovieChunk(base, chunk).tag() != ui_movies::kTagTrm) {
    const uint32_t tag = schema::UiMovieChunk(base, chunk).tag();
    if (tag == kTagPrm || tag == kTagPic || tag == kTagTxt) {
      guest::StoreU32(base, chunk + 80, shown);
    }
    chunk += schema::UiMovieChunk(base, chunk).next();
  }
  if (id == -1) {
    return hud;
  }
  const uint32_t name = ui_movies::FindNode(base, RacersMovie(base, hud), kNone, 19);
  const uint32_t picture = ui_movies::FindNode(base, RacersMovie(base, hud), 5, 0);
  guest::StoreU32(base, name + 80, 1);
  const uint32_t panel = ui_movies::FindNode(base, RacersMovie(base, hud), kNone, 5);
  guest::StoreU32(base, panel + 80, 1);
  guest::StoreU32(base, picture + 80, 1);
  // UiMovie::SetNodeText's r6 to r10 (varargs) passed 0: the original leaves
  // them unset (the user's call for the parking area, 2026-10-30).
  calls.Call(kSetNodeText, frame, 0x82213724, RacersMovie(base, hud), name,
             uint64_t(int64_t(id) + 4650));
  const uint32_t item = data_tables::Item(base, uint32_t(id));
  const int32_t kind = int8_t(schema::OpponentItem(base, item).category());
  guest::StoreU32(base, picture + 108, uint32_t(kind < 48 ? kind : id - 262));
  return item;
}

uint64_t UpdateItem(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82214238 RaceHud::UpdateItem");
  const Hud h(base, hud);
  const uint32_t item = G::race_progress(base).item_shown();
  if (h.item_shown() == item) {
    return hud;
  }
  h.set_item_shown(item);
  return ShowItem(base, hud, int32_t(item), sp, calls);
}

namespace {

// The HUD's item picture: DataTables::Item(id)'s byte +34 (signed), or for
// 48 and up id - 262 (the originals' word from the zero-extended id).
uint32_t ItemPicture(uint8_t* base, uint32_t id) {
  const int32_t kind = int8_t(schema::OpponentItem(base, data_tables::Item(base, id)).category());
  return kind < 48 ? uint32_t(kind) : id - 262;
}

constexpr uint64_t kAll = ~0ull;  // a sub-scene of -1 in a guest call's register

}  // namespace

uint64_t SetUpTimer(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822123C8 RaceHud::SetUpTimer");
  const uint32_t frame = sp - 144;
  guest::StoreU32(base, frame, sp);
  const Hud h(base, hud);
  const Racers racers = h.racers();
  const uint32_t animation = racers.timer_animation();
  const uint32_t count = racers.count();
  h.set_timer_animation(animation);
  if (int32_t(animation) >= 0) {
    for (uint32_t i = 0; i < 5; i++) {
      calls.Call(kFinishNamed, frame, 0x82212408, Timer(base, hud), i);
    }
    const uint32_t node = ui_movies::FindNode(base, Timer(base, hud), kNone, 0);
    guest::StoreU32(base, node + 80, 1);
    ui_movies::PlayNamed(base, Timer(base, hud), h.timer_animation(), 0, 0, 0, 0);
    ui_movies::MoveNamedAnimation(base, Timer(base, hud), h.timer_animation(), true);
  } else {
    const uint32_t node = ui_movies::FindNode(base, Timer(base, hud), kNone, 0);
    guest::StoreU32(base, node + 80, 0);
  }
  const uint32_t group = ui_movies::FindNode(base, Timer(base, hud), kNone, 3);
  for (int32_t k = 0; k < int32_t(guest::LoadU32(base, group + 104)); k++) {
    const uint32_t node = ui_movies::FindNode(base, Timer(base, hud), kNone,
                                              guest::LoadU32(base, group + 100) + uint32_t(k));
    guest::StoreU32(base, node + 80, 0);
  }
  // Record count - 1's item (records 972 bytes from the racers' +0).
  const uint32_t last = guest::LoadU32(base, h.racers().address + count * 972 - 964);
  if (int32_t(last) != -1) {
    const uint32_t child = guest::LoadU32(base, group + 100) + count - 2;
    const uint32_t shown = ui_movies::FindNode(base, Timer(base, hud), kNone, child);
    guest::StoreU32(base, shown + 80, 1);
    const uint32_t picture = ui_movies::FindNode(base, Timer(base, hud),
                                                 guest::LoadU32(base, group + 100) + count - 2, 0);
    guest::StoreU32(base, picture + 108, ItemPicture(base, last));
  }
  for (uint32_t i = 1; i < 4; i++) {
    if (int32_t(i) < int32_t(count + 1)) {
      const uint32_t id = guest::LoadU32(base, h.racers().item0_address() + 972 * i);
      if (int32_t(id) != -1) {
        // SetText's varargs (r7 to r10) passed 0: the original leaves them
        // unset (the user's call for the parking area, 2026-10-30).
        calls.Call(kSetText, frame, 0x82212568, Timer(base, hud), kAll, i + 26,
                   uint64_t(id) + 4650);
      }
    } else {
      calls.Call(kClearText, frame, 0x8221257C, Timer(base, hud), kAll, i + 26);
    }
  }
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  return calls.Call(kSetChildNameText, frame, 0x822125AC, Timer(base, hud), kAll, 26,
                    uint64_t(save) + 8);
}

uint64_t SetUpRacers(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82213F20 RaceHud::SetUpRacers");
  using namespace camera_modes::steps;
  const uint32_t frame = sp - 176;
  guest::StoreU32(base, frame, sp);
  ShowItem(base, hud, -1, frame, calls);
  const Hud h(base, hud);
  const uint32_t count = h.racers().count();
  h.set_racer_count(count);
  uint32_t group = ui_movies::FindNode(base, RacersMovie(base, hud), kNone, 0);
  {
    uint32_t k = guest::LoadU32(base, group + 100);
    uint32_t end = guest::LoadU32(base, group + 104) + k;
    while (int32_t(k) < int32_t(end)) {
      const uint32_t node = ui_movies::FindNode(base, RacersMovie(base, hud), kNone, k);
      guest::StoreU32(base, node + 80, 0);
      k++;
      end = guest::LoadU32(base, group + 100) + guest::LoadU32(base, group + 104);
    }
  }
  const uint32_t racers = h.racers().address;
  const uint32_t last = guest::LoadU32(base, racers + h.racer_count() * 972 - 964);
  if (int32_t(last) != -1) {
    const uint32_t shown = ui_movies::FindNode(base, RacersMovie(base, hud), kNone,
                                               guest::LoadU32(base, group + 100) + count - 2);
    guest::StoreU32(base, shown + 80, 1);
    const uint32_t picture = ui_movies::FindNode(base, RacersMovie(base, hud),
                                                 guest::LoadU32(base, group + 100) + count - 2, 0);
    guest::StoreU32(base, picture + 108, ItemPicture(base, last));
  }
  for (uint32_t k = 1; k < 3; k++) {
    const uint32_t node = ui_movies::FindNode(base, RacersMovie(base, hud), kNone, k + 2);
    guest::StoreU32(base, node + 80, 1);
  }
  if (int32_t(count - 1) < 3) {
    for (uint32_t k = count + 1; int32_t(k) < 5; k++) {
      const uint32_t node = ui_movies::FindNode(base, RacersMovie(base, hud), kNone, k);
      guest::StoreU32(base, node + 80, 0);
    }
  }
  if (int32_t(count) > 0) {
    // The constants, loaded once before the loop as the original does.
    const double zero = Lfs(base, kZero);
    const double scale4 = Lfs(base, 0x82002A60);
    const double scale12 = Lfs(base, 0x8200F724);
    for (uint32_t i = 0; int32_t(i) < int32_t(count); i++) {
      group = ui_movies::FindNode(base, RacersMovie(base, hud), kNone, i + 1);
      if (i == 0) {
        const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
        calls.Call(kSetChildNameText, frame, 0x822140D4, RacersMovie(base, hud), kAll,
                   uint64_t(guest::LoadU32(base, group + 100)) + 5, uint64_t(save) + 8);
      } else {
        const uint32_t id = guest::LoadU32(base, h.racers().item0_address() + 972 * i);
        if (int32_t(id) != -1) {
          // SetText's varargs passed 0 (the user's call, 2026-10-30).
          calls.Call(kSetText, frame, 0x82214108, RacersMovie(base, hud), kAll,
                     uint64_t(guest::LoadU32(base, group + 100)) + 5, uint64_t(id) + 4650);
        }
      }
      const uint32_t record = h.racers().address + 972 * i;
      const schema::RaceHudBar bar(base, h.bars_address() + 40 * i);
      const int64_t target = int32_t(Racers(base, record).bar_max0());
      bar.set_min(float(zero));
      bar.set_value(float(zero));
      bar.set_animation_played(0);
      bar.set_touched(0);
      // std (the conversion's doubleword): the high word first.
      guest::StoreU32(base, frame + 80, uint32_t(uint64_t(target) >> 32));
      guest::StoreU32(base, frame + 84, uint32_t(target));
      const double value = Single(double(target));
      bar.set_max(float(value));
      bar.set_target(float(value));
      bar.set_trail(float(value));
      bar.set_value_step(float(Fmuls(value, scale4)));
      bar.set_trail_step(float(Fmuls(value, scale12)));
    }
  }
  uint64_t r3 = 0;
  for (uint32_t i = 0; i < 4; i++) {
    r3 = calls.Call(kFinish, frame, 0x8221417C, RacersMovie(base, hud), kAll, i);
  }
  h.set_any_bar_low(0);
  return r3;
}

uint64_t SetMode(uint8_t* base, uint32_t hud, uint64_t mode, uint32_t sp, uint32_t lr,
                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822157A8 RaceHud::SetMode");
  Hud(base, hud).set_mode(uint32_t(mode));
  switch (uint32_t(mode)) {
    case 0:
      return SetUpRacers(base, hud, sp, calls);
    case 1:
      return SetUpTimer(base, hud, sp, calls);
    case 2:
      return calls.Call(kSetUpMode2, sp, lr, hud, mode);
    case 3:
      return calls.Call(kSetUpMode3, sp, lr, hud, mode);
    case 4:
      return calls.Call(kSetUpMode4, sp, lr, hud, mode);
    default:
      return hud;
  }
}

uint32_t MovieMask(uint32_t kind) {
  ITC_NATIVE_RUNNING("sub_82211BE8 RaceScreens::MovieMask");
  static constexpr uint32_t kMasks[11] = {0x404E5F, 0x406A5F, 0x405A9F, 0x401F,
                                          0x41C320, 0x41C320, 0x72591F, 0x484F1F,
                                          0x40000,  0x404010, 0x7FFFFF};
  return kind > 10 ? 0 : kMasks[kind];
}

uint32_t Init(uint8_t* base, uint32_t hud, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82215260 RaceHud::Init");
  const uint32_t frame = sp - 128;
  guest::StoreU32(base, frame, sp);
  const Hud h(base, hud);
  const struct {
    uint32_t id, field;
  } kMovies[9] = {{10, h.racers_movie_address()}, {0, h.dashboard_address()},
                  {11, h.countdown_address()},    {3, h.warning_address()},
                  {2, h.movie2_address()},        {13, h.timer_address()},
                  {1, h.movie1_address()},        {12, h.splits_address()},
                  {19, h.movie19_address()}};
  const uint32_t key = frame + 80;
  uint32_t found = 0;
  for (uint32_t i = 0; i < 9; i++) {
    guest::StoreU32(base, key, kMovies[i].id);
    found = ui_movies::PairsFind(base, ui_movies::kMovieTable, key);
    const uint32_t movie = guest::LoadU32(base, found);
    if (i == 8) {
      h.set_swapped_texture(0);
    }
    guest::StoreU32(base, kMovies[i].field, movie);
  }
  return found;
}

uint64_t Make1004(uint8_t* base, uint32_t owner, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82216A60 RaceScreens::Make1004");
  const uint32_t frame = sp - 128;
  guest::StoreU32(base, frame, sp);
  uint32_t object = uint32_t(calls.Call(kNew, frame, 0x82216A78, 264));
  if (object != 0) {
    calls.Call(kMemset, frame, 0x82216A94, object, 0, 264);
  }
  const schema::AltHudHolder holder(base, owner);
  holder.set_object(object);
  const uint32_t list = uint32_t(calls.Call(kNew, frame, 0x82216AA8, 72));
  if (list != 0) {
    const schema::MenuList menu(base, list);
    menu.set_enabled(0u);
    menu.set_player(0);
  }
  holder.set_menu_list(list);
  guest::StoreU32(base, frame + 84, 159);
  guest::StoreU32(base, frame + 92, 158);
  const uint64_t r3 =
      calls.CallArgs(kMenuListInit, frame, 0x82216AF4, {list, 1, 1, 0, 154, 153, 155, 156});
  const schema::AltHud alt = holder.object();
  alt.set_shown(0);
  alt.set_big_view(0);
  return r3;
}

uint64_t ScreensInit(uint8_t* base, uint32_t holder, uint32_t kind, uint32_t sp,
                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82211DA0 RaceScreens::Init");
  const uint32_t frame = sp - 128;
  guest::StoreU32(base, frame, sp);
  calls.Call(kMoviesStart, frame, 0x82211DB8, holder, kind);
  calls.Call(kStringsLoad, frame, 0x82211DC8, G::strings(base).address, 47);
  const uint32_t mask = MovieMask(kind);
  for (uint32_t i = 0; i < 23; i++) {
    if ((mask >> i) & 1) {
      calls.Call(kMoviesOpen, frame, 0x82211E04, i, guest::LoadU32(base, 0x8200D158 + 4 * i));
    }
  }
  // The seven screens' globals copied to +1012 to +1036, in order.
  using Screens = schema::RaceScreensGlobals;
  using Unseen = schema::RaceScreensUnseenGlobals;
  const schema::RaceScreens screens(base, holder);
  screens.set_pause_menu(Screens::pause_screen(base).address);
  screens.set_kind4_pause_menu(Unseen::pause4_screen(base).address);
  screens.set_after_race_menu(Screens::after_screen(base).address);
  screens.set_career_results(Screens::career_results(base).address);
  screens.set_race_results(Unseen::race_results(base).address);
  screens.set_split_results_screen(Unseen::split_results(base).address);
  screens.set_kind6_menu(Unseen::kind6_menu(base).address);
  // The race HUD at the holder's +16 (no field of RaceScreens yet).
  Init(base, holder + 16, frame);
  const uint64_t r3 = Make1004(base, screens.alt_hud_address(), frame, calls);
  screens.set_flags(0);
  return r3;
}

uint64_t ScreensDestruct(uint8_t* base, uint32_t holder, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82211B98 RaceScreens::~RaceScreens");
  const uint32_t frame = sp - 96;
  guest::StoreU32(base, frame, sp);
  TearDown(base, holder + 16, frame, calls);
  const uint32_t alt = schema::RaceScreens(base, holder).alt_hud_address();
  if (!alt_hud::Destroy(base, alt, frame, calls)) {
    calls.Call(kAltHudDestroy, frame, 0x82211BBC, alt);
  }
  // The jump reads no argument; r3 as Strings::Release leaves it (DeleteAll
  // hands it back when the movie table is empty).
  const uint64_t r3 =
      calls.Call(kStringsRelease, frame, 0x82211BCC, G::strings(base).address, 47);
  return calls.Call(kDeleteAllJump, frame, 0x82211BD0, r3);
}

uint64_t ScreensDestroy(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82211D18 RaceScreens::Destroy");
  const uint32_t frame = sp - 96;
  guest::StoreU32(base, frame, sp);
  struct Teardown {
    std::optional<uint64_t> (*native)(uint8_t*, uint32_t);
    uint32_t original, lr;
  };
  static constexpr Teardown kTeardowns[8] = {
      {race_screens::PauseMenuDestroy, 0x8220C4C8, 0x82211D2C},
      {race_screens::PauseMenu4Destroy, 0x8220FC98, 0x82211D30},
      {race_screens::AfterRaceMenuDestroy, 0x8221A3E8, 0x82211D34},
      {race_screens::CareerResultsDestroy, 0x82219170, 0x82211D38},
      {race_screens::RaceResultsDestroy, 0x8221F3C8, 0x82211D3C},
      {race_screens::SplitResultsDestroy, 0x8221FA68, 0x82211D40},
      {race_screens::RaceDialogDestroy, 0x82217CD0, 0x82211D44},
      {race_screens::Kind6MenuDestroy, 0x8221BE30, 0x82211D48},
  };
  for (const Teardown& teardown : kTeardowns) {
    if (!teardown.native(base, frame)) {
      calls.Call(teardown.original, frame, teardown.lr);
    }
  }
  ScreensDestruct(base, G::screens_holder(base).address, frame, calls);
  const uint32_t holder = G::screens_holder(base).address;
  if (holder == 0) {
    constexpr uint32_t kText = 0x8200D1C4;
    helpers::DebugOutput(nullptr);
    return uint64_t(int64_t(int32_t(kText)));
  }
  uint64_t r3;
  if (memory::IsPages(holder)) {
    r3 = memory::FreePages(holder) ? 1 : 0;
  } else {
    r3 = calls.Call(0x82295330, frame, 0x82211D70, holder, 0, 0x8000);
  }
  G::set_screens_holder(base, 0u);
  return r3;
}

uint64_t ScreensCreate(uint8_t* base, uint32_t kind, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82211F40 RaceScreens::Create");
  const uint32_t frame = sp - 112;
  guest::StoreU32(base, frame, sp);
  // VirtualAlloc(0, 1040, MEM_COMMIT, PAGE_READWRITE) over the allocation
  // layer (the adapter runs this only there, hooks::NativeHeapActive).
  G::set_screens_holder(base, memory::AllocPages(1040, 4096));
  race_screens::PauseMenuCreate(base, frame);
  race_screens::PauseMenu4Create(base, frame);
  race_screens::AfterRaceMenuCreate(base, frame);
  race_screens::CareerResultsCreate(base, frame);
  race_screens::RaceResultsCreate(base, frame);
  race_screens::SplitResultsCreate(base, frame);
  race_screens::RaceDialogCreate(base, frame);
  race_screens::Kind6MenuCreate(base, frame);
  return ScreensInit(base, G::screens_holder(base).address, kind, frame, calls);
}

uint64_t DrawPauseMenu(uint8_t* base, uint32_t holder, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82211D08 RaceScreens::DrawPauseMenu");
  const uint32_t menu = schema::RaceScreens(base, holder).pause_menu().address;
  return race_screens::PauseMenuDraw(base, menu, sp, calls);
}

uint64_t DrawAfterRaceMenu(uint8_t* base, uint32_t holder, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82211D10 RaceScreens::DrawAfterRaceMenu");
  const uint32_t menu = schema::RaceScreens(base, holder).after_race_menu().address;
  return race_screens::AfterRaceMenuDraw(base, menu, sp, calls);
}

uint64_t RunCountdown(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls,
                      const menu_sounds::Calls& sounds, double* f1) {
  ITC_NATIVE_RUNNING("sub_822129A8 RaceHud::RunCountdown");
  const uint32_t frame = sp - 112;
  guest::StoreU32(base, frame, sp);
  const uint64_t run = calls.Call(kRun, frame, 0x822129C8, Countdown(base, hud), 0);
  const uint32_t movie = Countdown(base, hud);
  Hud(base, hud).set_countdown_running(uint32_t(run));
  // UiMovie::AnimationTime leaves r3 (the movie) and its time in f1, set
  // before the sound's call as the original leaves it.
  const uint32_t time = ui_movies::AnimationTime(base, movie, kNone, 0);
  *f1 = double(std::bit_cast<float>(time));
  const int32_t frames = camera_modes::steps::Fctiwz(*f1);
  guest::StoreU32(base, frame + 80, uint32_t(frames));
  uint32_t cue;
  if (frames == 60 || frames == 120 || frames == 180) {
    cue = 8;
  } else if (frames == 240) {
    cue = 9;
  } else {
    return movie;
  }
  return menu_sounds::PlayMenuSound(base, cue, frame, sounds);
}

uint64_t RunWarning(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls,
                    const menu_sounds::Calls& sounds) {
  ITC_NATIVE_RUNNING("sub_82212A48 RaceHud::RunWarning");
  // The frame is made before the flag's test.
  const uint32_t frame = sp - 112;
  guest::StoreU32(base, frame, sp);
  if (Hud(base, hud).warning_shown() == 0) {
    return hud;
  }
  // Sound 10 unless the sound manager's slot 14 (AudioManager::IsCuePlaying:
  // bank 0, cue 10 at the frame's +84 and +80) says it's playing.
  guest::StoreU32(base, frame + 80, 10);
  guest::StoreU32(base, frame + 84, 0);
  const schema::AudioManager manager = schema::MenuSoundsGlobals::sound_manager(base);
  const uint32_t playing = manager.vtable().is_cue_playing();
  const uint64_t r3 =
      calls.Call(playing, frame, 0x82212A98, manager.address, frame + 84, frame + 80);
  if ((r3 & 0xFF) == 0) {
    menu_sounds::PlayMenuSound(base, 10, frame, sounds);
  }
  return calls.Call(kRun, frame, 0x82212AB4, Warning(base, hud), 0);
}

uint64_t UpdateRacerBar(uint8_t* base, uint32_t hud, uint64_t index, uint64_t racer, uint32_t sp,
                        const Calls& calls, const menu_sounds::Calls& sounds) {
  ITC_NATIVE_RUNNING("sub_82214790 RaceHud::UpdateRacerBar");
  using namespace camera_modes::steps;
  const uint32_t frame = sp - 176;
  guest::StoreU32(base, frame, sp);
  const Hud h(base, hud);
  const schema::RaceHudBar bar(base, uint32_t(index * 40) + h.bars_address());
  const uint32_t racers = h.racers().address;
  const uint32_t target = Racers(base, uint32_t(racer * 972) + racers).bar_max0();
  if (bar.touched() == 0 && Touching(base, uint32_t(racer), frame) != 0) {
    const double value = double(bar.value());
    bar.set_touched(1);
    bar.set_trail(float(value));
  }
  BarStep(base, bar.address, int32_t(target), frame);
  // The group (r4 + 1) and its children 1, 3, 2 and 4, in the original's
  // order.
  const uint32_t group = ui_movies::FindNode(base, RacersMovie(base, hud), kNone,
                                             uint32_t(index + 1));
  auto child = [&](uint32_t k) {
    return ui_movies::FindNode(base, RacersMovie(base, hud), kNone,
                               guest::LoadU32(base, group + 100) + k);
  };
  const uint32_t n1 = child(1);
  const uint32_t n3 = child(3);
  const uint32_t n2 = child(2);
  const uint32_t n4 = child(4);
  uint64_t r3 = n4;
  const bool first = int32_t(uint32_t(index)) == 0;
  const double goal = double(bar.target());
  if (!(goal > double(bar.min()))) {
    guest::StoreU32(base, n1 + 80, 0);
    guest::StoreU32(base, n3 + 80, 0);
    guest::StoreU32(base, n4 + 80, 0);
    if (bar.animation_played() != 0) {
      r3 = calls.Call(kFinish, frame, 0x822148A8, RacersMovie(base, hud), ~0ull, index);
    }
  } else {
    // The double at 0x8200F738 times the maximum (fmul, not rounded).
    const uint64_t bits =
        uint64_t(guest::LoadU32(base, 0x8200F738)) << 32 | guest::LoadU32(base, 0x8200F73C);
    const double low = double(bar.max()) * std::bit_cast<double>(bits);
    if (!(goal > low)) {
      guest::StoreU32(base, n1 + 80, 0);
      guest::StoreU32(base, n3 + 80, 0);
      guest::StoreU32(base, n4 + 80, 1);
      h.set_any_bar_low(1);
      if (bar.animation_played() == 0) {
        bar.set_animation_played(1);
        ui_movies::Play(base, RacersMovie(base, hud), kNone, uint32_t(index), 0, 0, 0, 0);
        const uint32_t movie = RacersMovie(base, hud);
        r3 = MovedR3(ui_movies::MoveAnimation(base, movie, kNone, uint32_t(index), true), frame,
                     movie);
      }
      if (first) {
        // Sound 7 unless the sound manager's slot 14 (AudioManager::
        // IsCuePlaying: bank 0, cue 7 at the frame's +84 and +80) says it's
        // playing.
        guest::StoreU32(base, frame + 84, 0);
        guest::StoreU32(base, frame + 80, 7);
        const schema::AudioManager manager = schema::MenuSoundsGlobals::sound_manager(base);
        const uint32_t playing = manager.vtable().is_cue_playing();
        r3 = calls.Call(playing, frame, 0x82214948, manager.address, frame + 84, frame + 80);
        if ((r3 & 0xFF) == 0) {
          r3 = menu_sounds::PlayMenuSound(base, 7, frame, sounds);
        }
      }
    } else {
      guest::StoreU32(base, n1 + 80, bar.rising() != 0 ? 0 : 1);
      guest::StoreU32(base, n3 + 80, 1);
      guest::StoreU32(base, n4 + 80, 0);
    }
  }
  guest::StoreU32(base, n2 + 80, int32_t(bar.touched()) == 1 ? 1 : 0);
  // The four nodes' x scales (+28): the value's fraction of the range (n3,
  // n1, n4) and the trail's lead over it (n2), each loaded again.
  auto range = [&]() { return Fsubs(double(bar.max()), double(bar.min())); };
  Stfs(base, n3 + 28, Fdivs(double(bar.value()), range()));
  Stfs(base, n1 + 28, Fdivs(double(bar.value()), range()));
  {
    const double lead = Fsubs(double(bar.trail()), double(bar.value()));
    Stfs(base, n2 + 28, Fdivs(lead, range()));
  }
  Stfs(base, n4 + 28, Fdivs(double(bar.value()), range()));
  // Node 2's x (+12): after node 1 by the float at 0x8200F730 times its
  // scale (bar 0: 1.0 less the trail's fraction).
  const double width = Lfs(base, 0x8200F730);
  double x;
  if (first) {
    const double scale = Fsubs(Lfs(base, 0x82000A30), Fdivs(double(bar.trail()), range()));
    x = Fmadds(scale, width, Lfs(base, n1 + 12));
  } else {
    x = Fmadds(Lfs(base, n1 + 28), width, Lfs(base, n1 + 12));
  }
  Stfs(base, n2 + 12, x);
  return r3;
}

}  // namespace race_hud_object
