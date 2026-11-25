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

using G = schema::RaceStatesGlobals;
using Machine = schema::RaceStates;

namespace {

uint32_t Stages() {
  return G::race_data().stages_address();
}
// The racers' list's entry j (the manager's +2308).
uint32_t Racer(uint32_t j) {
  return Load(Manager(G::racers_manager()).racers_address() + 4 * j);
}
// The first racer record (from the race data's +48, its count +0x59610)
// flagged 0x1 (the player's): its index, or -1 with none.
int32_t PlayerRecord() {
  const schema::RaceData data = G::race_data();
  const int32_t count = int32_t(data.car_count());
  for (int32_t i = 0; i < count; i++) {
    if (Load(data.record0_flags_address() + kRecordBytes * uint32_t(i)) & 1) {
      return i;
    }
  }
  return -1;
}
// A racer's entry is in the current stage and isn't the player's.
bool OtherInStage(uint32_t entry) {
  const uint32_t record = RacerEntry(entry).record();
  if (va::StagesHasRacer(Stages(), record, 0xFFFFFFFF) == 0) {
    return false;
  }
  return !(Load(G::race_data().record0_flags_address() + kRecordBytes * record) & 1);
}
// A route position's (the halfwords at `position`) slice center: the
// section's record (its unsigned halfword x 4 in the table) + 124 + 84 x
// the signed slice.
uint32_t SliceCenter(uint32_t position) {
  const uint32_t file = G::route();
  const schema::RoutePosition at(position);
  const uint32_t record =
      Load(RouteFile(file).sections() + (uint32_t(at.section()) << 2));
  return uint32_t(int64_t(S16(at.slice_address())) * RouteSlice::kSize) +
         RouteSection(record).slices_address();
}

}  // namespace

