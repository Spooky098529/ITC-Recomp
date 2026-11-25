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

void FadeOutBlack() {
  mode_tick::ScreenFadesFadeOut(G::fade_out_passes(), 0, 0, 0, 0);
}

void SetFlags(uint32_t bits) {
  const schema::RaceData data = G::race_data();
  data.set_flags(data.flags() | bits);
}

}  // namespace

void Begin(uint32_t machine, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82164DC0 RaceStates::Begin");
  const Machine m(machine);
  const uint32_t progress = m.progress().address;
  m.set_ticks(0);
  m.set_word_9620(0);
  m.set_step(0);
  ProgressReset(progress, frame - 96);
  m.set_still_ticks(0);
  m.set_word_9632(0);
  m.set_settled(0);
  m.set_settled_ticks(0);
}

void FadeInFree(uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82163398 RaceStates::FadeInFree");
  const Machine m(machine);
  m.set_ticks(0);
  m.set_word_9620(0);
  m.set_step(0);
  mode_tick::ScreenFadesFadeIn(G::fade_in_passes(), 0, 0, 0, 0);
}

void FadeOut() {
  ITC_NATIVE_RUNNING("sub_82162310 RaceStates::FadeOut");
  FadeOutBlack();
}

void EndWhenFaded(uint32_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_82168F50 RaceStates::EndWhenFaded");
  if (FadesDone()) {
    Post(dispatcher, 49);
  }
}

bool Unload() {
  ITC_NATIVE_RUNNING("sub_82163920 RaceStates::Unload");
  return memory::crt::OperatorDelete(G::progress().address);
}

void OnRollingStart(uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_821632C8 RaceStates::OnRollingStart");
  if (int32_t(G::fade_alpha()) == 0) {
    FadeOutBlack();
  }
  Machine(machine).set_step(0);
}

uint64_t OnStandingStart(uint32_t machine, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821643B0 RaceStates::OnStandingStart");
  const Machine m(machine);
  m.set_word_9620(0);
  m.set_still_ticks(0);
  m.set_word_9632(0);
  m.set_step(0);
  m.set_ticks(0);
  m.set_settled(0);
  m.set_settled_ticks(0);
  G::set_settled_word(0);
  return ai_drivers::StartRanked(sp - 224);
}

void ClearStep(uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82163348 RaceStates::ClearStep");
  Machine(machine).set_step(0);
}

void OnQuit(uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82163378 RaceStates::OnQuit");
  Machine(machine).set_pause_quit(0);
}

void OnReplay(uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_821647B0 RaceStates::OnReplay");
  FadeOutBlack();
  const Machine m(machine);
  m.set_step(0);
  m.set_still_ticks(0);
  m.set_word_9632(0);
  SetFlags(0x2000);
}

void FreeStartTick(uint32_t machine, uint32_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_82168B80 RaceStates::FreeStartTick");
  const Machine m(machine);
  if (m.ticks() > 90) {
    const int32_t count = int32_t(track_collision::RaceRacerCount());
    bool hold = false;
    for (int32_t i = 0; i < count; i++) {
      const uint32_t vehicle = va::VehicleGet(uint32_t(i));
      if (vehicle == 0 || (VehicleView(vehicle).car_flags() & 129) == 0) {
        continue;
      }
      const uint32_t identity = va::VehicleIdentity(vehicle);
      if (int32_t(track_route::SliceTest(race_world::kRoute,
                                              TrackPos(identity).route_section_address())) == 1) {
        hold = true;
        break;
      }
    }
    if (!hold && FadesDone()) {
      Post(dispatcher, 7);
      return;
    }
  }
  m.set_ticks(m.ticks() + 1);
}

void QuitTick(uint32_t machine, uint32_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_82168500 RaceStates::QuitTick");
  Post(dispatcher, 29);
  const schema::RaceData data = G::race_data();
  data.set_flags(data.flags() & ~0x20000u);
  const Machine m(machine);
  uint32_t message;
  if (int32_t(Kind()) == 5) {
    message = 48;
  } else if (G::race_data().flags() & 0x80) {
    message = 36;
  } else {
    m.progress().set_end(3);
    const uint32_t how = int32_t(m.pause_quit()) != 0 ? 1 : 2;
    m.progress().set_end_reason(how);
    message = 34;
  }
  Post(dispatcher, message);
}

void AfterRaceTick(uint32_t machine, uint32_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_82167970 RaceStates::AfterRaceTick");
  uint32_t message = 38;
  if (G::race_data().flags() & 0x80) {
    message = int32_t(Kind()) == 5 ? 42 : 36;
  }
  Post(dispatcher, message);
  Machine(machine).set_ticks(0);
}

void RollingStartTick(uint32_t machine, uint32_t dispatcher, uint32_t frame,
                      bool heap, const race_world::OriginalCall& refresh,
                      const race_world::StartCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82166E50 RaceStates::RollingStartTick");
  if (!FadesDone()) {
    return;
  }
  if (!(G::race_data().flags() & 0x100)) {
    Post(dispatcher, 31);
    SetFlags(0x100);
  }
  SetFlags(0x2000);
  SetFlags(0x4000);
  race_world::Refresh(G::racers_manager(), frame - 1904, heap, refresh);
  race_world::PlaceRolling(machine, 1, frame - 272, calls);
  Post(dispatcher, 15);
  Machine(machine).set_step(0);
}

}  // namespace race_states
