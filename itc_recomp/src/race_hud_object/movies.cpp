// The race HUD object's movie calls and its set-up
// (docs/functions/race-hud-object.md): see race_hud_object.h.

#include "race_hud_object/race_hud_object.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "menu_sounds/menu_sounds.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <bit>

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

uint64_t Make1004(uint8_t* base, uint32_t owner, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82216A60 RaceScreens::Make1004");
  const uint32_t frame = sp - 128;
  guest::StoreU32(base, frame, sp);
  uint32_t object = uint32_t(calls.Call(kNew, frame, 0x82216A78, 264));
  if (object != 0) {
    calls.Call(kMemset, frame, 0x82216A94, object, 0, 264);
  }
  guest::StoreU32(base, owner + 0, object);
  const uint32_t list = uint32_t(calls.Call(kNew, frame, 0x82216AA8, 72));
  if (list != 0) {
    guest::StoreU32(base, list + 68, 0);
    guest::StoreU32(base, list + 60, 0);
  }
  guest::StoreU32(base, owner + 4, list);
  guest::StoreU32(base, frame + 84, 159);
  guest::StoreU32(base, frame + 92, 158);
  const uint64_t r3 =
      calls.CallArgs(kMenuListInit, frame, 0x82216AF4, {list, 1, 1, 0, 154, 153, 155, 156});
  object = guest::LoadU32(base, owner + 0);
  guest::StoreU32(base, object + 0, 0);
  guest::StoreU32(base, object + 256, 0);
  return r3;
}

uint64_t ScreensInit(uint8_t* base, uint32_t holder, uint32_t kind, uint32_t sp,
                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82211DA0 RaceScreens::Init");
  const uint32_t frame = sp - 128;
  guest::StoreU32(base, frame, sp);
  calls.Call(kMoviesStart, frame, 0x82211DB8, holder, kind);
  calls.Call(kStringsLoad, frame, 0x82211DC8, guest::LoadU32(base, 0x826C5C34), 47);
  const uint32_t mask = MovieMask(kind);
  for (uint32_t i = 0; i < 23; i++) {
    if ((mask >> i) & 1) {
      calls.Call(kMoviesOpen, frame, 0x82211E04, i, guest::LoadU32(base, 0x8200D158 + 4 * i));
    }
  }
  static constexpr uint32_t kGlobals[7] = {0x826D84D0, 0x826D84E0, 0x826D8510, 0x826D8500,
                                           0x82814E18, 0x82814E1C, 0x82814E10};
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU32(base, holder + 1012 + 4 * i, guest::LoadU32(base, kGlobals[i]));
  }
  Init(base, holder + 16, frame);
  const uint64_t r3 = Make1004(base, holder + 1004, frame, calls);
  guest::StoreU32(base, holder + 8, 0);
  return r3;
}

uint64_t RunCountdown(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls,
                      const menu_sounds::Calls& sounds, double* f1) {
  ITC_NATIVE_RUNNING("sub_822129A8 RaceHud::RunCountdown");
  const uint32_t frame = sp - 112;
  guest::StoreU32(base, frame, sp);
  const uint64_t run = calls.Call(kRun, frame, 0x822129C8, Movie(base, hud, kHudCountdown), 0);
  const uint32_t movie = Movie(base, hud, kHudCountdown);
  guest::StoreU32(base, hud + 436, uint32_t(run));
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
  if (guest::LoadU32(base, hud + 468) == 0) {
    return hud;
  }
  // Sound 10 unless the sound manager's slot 14 (AudioManager::IsCuePlaying:
  // bank 0, cue 10 at the frame's +84 and +80) says it's playing.
  guest::StoreU32(base, frame + 80, 10);
  guest::StoreU32(base, frame + 84, 0);
  const uint32_t manager = guest::LoadU32(base, menu_sounds::kSoundManager);
  const uint32_t playing = guest::LoadU32(base, guest::LoadU32(base, manager) + 56);
  const uint64_t r3 = calls.Call(playing, frame, 0x82212A98, manager, frame + 84, frame + 80);
  if ((r3 & 0xFF) == 0) {
    menu_sounds::PlayMenuSound(base, 10, frame, sounds);
  }
  return calls.Call(kRun, frame, 0x82212AB4, Movie(base, hud, kHudWarning), 0);
}