uint32_t RacersNear(uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82165AB0 RaceStates::RacersNear");
  int32_t player_section = 0;
  const int32_t player = PlayerRecord();
  if (player >= 0) {
    const uint32_t entry = race_world::EntryOfRecord(uint32_t(player));
    const uint32_t vehicle = va::VehicleGet(RacerEntry(entry).vehicle_index());
    player_section = S16(TrackPos(va::VehicleIdentity(vehicle)).route_section_address());
  }
  for (uint32_t k = 0; k < 10; k++) {
    Store(frame + 80 + 4 * k, k);
  }
  Store(frame + 120, 0xFFFFFFFF);
  for (int32_t j = 0; j < int32_t(track_collision::RaceRacerCount()); j++) {
    const uint32_t entry = Racer(uint32_t(j));
    if (!OtherInStage(entry)) {
      continue;
    }
    const uint32_t vehicle = va::VehicleGet(RacerEntry(entry).vehicle_index());
    const TrackPos identity(va::VehicleIdentity(vehicle));
    const uint32_t file = G::route();
    const uint32_t record = Load(RouteFile(file).sections() +
                                           (uint32_t(identity.route_section()) << 2));
    if (player_section == S16(identity.route_section_address())) {
      continue;
    }
    bool linked = false;
    for (uint32_t k = 0, link = 0;;) {
      const int32_t to = Link(record, link);
      if (to != -1 && player_section == to) {
        linked = true;
        break;
      }
      k++;
      link = Load(frame + 80 + 4 * k);
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

uint32_t RacersClose(uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82165C78 RaceStates::RacersClose");
  Stfs(frame + 92, Lfs(kOne));
  const int32_t player = PlayerRecord();
  if (player >= 0) {
    const uint32_t entry = race_world::EntryOfRecord(uint32_t(player));
    const RouteSlice center(SliceCenter(RacerEntry(entry).position_address()));
    Stfs(frame + 96, Lfs(center.center_x_address()));
    Stfs(frame + 100, Lfs(center.center_y_address()));
    Stfs(frame + 104, Lfs(center.center_z_address()));
  }
  for (int32_t j = 0; j < int32_t(track_collision::RaceRacerCount()); j++) {
    const uint32_t entry = Racer(uint32_t(j));
    va::VehicleGet(RacerEntry(entry).vehicle_index());
    if (!OtherInStage(entry)) {
      continue;
    }
    const RouteSlice center(SliceCenter(RacerEntry(entry).position_address()));
    Stfs(frame + 80, Fsubs(Lfs(frame + 96), Lfs(center.center_x_address())));
    Stfs(frame + 84, Fsubs(Lfs(frame + 100), Lfs(center.center_y_address())));
    Stfs(frame + 88, Fsubs(Lfs(frame + 104), Lfs(center.center_z_address())));
    const uint32_t vector = (frame + 80) & ~0xFu;
    const float dot = vl::Vec3Dot(vector, vector);
    const uint32_t result = (frame + 112) & ~0xFu;
    for (uint32_t k = 0; k < 16; k += 4) {
      guest::StoreFloat(result + k, dot);
    }
    const double distance = double(vl::Sqrtf(Lfs(frame + 112)));
    if (distance > Lfs(G::close_distance_address())) {
      return 0;
    }
  }
  return 1;
}

uint32_t RacersStill(uint32_t count) {
  ITC_NATIVE_RUNNING("sub_82164A90 RaceStates::RacersStill");
  const int32_t racers = int32_t(track_collision::RaceRacerCount());
  bool moved = false;
  for (int32_t j = 0; j < racers; j++) {
    const VehicleView vehicle(va::VehicleGet(RacerEntry(Racer(uint32_t(j))).vehicle_index()));
    if (schema::CollisionObject(vehicle.collision_object()).first_contact().address != 0 ||
        (vehicle.collision_flags() & 7)) {
      moved = true;
      break;
    }
  }
  if (moved) {
    Store(count, 0);
  } else if (int32_t(Load(count)) < 30) {
    Store(count, Load(count) + 1);
  }
  return int32_t(Load(count)) >= 30 ? 1 : 0;
}

void CountdownTick(uint32_t machine, uint32_t dispatcher, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_821674A0 RaceStates::CountdownTick");
  const Machine m(machine);
  if (RacersNear(frame - 208) == 0) {
    Post(dispatcher, 19);
    m.set_ticks(0);
    m.set_word_9620(0);
    ai_drivers::ClearAll(frame - 128);
    return;
  }
  const uint32_t ticks = m.ticks();
  if (ticks < 120) {
    m.set_ticks(ticks + 1);
    return;
  }
  Post(dispatcher, 28);
  m.set_ticks(0);
  m.set_word_9620(0);
  const schema::RaceData data = G::race_data();
  data.set_flags(data.flags() & ~0x1000u);
  // The original loads the pointer twice here (the store's, the load's).
  const schema::RaceData store_to = G::race_data();
  store_to.set_flags(G::race_data().flags() & ~0x2000u);
  ai_drivers::ResetAll(frame - 128);
}

namespace {

void ChangeFlags(uint32_t set, uint32_t clear) {
  const schema::RaceData data = G::race_data();
  data.set_flags((data.flags() | set) & ~clear);
}

}  // namespace

bool StandingStartTickNative(uint32_t machine) {
  const Machine m(machine);
  return !(int32_t(Kind()) == 6 && m.ticks() < 900 && m.settled() != 0 &&
           m.settled_ticks() <= 900 &&
           int32_t(va::StagesType(Stages(), 0xFFFFFFFF)) != 1);
}

void StandingStartTick(uint32_t machine, uint32_t dispatcher, uint32_t frame,
                       bool heap, const StandingCalls& calls) {
  ITC_NATIVE_RUNNING("sub_821692C8 RaceStates::StandingStartTick");
  const Machine m(machine);
  if (RacersNear(frame - 208) == 0) {
    Post(dispatcher, 19);
    ai_drivers::ClearAll(frame - 128);
    return;
  }
  if (m.ticks() >= 900) {
    // The racers put back at the start, step by step.
    const uint32_t step = m.step();
    if (step == 0) {
      mode_tick::ScreenFadesFadeOut(G::fade_out_passes(), 0, 0, 0, 0);
      m.set_step(1);
    }
    if (step <= 1) {
      if (!FadesDone()) {
        return;
      }
      Post(dispatcher, 31);
      ChangeFlags(0x100, 0);
      ChangeFlags(0x2000, 0);
      ChangeFlags(0x4000, 0);
      race_world::Refresh(G::racers_manager(), frame - 1904, heap, calls.refresh);
      m.set_step(2);
    } else if (step == 2) {
      race_world::Place(machine, 1, 0, frame, calls.start);
      race_world::ListRacers(G::racers_manager(), frame - 1872, heap, calls.list);
      m.set_step(3);
    } else if (step == 3) {
      Post(dispatcher, 31);
      ChangeFlags(0, 0x100);
      Post(dispatcher, 32);
      ChangeFlags(0, 0x2000);
      ChangeFlags(0, 0x4000);
      mode_tick::ScreenFadesFadeIn(G::fade_in_passes(), 0, 0, 0, 0);
      m.set_step(4);
    } else if (step == 4) {
      m.set_ticks(0);
      m.set_step(0);
      m.set_still_ticks(0);
      m.set_word_9632(0);
    }
    return;
  }
  const bool linked = int32_t(Kind()) == 6;
  if (!linked) {
    if (RacersClose(frame - 208) == 0) {
      m.set_ticks(900);
    } else {
      m.set_ticks(m.ticks() + 1);
    }
  }
  const uint32_t copy = frame + 80;
  if (!va::StagesCopy(copy, Stages(), 0xFFFFFFFF)) {
    calls.start.original(0x820FFD88, frame, copy, Stages(), 0xFFFFFFFFFFFFFFFFull);
  }
  if (m.settled() == 0) {
    if (RacersStill(m.still_ticks_address()) != 1 || !FadesDone()) {
      return;
    }
    if (!(int32_t(Kind()) == 6 && int32_t(Load(copy)) != 1)) {
      uint32_t settled = 0;
      if (!ai_drivers::AllSettled(frame - 6320, &settled)) {
        settled = calls.all_settled(frame);
      }
      if (int32_t(settled) == 0) {
        return;
      }
    }
    m.set_settled(1);
    ChangeFlags(0, 0x1);
    return;
  }
  if (m.settled_ticks() > 900) {
    if (int32_t(G::sign_in_state()) == 0) {
      G::set_sign_in_state(1);
      G::set_sign_in_reason(6);
    }
    return;
  }
  // The System Link case (kind 6, the copy's +0 not 1) is the original's
  // (StandingStartTickNative).
  Post(dispatcher, 26);
  m.set_still_ticks(0);
  m.set_word_9632(0);
  m.set_ticks(0);
  m.set_settled(0);
  m.set_settled_ticks(0);
  ai_drivers::ClearAll(frame - 128);
}

}  // namespace race_states
