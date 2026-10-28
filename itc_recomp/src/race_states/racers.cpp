// The racers at a standing start and the countdown
// (docs/functions/race-states.md).

#include "race_states/race_states.h"
#include "race_states/steps.h"

#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "car_physics/float_ops.h"
#include "mode_tick/mode_tick.h"
#include "track_collision/track_collision.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_states {

using namespace car_physics::ops;
using namespace steps;
namespace va = vehicle_accessors;
namespace vl = vector_library;

namespace {

constexpr uint32_t kClose = 0x8200257C;  // 50.0

uint32_t RaceData(uint8_t* base) {
  return Load(base, race_world::kRaceData);
}
uint32_t Stages(uint8_t* base) {
  return RaceData(base) + 0x59620;
}
// The racers' list's entry j (the manager's +2308).
uint32_t Racer(uint8_t* base, uint32_t j) {
  return Load(base, Load(base, race_world::kManager) + 2308 + 4 * j);
}
// The first racer record (from the race data's +48, its count +0x595E0)
// flagged 0x1 (the player's): its index, or -1 with none.
int32_t PlayerRecord(uint8_t* base) {
  const uint32_t records = RaceData(base) + 48;
  const int32_t count = int32_t(Load(base, records + 0x595E0));
  for (int32_t i = 0; i < count; i++) {
    if (Load(base, records + 3888 + 3936 * uint32_t(i)) & 1) {
      return i;
    }
  }
  return -1;
}
// A racer's entry is in the current stage and isn't the player's.
bool OtherInStage(uint8_t* base, uint32_t entry) {
  const uint32_t record = Load(base, entry + 24);
  if (va::StagesHasRacer(base, Stages(base), record, 0xFFFFFFFF) == 0) {
    return false;
  }
  return !(Load(base, (record + 1) * 3936 + RaceData(base)) & 1);
}
// A route position's (the halfwords at `position`) slice center: the
// section's record (its unsigned halfword x 4 in the table) + 124 + 84 x
// the signed slice.
uint32_t SliceCenter(uint8_t* base, uint32_t position) {
  const uint32_t file = Load(base, race_world::kRoute);
  const uint32_t record =
      Load(base, Load(base, file + 12) + (uint32_t(guest::LoadU16(base, position)) << 2));
  return uint32_t(int64_t(S16(base, position + 2)) * 84) + record + 124;
}

}  // namespace

uint32_t RacersNear(uint8_t* base, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82165AB0 RaceStates::RacersNear");
  int32_t player_section = 0;
  const int32_t player = PlayerRecord(base);
  if (player >= 0) {
    const uint32_t entry = race_world::EntryOfRecord(base, uint32_t(player));
    const uint32_t vehicle = va::VehicleGet(base, Load(base, entry + 20));
    player_section = S16(base, va::VehicleIdentity(vehicle) + 4);
  }
  for (uint32_t k = 0; k < 10; k++) {
    Store(base, frame + 80 + 4 * k, k);
  }
  Store(base, frame + 120, 0xFFFFFFFF);
  for (int32_t j = 0; j < int32_t(track_collision::RaceRacerCount(base)); j++) {
    const uint32_t entry = Racer(base, uint32_t(j));
    if (!OtherInStage(base, entry)) {
      continue;
    }
    const uint32_t vehicle = va::VehicleGet(base, Load(base, entry + 20));
    const uint32_t identity = va::VehicleIdentity(vehicle);
    const uint32_t file = Load(base, race_world::kRoute);
    const uint32_t record =
        Load(base, Load(base, file + 12) + (uint32_t(guest::LoadU16(base, identity + 4)) << 2));
    if (player_section == S16(base, identity + 4)) {
      continue;
    }
    bool linked = false;
    for (uint32_t k = 0, link = 0;;) {
      const int32_t to = Link(base, record, link);
      if (to != -1 && player_section == to) {
        linked = true;
        break;
      }
      k++;
      link = Load(base, frame + 80 + 4 * k);
      if (int32_t(link) == -1) {
        break;
      }
    }
    if (!linked) {
      return 0;
    }
  }
  return 1;
}

