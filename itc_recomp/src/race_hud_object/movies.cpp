// The race HUD object's movie calls and its set-up
// (docs/functions/race-hud-object.md): see race_hud_object.h.

#include "race_hud_object/race_hud_object.h"

#include "alt_hud/alt_hud.h"
#include "base/log.h"
#include "camera_modes/steps.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "helpers/debug_output.h"
#include "memory/handles.h"
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
uint32_t RacersMovie(uint32_t hud) { return Hud(hud).racers_movie().address; }
uint32_t Dashboard(uint32_t hud) { return Hud(hud).dashboard().address; }
uint32_t Countdown(uint32_t hud) { return Hud(hud).countdown().address; }
uint32_t Warning(uint32_t hud) { return Hud(hud).warning().address; }
uint32_t Timer(uint32_t hud) { return Hud(hud).timer().address; }
uint32_t Movie1(uint32_t hud) { return Hud(hud).movie1().address; }

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

uint32_t StartCountdown(uint32_t hud) {
  ITC_NATIVE_RUNNING("sub_82212270 RaceHud::StartCountdown");
  const uint32_t movie = Countdown(hud);
  Hud(hud).set_countdown_running(1);
  return ui_movies::Play(movie, kNone, 0, 0, 0, 0, 0);
}

uint64_t StartMovie1(uint32_t hud, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82212718 RaceHud::StartMovie1");
  // The original's frame, for MovedR3's address only (no guest code is
  // called from it, so no back chain).
  const uint32_t frame = sp - 96;
  ui_movies::PlayNamed(Movie1(hud), 0, 0, 0, 0, 0);
  const uint32_t movie = Movie1(hud);
  return MovedR3(ui_movies::MoveNamedAnimation(movie, 0, true), frame, movie);
}

uint64_t FinishMovie1(uint32_t hud, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82212878 RaceHud::FinishMovie1");
  return calls.Call(kFinishNamed, sp, lr, Movie1(hud), 0);
}

uint64_t DrawWarning(uint32_t hud, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82212AC8 RaceHud::DrawWarning");
  if (Hud(hud).warning_shown() == 0) {
    return hud;
  }
  return calls.Call(kRun, sp, lr, Warning(hud), 1);
}

uint64_t RunMovie1(uint32_t hud, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82212AF8 RaceHud::RunMovie1");
  return calls.Call(kRun, sp, lr, Movie1(hud), 0);
}

uint64_t RunItem(uint32_t hud, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82212B08 RaceHud::RunItem");
  const uint32_t frame = sp - 96;
  guest::StoreU32(frame, sp);
  if (int32_t(Hud(hud).item_shown()) == -1) {
    return hud;
  }
  calls.Call(kRun, frame, 0x82212B34, RacersMovie(hud), 0);
  // sub_82106540(200, 500, 0, 0x8200F710, +460): empty (audio.md), so
  // nothing but its r3, the 200 it was handed.
  return 200;
}

uint64_t Draw(uint32_t hud, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822128A8 RaceHud::Draw");
  const uint32_t frame = sp - 96;
  guest::StoreU32(frame, sp);
  uint64_t r3 = calls.Call(kRun, frame, 0x822128C8, Dashboard(hud), 0);
  const Hud h(hud);
  if (h.view_dashboard() == 0 || h.option_mirror() == 0) {
    return r3;
  }
  const uint32_t mirror = ui_movies::FindNode(Dashboard(hud), kNone, 19);
  return calls.Call(kDrawMirror, frame, 0x822128F4, mirror);
}