uint64_t UpdateRacerBar(uint8_t* base, uint32_t hud, uint64_t index, uint64_t racer, uint32_t sp,
                        const Calls& calls, const menu_sounds::Calls& sounds) {
  ITC_NATIVE_RUNNING("sub_82214790 RaceHud::UpdateRacerBar");
  using namespace camera_modes::steps;
  const uint32_t frame = sp - 176;
  guest::StoreU32(base, frame, sp);
  const uint32_t bar = uint32_t(index * 40 + hud) + 244;
  const uint32_t racers = guest::LoadU32(base, hud + 444);
  const uint32_t target = guest::LoadU32(base, uint32_t(racer * 972) + racers + 788);
  if (guest::LoadU32(base, bar + 36) == 0 && Touching(base, uint32_t(racer), frame) != 0) {
    const double value = Lfs(base, bar + 0);
    guest::StoreU32(base, bar + 36, 1);
    Stfs(base, bar + 8, value);
  }
  BarStep(base, bar, int32_t(target), frame);
  // The group (r4 + 1) and its children 1, 3, 2 and 4, in the original's
  // order.
  const uint32_t group = ui_movies::FindNode(base, Movie(base, hud, kHudRacers), kNone,
                                             uint32_t(index + 1));
  auto child = [&](uint32_t k) {
    return ui_movies::FindNode(base, Movie(base, hud, kHudRacers), kNone,
                               guest::LoadU32(base, group + 100) + k);
  };
  const uint32_t n1 = child(1);
  const uint32_t n3 = child(3);
  const uint32_t n2 = child(2);
  const uint32_t n4 = child(4);
  uint64_t r3 = n4;
  const bool first = int32_t(uint32_t(index)) == 0;
  const double goal = Lfs(base, bar + 24);
  if (!(goal > Lfs(base, bar + 16))) {
    guest::StoreU32(base, n1 + 80, 0);
    guest::StoreU32(base, n3 + 80, 0);
    guest::StoreU32(base, n4 + 80, 0);
    if (guest::LoadU32(base, bar + 32) != 0) {
      r3 = calls.Call(kFinish, frame, 0x822148A8, Movie(base, hud, kHudRacers), ~0ull, index);
    }
  } else {
    // The double at 0x8200F738 times the maximum (fmul, not rounded).
    const uint64_t bits =
        uint64_t(guest::LoadU32(base, 0x8200F738)) << 32 | guest::LoadU32(base, 0x8200F73C);
    const double low = Lfs(base, bar + 20) * std::bit_cast<double>(bits);
    if (!(goal > low)) {
      guest::StoreU32(base, n1 + 80, 0);
      guest::StoreU32(base, n3 + 80, 0);
      guest::StoreU32(base, n4 + 80, 1);
      guest::StoreU32(base, hud + 456, 1);
      if (guest::LoadU32(base, bar + 32) == 0) {
        guest::StoreU32(base, bar + 32, 1);
        ui_movies::Play(base, Movie(base, hud, kHudRacers), kNone, uint32_t(index), 0, 0, 0, 0);
        const uint32_t movie = Movie(base, hud, kHudRacers);
        r3 = MovedR3(ui_movies::MoveAnimation(base, movie, kNone, uint32_t(index), true), frame,
                     movie);
      }
      if (first) {
        // Sound 7 unless the sound manager's slot 14 (AudioManager::
        // IsCuePlaying: bank 0, cue 7 at the frame's +84 and +80) says it's
        // playing.
        guest::StoreU32(base, frame + 84, 0);
        guest::StoreU32(base, frame + 80, 7);
        const uint32_t manager = guest::LoadU32(base, menu_sounds::kSoundManager);
        const uint32_t playing = guest::LoadU32(base, guest::LoadU32(base, manager) + 56);
        r3 = calls.Call(playing, frame, 0x82214948, manager, frame + 84, frame + 80);
        if ((r3 & 0xFF) == 0) {
          r3 = menu_sounds::PlayMenuSound(base, 7, frame, sounds);
        }
      }
    } else {
      guest::StoreU32(base, n1 + 80, guest::LoadU32(base, bar + 28) != 0 ? 0 : 1);
      guest::StoreU32(base, n3 + 80, 1);
      guest::StoreU32(base, n4 + 80, 0);
    }
  }
  guest::StoreU32(base, n2 + 80, int32_t(guest::LoadU32(base, bar + 36)) == 1 ? 1 : 0);
  // The four nodes' x scales (+28): the value's fraction of the range (n3,
  // n1, n4) and the trail's lead over it (n2), each loaded again.
  auto range = [&]() { return Fsubs(Lfs(base, bar + 20), Lfs(base, bar + 16)); };
  Stfs(base, n3 + 28, Fdivs(Lfs(base, bar + 0), range()));
  Stfs(base, n1 + 28, Fdivs(Lfs(base, bar + 0), range()));
  {
    const double lead = Fsubs(Lfs(base, bar + 8), Lfs(base, bar + 0));
    Stfs(base, n2 + 28, Fdivs(lead, range()));
  }
  Stfs(base, n4 + 28, Fdivs(Lfs(base, bar + 0), range()));
  // Node 2's x (+12): after node 1 by the float at 0x8200F730 times its
  // scale (bar 0: 1.0 less the trail's fraction).
  const double width = Lfs(base, 0x8200F730);
  double x;
  if (first) {
    const double scale = Fsubs(Lfs(base, 0x82000A30), Fdivs(Lfs(base, bar + 8), range()));
    x = Fmadds(scale, width, Lfs(base, n1 + 12));
  } else {
    x = Fmadds(Lfs(base, n1 + 28), width, Lfs(base, n1 + 12));
  }
  Stfs(base, n2 + 12, x);
  return r3;
}

}  // namespace race_hud_object