uint32_t RacersClose(uint8_t* base, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82165C78 RaceStates::RacersClose");
  Stfs(base, frame + 92, Lfs(base, kOne));
  const int32_t player = PlayerRecord(base);
  if (player >= 0) {
    const uint32_t entry = race_world::EntryOfRecord(base, uint32_t(player));
    const uint32_t center = SliceCenter(base, entry + 8);
    Stfs(base, frame + 96, Lfs(base, center + 0));
    Stfs(base, frame + 100, Lfs(base, center + 4));
    Stfs(base, frame + 104, Lfs(base, center + 8));
  }
  for (int32_t j = 0; j < int32_t(track_collision::RaceRacerCount(base)); j++) {
    const uint32_t entry = Racer(base, uint32_t(j));
    va::VehicleGet(base, Load(base, entry + 20));
    if (!OtherInStage(base, entry)) {
      continue;
    }
    const uint32_t center = SliceCenter(base, entry + 8);
    Stfs(base, frame + 80, Fsubs(Lfs(base, frame + 96), Lfs(base, center + 0)));
    Stfs(base, frame + 84, Fsubs(Lfs(base, frame + 100), Lfs(base, center + 4)));
    Stfs(base, frame + 88, Fsubs(Lfs(base, frame + 104), Lfs(base, center + 8)));
    const uint32_t vector = (frame + 80) & ~0xFu;
    const float dot = vl::Vec3Dot(base, vector, vector);
    const uint32_t result = (frame + 112) & ~0xFu;
    for (uint32_t k = 0; k < 16; k += 4) {
      guest::StoreFloat(base, result + k, dot);
    }
    const double distance = double(vl::Sqrtf(Lfs(base, frame + 112)));
    if (distance > Lfs(base, kClose)) {
      return 0;
    }
  }
  return 1;
}

uint32_t RacersStill(uint8_t* base, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_82164A90 RaceStates::RacersStill");
  const int32_t racers = int32_t(track_collision::RaceRacerCount(base));
  bool moved = false;
  for (int32_t j = 0; j < racers; j++) {
    const uint32_t vehicle = va::VehicleGet(base, Load(base, Racer(base, uint32_t(j)) + 20));
    if (Load(base, Load(base, vehicle + 340) + 240) != 0 || (Load(base, vehicle + 6152) & 7)) {
      moved = true;
      break;
    }
  }
  if (moved) {
    Store(base, count, 0);
  } else if (int32_t(Load(base, count)) < 30) {
    Store(base, count, Load(base, count) + 1);
  }
  return int32_t(Load(base, count)) >= 30 ? 1 : 0;
}

void CountdownTick(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_821674A0 RaceStates::CountdownTick");
  if (RacersNear(base, frame - 208) == 0) {
    Post(base, dispatcher, 19);
    Store(base, machine + 9616, 0);
    Store(base, machine + 9620, 0);
    ai_drivers::ClearAll(base, frame - 128);
    return;
  }
  const uint32_t ticks = Load(base, machine + 9616);
  if (ticks < 120) {
    Store(base, machine + 9616, ticks + 1);
    return;
  }
  Post(base, dispatcher, 28);
  Store(base, machine + 9616, 0);
  Store(base, machine + 9620, 0);
  const uint32_t flags = FlagsAddress(base);
  Store(base, flags, Load(base, flags) & ~0x1000u);
  Store(base, FlagsAddress(base), Load(base, FlagsAddress(base)) & ~0x2000u);
  ai_drivers::ResetAll(base, frame - 128);
}

namespace {

constexpr uint32_t kFadeInPasses = 0x82001BEC;   // 5
constexpr uint32_t kFadeOutPasses = 0x82001BF0;  // 15
constexpr uint32_t kSignInState = 0x826BAB3C;
constexpr uint32_t kSignInReason = 0x826BAB5C;

void ChangeFlags(uint8_t* base, uint32_t set, uint32_t clear) {
  const uint32_t at = FlagsAddress(base);
  Store(base, at, (Load(base, at) | set) & ~clear);
}

}  // namespace