uint64_t UpdateWarning(uint32_t hud, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82212C50 RaceHud::UpdateWarning");
  const uint32_t frame = sp - 96;
  guest::StoreU32(frame, sp);
  const Hud h(hud);
  const schema::Vehicle car = h.vehicle();
  const uint32_t wrong_way = schema::VehicleStatus(car.status_address()).wrong_way();
  if (wrong_way == 0) {
    if (h.warning_shown() == 0) {
      return hud;
    }
    const uint64_t r3 =
        calls.Call(kFinish, frame, 0x82212C90, Warning(hud), kNone, 0);
    h.set_warning_shown(0);
    return r3;
  }
  if (h.warning_shown() != 0) {
    return hud;
  }
  calls.Call(kFinish, frame, 0x82212CBC, Warning(hud), kNone, 0);
  ui_movies::Play(Warning(hud), kNone, 0, 0, 0, 0, 0);
  const uint32_t movie = Warning(hud);
  const uint64_t r3 =
      MovedR3(ui_movies::MoveAnimation(movie, kNone, 0, true), frame, movie);
  h.set_warning_shown(1);
  return r3;
}

uint64_t UpdateRecordingFull(uint32_t hud, int32_t room, uint32_t sp,
                             const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82212E60 RaceHud::UpdateRecordingFull");
  const uint32_t frame = sp - 96;
  guest::StoreU32(frame, sp);
  const Hud h(hud);
  if (room == 0) {
    if (h.recording_full() != 0) {
      return hud;
    }
    const uint32_t dashboard = Dashboard(hud);
    h.set_recording_full(1);
    calls.Call(kFinishNamed, frame, 0x82212E9C, dashboard, 6);
    return ui_movies::PlayNamed(Dashboard(hud), 6, 0, 0, 0, 0);
  }
  if (room != 1) {
    return hud;
  }
  const uint32_t dashboard = Dashboard(hud);
  h.set_recording_full(0);
  const uint32_t node = ui_movies::FindNode(dashboard, kNone, 5);
  camera_modes::steps::Stfs(node + 68, camera_modes::steps::Lfs(kZero));
  return node;
}

uint64_t ShowItem(uint32_t hud, int32_t id, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82213638 RaceHud::ShowItem");
  const uint32_t frame = sp - 128;
  guest::StoreU32(frame, sp);
  constexpr uint32_t kTagPrm = 0x50524D00;  // "PRM\0"
  constexpr uint32_t kTagPic = 0x50494300;  // "PIC\0"
  constexpr uint32_t kTagTxt = 0x54585400;  // "TXT\0"
  const uint32_t shown = id == -1 ? 1 : 0;
  uint32_t chunk = schema::UiMovie(RacersMovie(hud)).live_scene();
  while (schema::UiMovieChunk(chunk).tag() != ui_movies::kTagTrm) {
    const uint32_t tag = schema::UiMovieChunk(chunk).tag();
    if (tag == kTagPrm || tag == kTagPic || tag == kTagTxt) {
      guest::StoreU32(chunk + 80, shown);
    }
    chunk += schema::UiMovieChunk(chunk).next();
  }
  if (id == -1) {
    return hud;
  }
  const uint32_t name = ui_movies::FindNode(RacersMovie(hud), kNone, 19);
  const uint32_t picture = ui_movies::FindNode(RacersMovie(hud), 5, 0);
  guest::StoreU32(name + 80, 1);
  const uint32_t panel = ui_movies::FindNode(RacersMovie(hud), kNone, 5);
  guest::StoreU32(panel + 80, 1);
  guest::StoreU32(picture + 80, 1);
  // UiMovie::SetNodeText's r6 to r10 (varargs) passed 0: the original leaves
  // them unset (the user's call for the parking area, 2026-10-30).
  calls.Call(kSetNodeText, frame, 0x82213724, RacersMovie(hud), name,
             uint64_t(int64_t(id) + 4650));
  const uint32_t item = data_tables::Item(uint32_t(id));
  const int32_t kind = int8_t(schema::OpponentItem(item).category());
  guest::StoreU32(picture + 108, uint32_t(kind < 48 ? kind : id - 262));
  return item;
}

