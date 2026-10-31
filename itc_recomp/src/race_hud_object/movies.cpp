// The race HUD object's movie calls and its set-up
// (docs/functions/race-hud-object.md): see race_hud_object.h.

#include "race_hud_object/race_hud_object.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace race_hud_object {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;  // a sub-scene index of -1

// The HUD's other movies.
constexpr uint32_t kHudRacers = 604;     // the racers' gauges (id 10)
constexpr uint32_t kHudCountdown = 612;  // the start countdown (id 11)
constexpr uint32_t kHudWarning = 616;    // the warning (id 3)
constexpr uint32_t kHudMovie1 = 628;     // the movie of id 1

uint32_t Movie(uint8_t* base, uint32_t hud, uint32_t field) {
  return guest::LoadU32(base, hud + field);
}

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
  const uint32_t movie = Movie(base, hud, kHudCountdown);
  guest::StoreU32(base, hud + 436, 1);
  return ui_movies::Play(base, movie, kNone, 0, 0, 0, 0, 0);
}

uint64_t StartMovie1(uint8_t* base, uint32_t hud, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82212718 RaceHud::StartMovie1");
  const uint32_t frame = sp - 96;
  guest::StoreU32(base, frame, sp);
  ui_movies::PlayNamed(base, Movie(base, hud, kHudMovie1), 0, 0, 0, 0, 0);
  const uint32_t movie = Movie(base, hud, kHudMovie1);
  return MovedR3(ui_movies::MoveNamedAnimation(base, movie, 0, true), frame, movie);
}

uint64_t FinishMovie1(uint8_t* base, uint32_t hud, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82212878 RaceHud::FinishMovie1");
  return calls.Call(kFinishNamed, sp, lr, Movie(base, hud, kHudMovie1), 0);
}

uint64_t DrawWarning(uint8_t* base, uint32_t hud, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82212AC8 RaceHud::DrawWarning");
  if (guest::LoadU32(base, hud + 468) == 0) {
    return hud;
  }
  return calls.Call(kRun, sp, lr, Movie(base, hud, kHudWarning), 1);
}

uint64_t RunMovie1(uint8_t* base, uint32_t hud, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82212AF8 RaceHud::RunMovie1");
  return calls.Call(kRun, sp, lr, Movie(base, hud, kHudMovie1), 0);
}

uint64_t RunItem(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82212B08 RaceHud::RunItem");
  const uint32_t frame = sp - 96;
  guest::StoreU32(base, frame, sp);
  if (int32_t(guest::LoadU32(base, hud + 460)) == -1) {
    return hud;
  }
  calls.Call(kRun, frame, 0x82212B34, Movie(base, hud, kHudRacers), 0);
  // sub_82106540(200, 500, 0, 0x8200F710, +460): empty (audio.md), so
  // nothing but its r3, the 200 it was handed.
  return 200;
}

uint64_t Draw(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822128A8 RaceHud::Draw");
  const uint32_t frame = sp - 96;
  guest::StoreU32(base, frame, sp);
  uint64_t r3 = calls.Call(kRun, frame, 0x822128C8, Movie(base, hud, kHudDashboard), 0);
  if (guest::LoadU32(base, hud + 472) == 0 || guest::LoadU32(base, hud + 12) == 0) {
    return r3;
  }
  const uint32_t mirror = ui_movies::FindNode(base, Movie(base, hud, kHudDashboard), kNone, 19);
  return calls.Call(kDrawMirror, frame, 0x822128F4, mirror);
}

uint64_t UpdateWarning(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82212C50 RaceHud::UpdateWarning");
  const uint32_t frame = sp - 96;
  guest::StoreU32(base, frame, sp);
  const uint32_t car = guest::LoadU32(base, hud + kHudVehicle);
  const uint32_t wrong_way = guest::LoadU32(base, car + 6168);
  if (wrong_way == 0) {
    if (guest::LoadU32(base, hud + 468) == 0) {
      return hud;
    }
    const uint64_t r3 =
        calls.Call(kFinish, frame, 0x82212C90, Movie(base, hud, kHudWarning), kNone, 0);
    guest::StoreU32(base, hud + 468, 0);
    return r3;
  }
  if (guest::LoadU32(base, hud + 468) != 0) {
    return hud;
  }
  calls.Call(kFinish, frame, 0x82212CBC, Movie(base, hud, kHudWarning), kNone, 0);
  ui_movies::Play(base, Movie(base, hud, kHudWarning), kNone, 0, 0, 0, 0, 0);
  const uint32_t movie = Movie(base, hud, kHudWarning);
  const uint64_t r3 =
      MovedR3(ui_movies::MoveAnimation(base, movie, kNone, 0, true), frame, movie);
  guest::StoreU32(base, hud + 468, 1);
  return r3;
}