bool StandingStartTickNative(uint8_t* base, uint32_t machine) {
  return !(int32_t(Kind(base)) == 6 && Load(base, machine + 9616) < 900 &&
           Load(base, machine + 9636) != 0 && Load(base, machine + 9640) <= 900 &&
           int32_t(va::StagesType(base, Stages(base), 0xFFFFFFFF)) != 1);
}

void StandingStartTick(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t frame,
                       bool heap, const StandingCalls& calls) {
  ITC_NATIVE_RUNNING("sub_821692C8 RaceStates::StandingStartTick");
  if (RacersNear(base, frame - 208) == 0) {
    Post(base, dispatcher, 19);
    ai_drivers::ClearAll(base, frame - 128);
    return;
  }
  if (Load(base, machine + 9616) >= 900) {
    // The racers put back at the start, step by step.
    const uint32_t step = Load(base, machine + 9612);
    if (step == 0) {
      mode_tick::ScreenFadesFadeOut(base, Load(base, kFadeOutPasses), 0, 0, 0, 0);
      Store(base, machine + 9612, 1);
    }
    if (step <= 1) {
      if (!FadesDone(base)) {
        return;
      }
      Post(base, dispatcher, 31);
      ChangeFlags(base, 0x100, 0);
      ChangeFlags(base, 0x2000, 0);
      ChangeFlags(base, 0x4000, 0);
      race_world::Refresh(base, Load(base, race_world::kManager), frame - 1904, heap,
                          calls.refresh);
      Store(base, machine + 9612, 2);
    } else if (step == 2) {
      race_world::Place(base, machine, 1, 0, frame, calls.start);
      race_world::ListRacers(base, Load(base, race_world::kManager), frame - 1872, heap,
                             calls.list);
      Store(base, machine + 9612, 3);
    } else if (step == 3) {
      Post(base, dispatcher, 31);
      ChangeFlags(base, 0, 0x100);
      Post(base, dispatcher, 32);
      ChangeFlags(base, 0, 0x2000);
      ChangeFlags(base, 0, 0x4000);
      mode_tick::ScreenFadesFadeIn(base, Load(base, kFadeInPasses), 0, 0, 0, 0);
      Store(base, machine + 9612, 4);
    } else if (step == 4) {
      Store(base, machine + 9616, 0);
      Store(base, machine + 9612, 0);
      Store(base, machine + 9628, 0);
      Store(base, machine + 9632, 0);
    }
    return;
  }
  const bool linked = int32_t(Kind(base)) == 6;
  if (!linked) {
    if (RacersClose(base, frame - 208) == 0) {
      Store(base, machine + 9616, 900);
    } else {
      Store(base, machine + 9616, Load(base, machine + 9616) + 1);
    }
  }
  const uint32_t copy = frame + 80;
  if (!va::StagesCopy(base, copy, Stages(base), 0xFFFFFFFF)) {
    calls.start.original(0x820FFD88, frame, copy, Stages(base), 0xFFFFFFFFFFFFFFFFull);
  }
  if (Load(base, machine + 9636) == 0) {
    if (RacersStill(base, machine + 9628) != 1 || !FadesDone(base)) {
      return;
    }
    if (!(int32_t(Kind(base)) == 6 && int32_t(Load(base, copy)) != 1)) {
      uint32_t settled = 0;
      if (!ai_drivers::AllSettled(base, frame - 6320, &settled)) {
        settled = calls.all_settled(frame);
      }
      if (int32_t(settled) == 0) {
        return;
      }
    }
    Store(base, machine + 9636, 1);
    ChangeFlags(base, 0, 0x1);
    return;
  }
  if (Load(base, machine + 9640) > 900) {
    if (int32_t(Load(base, kSignInState)) == 0) {
      Store(base, kSignInState, 1);
      Store(base, kSignInReason, 6);
    }
    return;
  }
  // The System Link case (kind 6, the copy's +0 not 1) is the original's
  // (StandingStartTickNative).
  Post(base, dispatcher, 26);
  for (uint32_t at : {9628u, 9632u, 9616u, 9636u, 9640u}) {
    Store(base, machine + at, 0);
  }
  ai_drivers::ClearAll(base, frame - 128);
}

}  // namespace race_states
