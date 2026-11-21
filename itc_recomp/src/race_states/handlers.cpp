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
using G = schema::RaceStatesGlobals;
using Machine = schema::RaceStates;

namespace {

void FadeOutBlack(uint8_t* base) {
  mode_tick::ScreenFadesFadeOut(base, G::fade_out_passes(base), 0, 0, 0, 0);
}

void SetFlags(uint8_t* base, uint32_t bits) {
  const schema::RaceData data = G::race_data(base);
  data.set_flags(data.flags() | bits);
}

}  // namespace

void Begin(uint8_t* base, uint32_t machine, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82164DC0 RaceStates::Begin");
  const Machine m(base, machine);
  const uint32_t progress = m.progress().address;
  m.set_ticks(0);
  m.set_word_9620(0);
  m.set_step(0);
  ProgressReset(base, progress, frame - 96);
  m.set_still_ticks(0);
  m.set_word_9632(0);
  m.set_settled(0);
  m.set_settled_ticks(0);
}

void FadeInFree(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82163398 RaceStates::FadeInFree");
  const Machine m(base, machine);
  m.set_ticks(0);
  m.set_word_9620(0);
  m.set_step(0);
  mode_tick::ScreenFadesFadeIn(base, G::fade_in_passes(base), 0, 0, 0, 0);
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
  return memory::crt::OperatorDelete(G::progress(base).address);
}

void OnRollingStart(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_821632C8 RaceStates::OnRollingStart");
  if (int32_t(G::fade_alpha(base)) == 0) {
    FadeOutBlack(base);
  }
  Machine(base, machine).set_step(0);
}

uint64_t OnStandingStart(uint8_t* base, uint32_t machine, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821643B0 RaceStates::OnStandingStart");
  const Machine m(base, machine);
  m.set_word_9620(0);
  m.set_still_ticks(0);
  m.set_word_9632(0);
  m.set_step(0);
  m.set_ticks(0);
  m.set_settled(0);
  m.set_settled_ticks(0);
  G::set_settled_word(base, 0);
  return ai_drivers::StartRanked(base, sp - 224);
}

void ClearStep(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82163348 RaceStates::ClearStep");
  Machine(base, machine).set_step(0);
}

void OnQuit(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82163378 RaceStates::OnQuit");
  Machine(base, machine).set_pause_quit(0);
}

void OnReplay(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_821647B0 RaceStates::OnReplay");
  FadeOutBlack(base);
  const Machine m(base, machine);
  m.set_step(0);
  m.set_still_ticks(0);
  m.set_word_9632(0);
  SetFlags(base, 0x2000);
}

void FreeStartTick(uint8_t* base, uint32_t machine, uint32_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_82168B80 RaceStates::FreeStartTick");
  const Machine m(base, machine);
  if (m.ticks() > 90) {
    const int32_t count = int32_t(track_collision::RaceRacerCount(base));
    bool hold = false;
    for (int32_t i = 0; i < count; i++) {
      const uint32_t vehicle = va::VehicleGet(base, uint32_t(i));
      if (vehicle == 0 || (VehicleView(base, vehicle).car_flags() & 129) == 0) {
        continue;
      }
      const uint32_t identity = va::VehicleIdentity(vehicle);
      if (int32_t(track_route::SliceTest(base, race_world::kRoute,
                                              TrackPos(base, identity).route_section_address())) == 1) {
        hold = true;
        break;
      }
    }
    if (!hold && FadesDone(base)) {
      Post(base, dispatcher, 7);
      return;
    }
  }
  m.set_ticks(m.ticks() + 1);
}

void QuitTick(uint8_t* base, uint32_t machine, uint32_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_82168500 RaceStates::QuitTick");
  Post(base, dispatcher, 29);
  const schema::RaceData data = G::race_data(base);
  data.set_flags(data.flags() & ~0x20000u);
  const Machine m(base, machine);
  uint32_t message;
  if (int32_t(Kind(base)) == 5) {
    message = 48;
  } else if (G::race_data(base).flags() & 0x80) {
    message = 36;
  } else {
    m.progress().set_end(3);
    const uint32_t how = int32_t(m.pause_quit()) != 0 ? 1 : 2;
    m.progress().set_end_reason(how);
    message = 34;
  }
  Post(base, dispatcher, message);
}

void AfterRaceTick(uint8_t* base, uint32_t machine, uint32_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_82167970 RaceStates::AfterRaceTick");
  uint32_t message = 38;
  if (G::race_data(base).flags() & 0x80) {
    message = int32_t(Kind(base)) == 5 ? 42 : 36;
  }
  Post(base, dispatcher, message);
  Machine(base, machine).set_ticks(0);
}

void RollingStartTick(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t frame,
                      bool heap, const race_world::OriginalCall& refresh,
                      const race_world::StartCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82166E50 RaceStates::RollingStartTick");
  if (!FadesDone(base)) {
    return;
  }
  if (!(G::race_data(base).flags() & 0x100)) {
    Post(base, dispatcher, 31);
    SetFlags(base, 0x100);
  }
  SetFlags(base, 0x2000);
  SetFlags(base, 0x4000);
  race_world::Refresh(base, G::racers_manager(base), frame - 1904, heap, refresh);
  race_world::PlaceRolling(base, machine, 1, frame - 272, calls);
  Post(base, dispatcher, 15);
  Machine(base, machine).set_step(0);
}

}  // namespace race_states