uint64_t UpdateRecordingFull(uint8_t* base, uint32_t hud, int32_t room, uint32_t sp,
                             const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82212E60 RaceHud::UpdateRecordingFull");
  const uint32_t frame = sp - 96;
  guest::StoreU32(base, frame, sp);
  if (room == 0) {
    if (guest::LoadU32(base, hud + 492) != 0) {
      return hud;
    }
    const uint32_t dashboard = Movie(base, hud, kHudDashboard);
    guest::StoreU32(base, hud + 492, 1);
    calls.Call(kFinishNamed, frame, 0x82212E9C, dashboard, 6);
    return ui_movies::PlayNamed(base, Movie(base, hud, kHudDashboard), 6, 0, 0, 0, 0);
  }
  if (room != 1) {
    return hud;
  }
  const uint32_t dashboard = Movie(base, hud, kHudDashboard);
  guest::StoreU32(base, hud + 492, 0);
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
  uint32_t chunk = guest::LoadU32(base, Movie(base, hud, kHudRacers) + ui_movies::kMovieLive);
  while (guest::LoadU32(base, chunk) != ui_movies::kTagTrm) {
    const uint32_t tag = guest::LoadU32(base, chunk);
    if (tag == kTagPrm || tag == kTagPic || tag == kTagTxt) {
      guest::StoreU32(base, chunk + 80, shown);
    }
    chunk += guest::LoadU32(base, chunk + 4);
  }
  if (id == -1) {
    return hud;
  }
  const uint32_t name = ui_movies::FindNode(base, Movie(base, hud, kHudRacers), kNone, 19);
  const uint32_t picture = ui_movies::FindNode(base, Movie(base, hud, kHudRacers), 5, 0);
  guest::StoreU32(base, name + 80, 1);
  const uint32_t panel = ui_movies::FindNode(base, Movie(base, hud, kHudRacers), kNone, 5);
  guest::StoreU32(base, panel + 80, 1);
  guest::StoreU32(base, picture + 80, 1);
  // UiMovie::SetNodeText's r6 to r10 (varargs) passed 0: the original leaves
  // them unset (the user's call for the parking area, 2026-10-30).
  calls.Call(kSetNodeText, frame, 0x82213724, Movie(base, hud, kHudRacers), name,
             uint64_t(int64_t(id) + 4650));
  const uint32_t item = data_tables::Item(base, uint32_t(id));
  const int32_t kind = int8_t(guest::LoadU8(base, item + 34));
  guest::StoreU32(base, picture + 108, uint32_t(kind < 48 ? kind : id - 262));
  return item;
}

uint64_t UpdateItem(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82214238 RaceHud::UpdateItem");
  const uint32_t item = guest::LoadU32(base, guest::LoadU32(base, kRaceProgress) + 7240);
  if (guest::LoadU32(base, hud + 460) == item) {
    return hud;
  }
  guest::StoreU32(base, hud + 460, item);
  return ShowItem(base, hud, int32_t(item), sp, calls);
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
  static constexpr struct {
    uint32_t id, field;
  } kMovies[9] = {{10, 604}, {0, 608},  {11, 612}, {3, 616}, {2, 620},
                  {13, 624}, {1, 628}, {12, 632}, {19, 636}};
  const uint32_t key = frame + 80;
  uint32_t found = 0;
  for (uint32_t i = 0; i < 9; i++) {
    guest::StoreU32(base, key, kMovies[i].id);
    found = ui_movies::PairsFind(base, ui_movies::kMovieTable, key);
    const uint32_t movie = guest::LoadU32(base, found);
    if (i == 8) {
      guest::StoreU32(base, hud + 588, 0);
    }
    guest::StoreU32(base, hud + kMovies[i].field, movie);
  }
  return found;
}

}  // namespace race_hud_object