uint64_t UpdateItem(uint32_t hud, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82214238 RaceHud::UpdateItem");
  const Hud h(hud);
  const uint32_t item = G::race_progress().item_shown();
  if (h.item_shown() == item) {
    return hud;
  }
  h.set_item_shown(item);
  return ShowItem(hud, int32_t(item), sp, calls);
}

namespace {

// The HUD's item picture: DataTables::Item(id)'s byte +34 (signed), or for
// 48 and up id - 262 (the originals' word from the zero-extended id).
uint32_t ItemPicture(uint32_t id) {
  const int32_t kind = int8_t(schema::OpponentItem(data_tables::Item(id)).category());
  return kind < 48 ? uint32_t(kind) : id - 262;
}

constexpr uint64_t kAll = ~0ull;  // a sub-scene of -1 in a guest call's register

}  // namespace

uint64_t SetUpTimer(uint32_t hud, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822123C8 RaceHud::SetUpTimer");
  const uint32_t frame = sp - 144;
  guest::StoreU32(frame, sp);
  const Hud h(hud);
  const Racers racers = h.racers();
  const uint32_t animation = racers.timer_animation();
  const uint32_t count = racers.count();
  h.set_timer_animation(animation);
  if (int32_t(animation) >= 0) {
    for (uint32_t i = 0; i < 5; i++) {
      calls.Call(kFinishNamed, frame, 0x82212408, Timer(hud), i);
    }
    const uint32_t node = ui_movies::FindNode(Timer(hud), kNone, 0);
    guest::StoreU32(node + 80, 1);
    ui_movies::PlayNamed(Timer(hud), h.timer_animation(), 0, 0, 0, 0);
    ui_movies::MoveNamedAnimation(Timer(hud), h.timer_animation(), true);
  } else {
    const uint32_t node = ui_movies::FindNode(Timer(hud), kNone, 0);
    guest::StoreU32(node + 80, 0);
  }
  const uint32_t group = ui_movies::FindNode(Timer(hud), kNone, 3);
  for (int32_t k = 0; k < int32_t(guest::LoadU32(group + 104)); k++) {
    const uint32_t node = ui_movies::FindNode(Timer(hud), kNone,
                                              guest::LoadU32(group + 100) + uint32_t(k));
    guest::StoreU32(node + 80, 0);
  }
  // Record count - 1's item (records 972 bytes from the racers' +0).
  const uint32_t last = guest::LoadU32(h.racers().address + count * 972 - 964);
  if (int32_t(last) != -1) {
    const uint32_t child = guest::LoadU32(group + 100) + count - 2;
    const uint32_t shown = ui_movies::FindNode(Timer(hud), kNone, child);
    guest::StoreU32(shown + 80, 1);
    const uint32_t picture = ui_movies::FindNode(Timer(hud),
                                                 guest::LoadU32(group + 100) + count - 2, 0);
    guest::StoreU32(picture + 108, ItemPicture(last));
  }
  for (uint32_t i = 1; i < 4; i++) {
    if (int32_t(i) < int32_t(count + 1)) {
      const uint32_t id = guest::LoadU32(h.racers().item0_address() + 972 * i);
      if (int32_t(id) != -1) {
        // SetText's varargs (r7 to r10) passed 0: the original leaves them
        // unset (the user's call for the parking area, 2026-10-30).
        calls.Call(kSetText, frame, 0x82212568, Timer(hud), kAll, i + 26,
                   uint64_t(id) + 4650);
      }
    } else {
      calls.Call(kClearText, frame, 0x8221257C, Timer(hud), kAll, i + 26);
    }
  }
  const uint32_t save = vehicle_accessors::SaveDataOf(0);
  return calls.Call(kSetChildNameText, frame, 0x822125AC, Timer(hud), kAll, 26,
                    uint64_t(save) + 8);
}

