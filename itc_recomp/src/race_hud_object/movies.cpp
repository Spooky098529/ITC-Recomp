// The race HUD object's movie calls and its set-up
// (docs/functions/race-hud-object.md): see race_hud_object.h.

#include "race_hud_object/race_hud_object.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"

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
