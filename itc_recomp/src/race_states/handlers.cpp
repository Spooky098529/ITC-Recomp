// The race's states' handlers (docs/functions/race-states.md).

#include "race_states/race_states.h"
#include "race_states/steps.h"

#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "memory/c_runtime_heap.h"
#include "mode_tick/mode_tick.h"
#include "track_collision/track_collision.h"
#include "track_route/track_route.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_states {

using namespace steps;
namespace va = vehicle_accessors;

namespace {

constexpr uint32_t kFadeInPasses = 0x82001BEC;   // 5
constexpr uint32_t kFadeOutPasses = 0x82001BF0;  // 15
constexpr uint32_t kAlpha = 0x826A50D4;
constexpr uint32_t kSettledWord = 0x826C6B04;

void FadeOutBlack(uint8_t* base) {
  mode_tick::ScreenFadesFadeOut(base, Load(base, kFadeOutPasses), 0, 0, 0, 0);
}

void SetFlags(uint8_t* base, uint32_t bits) {
  const uint32_t at = FlagsAddress(base);
  Store(base, at, Load(base, at) | bits);
}

}  // namespace

void Begin(uint8_t* base, uint32_t machine, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82164DC0 RaceStates::Begin");
  const uint32_t progress = Load(base, machine + 9624);
  Store(base, machine + 9616, 0);
  Store(base, machine + 9620, 0);
  Store(base, machine + 9612, 0);
  ProgressReset(base, progress, frame - 96);
  for (uint32_t at : {9628u, 9632u, 9636u, 9640u}) {
    Store(base, machine + at, 0);
  }
}

void FadeInFree(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82163398 RaceStates::FadeInFree");
  Store(base, machine + 9616, 0);
  Store(base, machine + 9620, 0);
  Store(base, machine + 9612, 0);
  mode_tick::ScreenFadesFadeIn(base, Load(base, kFadeInPasses), 0, 0, 0, 0);
}

void FadeOut(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82162310 RaceStates::FadeOut");
  FadeOutBlack(base);
}

void EndWhenFaded(uint8_t* base, uint32_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_82168F50 RaceStates::EndWhenFaded");
  if (FadesDone(base)) {
    Post(base, dispatcher, 49);
  }
}

bool Unload(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82163920 RaceStates::Unload");
  return memory::crt::OperatorDelete(Load(base, kProgress));
}

void OnRollingStart(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_821632C8 RaceStates::OnRollingStart");
  if (int32_t(Load(base, kAlpha)) == 0) {
    FadeOutBlack(base);
  }
  Store(base, machine + 9612, 0);
}

uint64_t OnStandingStart(uint8_t* base, uint32_t machine, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821643B0 RaceStates::OnStandingStart");
  for (uint32_t at : {9620u, 9628u, 9632u, 9612u, 9616u, 9636u, 9640u}) {
    Store(base, machine + at, 0);
  }
  Store(base, kSettledWord, 0);
  return ai_drivers::StartRanked(base, sp - 224);
}

void ClearStep(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82163348 RaceStates::ClearStep");
  Store(base, machine + 9612, 0);
}

void OnQuit(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82163378 RaceStates::OnQuit");
  Store(base, machine + 9608, 0);
}

void OnReplay(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_821647B0 RaceStates::OnReplay");
  FadeOutBlack(base);
  Store(base, machine + 9612, 0);
  Store(base, machine + 9628, 0);
  Store(base, machine + 9632, 0);
  SetFlags(base, 0x2000);
}

void FreeStartTick(uint8_t* base, uint32_t machine, uint32_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_82168B80 RaceStates::FreeStartTick");
  if (Load(base, machine + 9616) > 90) {
    const int32_t count = int32_t(track_collision::RaceRacerCount(base));
    bool hold = false;
    for (int32_t i = 0; i < count; i++) {
      const uint32_t vehicle = va::VehicleGet(base, uint32_t(i));
      if (vehicle == 0 || (Load(base, vehicle + 6148) & 129) == 0) {
        continue;
      }
      const uint32_t identity = va::VehicleIdentity(vehicle);
      if (int32_t(track_route::SliceTest(base, race_world::kRoute, identity + 4)) == 1) {
        hold = true;
        break;
      }
    }
    if (!hold && FadesDone(base)) {
      Post(base, dispatcher, 7);
      return;
    }
  }
  Store(base, machine + 9616, Load(base, machine + 9616) + 1);
}

void QuitTick(uint8_t* base, uint32_t machine, uint32_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_82168500 RaceStates::QuitTick");
  Post(base, dispatcher, 29);
  const uint32_t flags = FlagsAddress(base);
  Store(base, flags, Load(base, flags) & ~0x20000u);
  uint32_t message;
  if (int32_t(Kind(base)) == 5) {
    message = 48;
  } else if (Load(base, FlagsAddress(base)) & 0x80) {
    message = 36;
  } else {
    Store(base, Load(base, machine + 9624) + 6104, 3);
    const uint32_t how = int32_t(Load(base, machine + 9608)) != 0 ? 1 : 2;
    Store(base, Load(base, machine + 9624) + 6108, how);
    message = 34;
  }
  Post(base, dispatcher, message);
}

void AfterRaceTick(uint8_t* base, uint32_t machine, uint32_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_82167970 RaceStates::AfterRaceTick");
  uint32_t message = 38;
  if (Load(base, FlagsAddress(base)) & 0x80) {
    message = int32_t(Kind(base)) == 5 ? 42 : 36;
  }
  Post(base, dispatcher, message);
  Store(base, machine + 9616, 0);
}

void RollingStartTick(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t frame,
                      bool heap, const race_world::OriginalCall& refresh,
                      const race_world::StartCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82166E50 RaceStates::RollingStartTick");
  if (!FadesDone(base)) {
    return;
  }
  if (!(Load(base, FlagsAddress(base)) & 0x100)) {
    Post(base, dispatcher, 31);
    SetFlags(base, 0x100);
  }
  SetFlags(base, 0x2000);
  SetFlags(base, 0x4000);
  race_world::Refresh(base, Load(base, race_world::kManager), frame - 1904, heap, refresh);
  race_world::PlaceRolling(base, machine, 1, frame - 272, calls);
  Post(base, dispatcher, 15);
  Store(base, machine + 9612, 0);
}

}  // namespace race_states