uint64_t SetUpRacers(uint32_t hud, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82213F20 RaceHud::SetUpRacers");
  using namespace camera_modes::steps;
  const uint32_t frame = sp - 176;
  guest::StoreU32(frame, sp);
  ShowItem(hud, -1, frame, calls);
  const Hud h(hud);
  const uint32_t count = h.racers().count();
  h.set_racer_count(count);
  uint32_t group = ui_movies::FindNode(RacersMovie(hud), kNone, 0);
  {
    uint32_t k = guest::LoadU32(group + 100);
    uint32_t end = guest::LoadU32(group + 104) + k;
    while (int32_t(k) < int32_t(end)) {
      const uint32_t node = ui_movies::FindNode(RacersMovie(hud), kNone, k);
      guest::StoreU32(node + 80, 0);
      k++;
      end = guest::LoadU32(group + 100) + guest::LoadU32(group + 104);
    }
  }
  const uint32_t racers = h.racers().address;
  const uint32_t last = guest::LoadU32(racers + h.racer_count() * 972 - 964);
  if (int32_t(last) != -1) {
    const uint32_t shown = ui_movies::FindNode(RacersMovie(hud), kNone,
                                               guest::LoadU32(group + 100) + count - 2);
    guest::StoreU32(shown + 80, 1);
    const uint32_t picture = ui_movies::FindNode(RacersMovie(hud),
                                                 guest::LoadU32(group + 100) + count - 2, 0);
    guest::StoreU32(picture + 108, ItemPicture(last));
  }
  for (uint32_t k = 1; k < 3; k++) {
    const uint32_t node = ui_movies::FindNode(RacersMovie(hud), kNone, k + 2);
    guest::StoreU32(node + 80, 1);
  }
  if (int32_t(count - 1) < 3) {
    for (uint32_t k = count + 1; int32_t(k) < 5; k++) {
      const uint32_t node = ui_movies::FindNode(RacersMovie(hud), kNone, k);
      guest::StoreU32(node + 80, 0);
    }
  }
  if (int32_t(count) > 0) {
    // The constants, loaded once before the loop as the original does.
    const double zero = Lfs(kZero);
    const double scale4 = Lfs(0x82002A60);
    const double scale12 = Lfs(0x8200F724);
    for (uint32_t i = 0; int32_t(i) < int32_t(count); i++) {
      group = ui_movies::FindNode(RacersMovie(hud), kNone, i + 1);
      if (i == 0) {
        const uint32_t save = vehicle_accessors::SaveDataOf(0);
        calls.Call(kSetChildNameText, frame, 0x822140D4, RacersMovie(hud), kAll,
                   uint64_t(guest::LoadU32(group + 100)) + 5, uint64_t(save) + 8);
      } else {
        const uint32_t id = guest::LoadU32(h.racers().item0_address() + 972 * i);
        if (int32_t(id) != -1) {
          // SetText's varargs passed 0 (the user's call, 2026-10-30).
          calls.Call(kSetText, frame, 0x82214108, RacersMovie(hud), kAll,
                     uint64_t(guest::LoadU32(group + 100)) + 5, uint64_t(id) + 4650);
        }
      }
      const uint32_t record = h.racers().address + 972 * i;
      const schema::RaceHudBar bar(h.bars_address() + 40 * i);
      const int64_t target = int32_t(Racers(record).bar_max0());
      bar.set_min(float(zero));
      bar.set_value(float(zero));
      bar.set_animation_played(0);
      bar.set_touched(0);
      // The conversion's doubleword (the original's std to frame + 80, read
      // by nothing else) left out.
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
    r3 = calls.Call(kFinish, frame, 0x8221417C, RacersMovie(hud), kAll, i);
  }
  h.set_any_bar_low(0);
  return r3;
}

uint64_t SetMode(uint32_t hud, uint64_t mode, uint32_t sp, uint32_t lr,
                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822157A8 RaceHud::SetMode");
  Hud(hud).set_mode(uint32_t(mode));
  switch (uint32_t(mode)) {
    case 0:
      return SetUpRacers(hud, sp, calls);
    case 1:
      return SetUpTimer(hud, sp, calls);
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

uint32_t Init(uint32_t hud, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82215260 RaceHud::Init");
  // The original's frame (its + 80 the key handed to UiMovie::PairsFind)
  // left out: the key passed by value.
  const Hud h(hud);
  // The HUD's movie fields, set through the view (a host object, E5 step 4).
  using Set = void (Hud::*)(uint32_t) const;
  static constexpr struct {
    uint32_t id;
    Set set;
  } kMovies[9] = {{10, &Hud::set_racers_movie}, {0, &Hud::set_dashboard},
                  {11, &Hud::set_countdown},    {3, &Hud::set_warning},
                  {2, &Hud::set_movie2},        {13, &Hud::set_timer},
                  {1, &Hud::set_movie1},        {12, &Hud::set_splits},
                  {19, &Hud::set_movie19}};
  uint32_t found = 0;
  for (uint32_t i = 0; i < 9; i++) {
    found = ui_movies::PairsFindKey(ui_movies::kMovieTable, kMovies[i].id);
    const uint32_t movie = guest::LoadU32(found);
    if (i == 8) {
      h.set_swapped_texture(0);
    }
    (h.*kMovies[i].set)(movie);
  }
  return found;
}

uint64_t Make1004(uint32_t owner, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82216A60 RaceScreens::Make1004");
  const uint32_t frame = sp - 128;
  guest::StoreU32(frame, sp);
  // operator new(264) at 0x82216A78: the object in host memory (E5 step 4).
  uint32_t object = schema::AltHud::New().address;
  if (object != 0) {
    calls.Call(kMemset, frame, 0x82216A94, object, 0, 264);
  }
  const schema::AltHudHolder holder(owner);
  holder.set_object(object);
  // A host object (E5 step 5) in place of the operator new(72) at
  // 0x82216AA8; the holder's +1008 keeps its handle.
  const uint32_t list = schema::MenuList::New().address;
  if (list != 0) {
    const schema::MenuList menu(list);
    menu.set_enabled(0u);
    menu.set_player(0);
  }
  holder.set_menu_list(list);
  guest::StoreU32(frame + 84, 159);
  guest::StoreU32(frame + 92, 158);
  const uint64_t r3 =
      calls.CallArgs(kMenuListInit, frame, 0x82216AF4, {list, 1, 1, 0, 154, 153, 155, 156});
  const schema::AltHud alt = holder.object();
  alt.set_shown(0);
  alt.set_big_view(0);
  return r3;
}

uint64_t ScreensInit(uint32_t holder, uint32_t kind, uint32_t sp,
                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82211DA0 RaceScreens::Init");
  const uint32_t frame = sp - 128;
  guest::StoreU32(frame, sp);
  calls.Call(kMoviesStart, frame, 0x82211DB8, holder, kind);
  calls.Call(kStringsLoad, frame, 0x82211DC8, G::strings().address, 47);
  const uint32_t mask = MovieMask(kind);
  for (uint32_t i = 0; i < 23; i++) {
    if ((mask >> i) & 1) {
      calls.Call(kMoviesOpen, frame, 0x82211E04, i, guest::LoadU32(0x8200D158 + 4 * i));
    }
  }
  // The seven screens' globals copied to +1012 to +1036, in order.
  using Screens = schema::RaceScreensGlobals;
  using Unseen = schema::RaceScreensUnseenGlobals;
  const schema::RaceScreens screens(holder);
  screens.set_pause_menu(Screens::pause_screen().address);
  screens.set_kind4_pause_menu(Unseen::pause4_screen().address);
  screens.set_after_race_menu(Screens::after_screen().address);
  screens.set_career_results(Screens::career_results().address);
  screens.set_race_results(Unseen::race_results().address);
  screens.set_split_results_screen(Unseen::split_results().address);
  screens.set_kind6_menu(Unseen::kind6_menu().address);
  // The race HUD at the holder's +16.
  Init(screens.hud().address, frame);
  const uint64_t r3 = Make1004(screens.alt_hud_holder().address, frame, calls);
  screens.set_flags(0);
  return r3;
}

uint64_t ScreensDestruct(uint32_t holder, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82211B98 RaceScreens::~RaceScreens");
  const uint32_t frame = sp - 96;
  guest::StoreU32(frame, sp);
  TearDown(schema::RaceScreens(holder).hud().address, frame, calls);
  const uint32_t alt = schema::RaceScreens(holder).alt_hud_holder().address;
  if (!alt_hud::Destroy(alt, frame, calls)) {
    calls.Call(kAltHudDestroy, frame, 0x82211BBC, alt);
  }
  // The jump reads no argument; r3 as Strings::Release leaves it (DeleteAll
  // hands it back when the movie table is empty).
  const uint64_t r3 =
      calls.Call(kStringsRelease, frame, 0x82211BCC, G::strings().address, 47);
  return calls.Call(kDeleteAllJump, frame, 0x82211BD0, r3);
}

uint64_t ScreensDestroy(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82211D18 RaceScreens::Destroy");
  const uint32_t frame = sp - 96;
  guest::StoreU32(frame, sp);
  struct Teardown {
    std::optional<uint64_t> (*native)(uint32_t);
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
    if (!teardown.native(frame)) {
      calls.Call(teardown.original, frame, teardown.lr);
    }
  }
  ScreensDestruct(G::screens_holder().address, frame, calls);
  const uint32_t holder = G::screens_holder().address;
  if (holder == 0) {
    constexpr uint32_t kText = 0x8200D1C4;
    helpers::DebugOutput(nullptr);
    return uint64_t(int64_t(int32_t(kText)));
  }
  uint64_t r3;
  if (memory::IsHandle(holder)) {
    // The holder in host memory (E5 step 4): deleted in place of the
    // original's VirtualFree at 0x82211D70, which returned 1.
    schema::RaceScreens(holder).Delete();
    r3 = 1;
  } else if (memory::IsPages(holder)) {
    r3 = memory::FreePages(holder) ? 1 : 0;
  } else {
    r3 = calls.Call(0x82295330, frame, 0x82211D70, holder, 0, 0x8000);
  }
  G::set_screens_holder(0u);
  return r3;
}

uint64_t ScreensCreate(uint32_t kind, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82211F40 RaceScreens::Create");
  // The original's frame: its callees' stack pointer (they make their own
  // frames below it; no guest code is called from it, so no back chain).
  const uint32_t frame = sp - 112;
  // VirtualAlloc(0, 1040, MEM_COMMIT, PAGE_READWRITE): the holder in host
  // memory (E5 step 4; zeroed, as committed pages are), its handle kept at
  // [0x826D84F0]. The adapter runs this only over the allocation layer
  // (hooks::NativeHeapActive).
  G::set_screens_holder(schema::RaceScreens::New().address);
  race_screens::PauseMenuCreate(frame);
  race_screens::PauseMenu4Create(frame);
  race_screens::AfterRaceMenuCreate(frame);
  race_screens::CareerResultsCreate(frame);
  race_screens::RaceResultsCreate(frame);
  race_screens::SplitResultsCreate(frame);
  race_screens::RaceDialogCreate(frame);
  race_screens::Kind6MenuCreate(frame);
  return ScreensInit(G::screens_holder().address, kind, frame, calls);
}

uint64_t DrawPauseMenu(uint32_t holder, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82211D08 RaceScreens::DrawPauseMenu");
  const uint32_t menu = schema::RaceScreens(holder).pause_menu().address;
  return race_screens::PauseMenuDraw(menu, sp, calls);
}

uint64_t DrawAfterRaceMenu(uint32_t holder, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82211D10 RaceScreens::DrawAfterRaceMenu");
  const uint32_t menu = schema::RaceScreens(holder).after_race_menu().address;
  return race_screens::AfterRaceMenuDraw(menu, sp, calls);
}

uint64_t RunCountdown(uint32_t hud, uint32_t sp, const Calls& calls,
                      const menu_sounds::Calls& sounds, double* f1) {
  ITC_NATIVE_RUNNING("sub_822129A8 RaceHud::RunCountdown");
  const uint32_t frame = sp - 112;
  guest::StoreU32(frame, sp);
  const uint64_t run = calls.Call(kRun, frame, 0x822129C8, Countdown(hud), 0);
  const uint32_t movie = Countdown(hud);
  Hud(hud).set_countdown_running(uint32_t(run));
  // UiMovie::AnimationTime leaves r3 (the movie) and its time in f1, set
  // before the sound's call as the original leaves it.
  const uint32_t time = ui_movies::AnimationTime(movie, kNone, 0);
  *f1 = double(std::bit_cast<float>(time));
  // The original's frame + 80 (the fctiwz's word, read back) a local.
  const int32_t frames = camera_modes::steps::Fctiwz(*f1);
  uint32_t cue;
  if (frames == 60 || frames == 120 || frames == 180) {
    cue = 8;
  } else if (frames == 240) {
    cue = 9;
  } else {
    return movie;
  }
  return menu_sounds::PlayMenuSound(cue, frame, sounds);
}

uint64_t RunWarning(uint32_t hud, uint32_t sp, const Calls& calls,
                    const menu_sounds::Calls& sounds) {
  ITC_NATIVE_RUNNING("sub_82212A48 RaceHud::RunWarning");
  // The frame is made before the flag's test.
  const uint32_t frame = sp - 112;
  guest::StoreU32(frame, sp);
  if (Hud(hud).warning_shown() == 0) {
    return hud;
  }
  // Sound 10 unless the sound manager's slot 14 (AudioManager::IsCuePlaying:
  // bank 0, cue 10 at the frame's +84 and +80) says it's playing.
  guest::StoreU32(frame + 80, 10);
  guest::StoreU32(frame + 84, 0);
  const schema::AudioManager manager = schema::MenuSoundsGlobals::sound_manager();
  const uint32_t playing = manager.vtable().is_cue_playing();
  const uint64_t r3 =
      calls.Call(playing, frame, 0x82212A98, manager.address, frame + 84, frame + 80);
  if ((r3 & 0xFF) == 0) {
    menu_sounds::PlayMenuSound(10, frame, sounds);
  }
  return calls.Call(kRun, frame, 0x82212AB4, Warning(hud), 0);
}

uint64_t UpdateRacerBar(uint32_t hud, uint64_t index, uint64_t racer, uint32_t sp,
                        const Calls& calls, const menu_sounds::Calls& sounds) {
  ITC_NATIVE_RUNNING("sub_82214790 RaceHud::UpdateRacerBar");
  using namespace camera_modes::steps;
  const uint32_t frame = sp - 176;
  guest::StoreU32(frame, sp);
  const Hud h(hud);
  const schema::RaceHudBar bar{uint32_t(index * 40) + h.bars_address()};
  const uint32_t racers = h.racers().address;
  const uint32_t target = Racers(uint32_t(racer * 972) + racers).bar_max0();
  if (bar.touched() == 0 && Touching(uint32_t(racer), frame) != 0) {
    const double value = double(bar.value());
    bar.set_touched(1);
    bar.set_trail(float(value));
  }
  BarStep(bar.address, int32_t(target), frame);
  // The group (r4 + 1) and its children 1, 3, 2 and 4, in the original's
  // order.
  const uint32_t group = ui_movies::FindNode(RacersMovie(hud), kNone,
                                             uint32_t(index + 1));
  auto child = [&](uint32_t k) {
    return ui_movies::FindNode(RacersMovie(hud), kNone,
                               guest::LoadU32(group + 100) + k);
  };
  const uint32_t n1 = child(1);
  const uint32_t n3 = child(3);
  const uint32_t n2 = child(2);
  const uint32_t n4 = child(4);
  uint64_t r3 = n4;
  const bool first = int32_t(uint32_t(index)) == 0;
  const double goal = double(bar.target());
  if (!(goal > double(bar.min()))) {
    guest::StoreU32(n1 + 80, 0);
    guest::StoreU32(n3 + 80, 0);
    guest::StoreU32(n4 + 80, 0);
    if (bar.animation_played() != 0) {
      r3 = calls.Call(kFinish, frame, 0x822148A8, RacersMovie(hud), ~0ull, index);
    }
  } else {
    // The double at 0x8200F738 times the maximum (fmul, not rounded).
    const uint64_t bits =
        uint64_t(guest::LoadU32(0x8200F738)) << 32 | guest::LoadU32(0x8200F73C);
    const double low = double(bar.max()) * std::bit_cast<double>(bits);
    if (!(goal > low)) {
      guest::StoreU32(n1 + 80, 0);
      guest::StoreU32(n3 + 80, 0);
      guest::StoreU32(n4 + 80, 1);
      h.set_any_bar_low(1);
      if (bar.animation_played() == 0) {
        bar.set_animation_played(1);
        ui_movies::Play(RacersMovie(hud), kNone, uint32_t(index), 0, 0, 0, 0);
        const uint32_t movie = RacersMovie(hud);
        r3 = MovedR3(ui_movies::MoveAnimation(movie, kNone, uint32_t(index), true), frame,
                     movie);
      }
      if (first) {
        // Sound 7 unless the sound manager's slot 14 (AudioManager::
        // IsCuePlaying: bank 0, cue 7 at the frame's +84 and +80) says it's
        // playing.
        guest::StoreU32(frame + 84, 0);
        guest::StoreU32(frame + 80, 7);
        const schema::AudioManager manager = schema::MenuSoundsGlobals::sound_manager();
        const uint32_t playing = manager.vtable().is_cue_playing();
        r3 = calls.Call(playing, frame, 0x82214948, manager.address, frame + 84, frame + 80);
        if ((r3 & 0xFF) == 0) {
          r3 = menu_sounds::PlayMenuSound(7, frame, sounds);
        }
      }
    } else {
      guest::StoreU32(n1 + 80, bar.rising() != 0 ? 0 : 1);
      guest::StoreU32(n3 + 80, 1);
      guest::StoreU32(n4 + 80, 0);
    }
  }
  guest::StoreU32(n2 + 80, int32_t(bar.touched()) == 1 ? 1 : 0);
  // The four nodes' x scales (+28): the value's fraction of the range (n3,
  // n1, n4) and the trail's lead over it (n2), each loaded again.
  auto range = [&]() { return Fsubs(double(bar.max()), double(bar.min())); };
  Stfs(n3 + 28, Fdivs(double(bar.value()), range()));
  Stfs(n1 + 28, Fdivs(double(bar.value()), range()));
  {
    const double lead = Fsubs(double(bar.trail()), double(bar.value()));
    Stfs(n2 + 28, Fdivs(lead, range()));
  }
  Stfs(n4 + 28, Fdivs(double(bar.value()), range()));
  // Node 2's x (+12): after node 1 by the float at 0x8200F730 times its
  // scale (bar 0: 1.0 less the trail's fraction).
  const double width = Lfs(0x8200F730);
  double x;
  if (first) {
    const double scale = Fsubs(Lfs(0x82000A30), Fdivs(double(bar.trail()), range()));
    x = Fmadds(scale, width, Lfs(n1 + 12));
  } else {
    x = Fmadds(Lfs(n1 + 28), width, Lfs(n1 + 12));
  }
  Stfs(n2 + 12, x);
  return r3;
}

}  // namespace race_hud_object
