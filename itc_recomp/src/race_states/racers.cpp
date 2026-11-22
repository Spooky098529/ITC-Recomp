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

uint32_t Stages(uint8_t* base) {
  return G::race_data(base).stages_address();
}
// The racers' list's entry j (the manager's +2308).
uint32_t Racer(uint8_t* base, uint32_t j) {
  return Load(base, Manager(base, G::racers_manager(base)).racers_address() + 4 * j);
}
// The first racer record (from the race data's +48, its count +0x59610)
// flagged 0x1 (the player's): its index, or -1 with none.
int32_t PlayerRecord(uint8_t* base) {
  const schema::RaceData data = G::race_data(base);
  const int32_t count = int32_t(data.car_count());
  for (int32_t i = 0; i < count; i++) {
    if (Load(base, data.record0_flags_address() + kRecordBytes * uint32_t(i)) & 1) {
      return i;
    }
  }
  return -1;
}
// A racer's entry is in the current stage and isn't the player's.
bool OtherInStage(uint8_t* base, uint32_t entry) {
  const uint32_t record = RacerEntry(base, entry).record();
  if (va::StagesHasRacer(base, Stages(base), record, 0xFFFFFFFF) == 0) {
    return false;
  }
  return !(Load(base, G::race_data(base).record0_flags_address() + kRecordBytes * record) & 1);
}
// A route position's (the halfwords at `position`) slice center: the
// section's record (its unsigned halfword x 4 in the table) + 124 + 84 x
// the signed slice.
uint32_t SliceCenter(uint8_t* base, uint32_t position) {
  const uint32_t file = G::route(base);
  const schema::RoutePosition at(base, position);
  const uint32_t record =
      Load(base, RouteFile(base, file).sections() + (uint32_t(at.section()) << 2));
  return uint32_t(int64_t(S16(base, at.slice_address())) * RouteSlice::kSize) +
         RouteSection(base, record).slices_address();
}

}  // namespace

uint32_t RacersNear(uint8_t* base, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82165AB0 RaceStates::RacersNear");
  int32_t player_section = 0;
  const int32_t player = PlayerRecord(base);
  if (player >= 0) {
    const uint32_t entry = race_world::EntryOfRecord(base, uint32_t(player));
    const uint32_t vehicle = va::VehicleGet(base, RacerEntry(base, entry).vehicle_index());
    player_section = S16(base, TrackPos(base, va::VehicleIdentity(vehicle)).route_section_address());
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
    const uint32_t vehicle = va::VehicleGet(base, RacerEntry(base, entry).vehicle_index());
    const TrackPos identity(base, va::VehicleIdentity(vehicle));
    const uint32_t file = G::route(base);
    const uint32_t record = Load(base, RouteFile(base, file).sections() +
                                           (uint32_t(identity.route_section()) << 2));
    if (player_section == S16(base, identity.route_section_address())) {
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
    const RouteSlice center(base, SliceCenter(base, RacerEntry(base, entry).position_address()));
    Stfs(base, frame + 96, Lfs(base, center.center_x_address()));
    Stfs(base, frame + 100, Lfs(base, center.center_y_address()));
    Stfs(base, frame + 104, Lfs(base, center.center_z_address()));
  }
  for (int32_t j = 0; j < int32_t(track_collision::RaceRacerCount(base)); j++) {
    const uint32_t entry = Racer(base, uint32_t(j));
    va::VehicleGet(base, RacerEntry(base, entry).vehicle_index());
    if (!OtherInStage(base, entry)) {
      continue;
    }
    const RouteSlice center(base, SliceCenter(base, RacerEntry(base, entry).position_address()));
    Stfs(base, frame + 80, Fsubs(Lfs(base, frame + 96), Lfs(base, center.center_x_address())));
    Stfs(base, frame + 84, Fsubs(Lfs(base, frame + 100), Lfs(base, center.center_y_address())));
    Stfs(base, frame + 88, Fsubs(Lfs(base, frame + 104), Lfs(base, center.center_z_address())));
    const uint32_t vector = (frame + 80) & ~0xFu;
    const float dot = vl::Vec3Dot(base, vector, vector);
    const uint32_t result = (frame + 112) & ~0xFu;
    for (uint32_t k = 0; k < 16; k += 4) {
      guest::StoreFloat(base, result + k, dot);
    }
    const double distance = double(vl::Sqrtf(Lfs(base, frame + 112)));
    if (distance > Lfs(base, G::close_distance_address())) {
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
    const VehicleView vehicle(
        base, va::VehicleGet(base, RacerEntry(base, Racer(base, uint32_t(j))).vehicle_index()));
    if (schema::CollisionObject(base, vehicle.collision_object()).first_contact().address != 0 ||
        (vehicle.collision_flags() & 7)) {
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
  const Machine m(base, machine);
  if (RacersNear(base, frame - 208) == 0) {
    Post(base, dispatcher, 19);
    m.set_ticks(0);
    m.set_word_9620(0);
    ai_drivers::ClearAll(base, frame - 128);
    return;
  }
  const uint32_t ticks = m.ticks();
  if (ticks < 120) {
    m.set_ticks(ticks + 1);
    return;
  }
  Post(base, dispatcher, 28);
  m.set_ticks(0);
  m.set_word_9620(0);
  const schema::RaceData data = G::race_data(base);
  data.set_flags(data.flags() & ~0x1000u);
  // The original loads the pointer twice here (the store's, the load's).
  const schema::RaceData store_to = G::race_data(base);
  store_to.set_flags(G::race_data(base).flags() & ~0x2000u);
  ai_drivers::ResetAll(base, frame - 128);
}

namespace {

void ChangeFlags(uint8_t* base, uint32_t set, uint32_t clear) {
  const schema::RaceData data = G::race_data(base);
  data.set_flags((data.flags() | set) & ~clear);
}

}  // namespace

bool StandingStartTickNative(uint8_t* base, uint32_t machine) {
  const Machine m(base, machine);
  return !(int32_t(Kind(base)) == 6 && m.ticks() < 900 && m.settled() != 0 &&
           m.settled_ticks() <= 900 &&
           int32_t(va::StagesType(base, Stages(base), 0xFFFFFFFF)) != 1);
}

void StandingStartTick(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t frame,
                       bool heap, const StandingCalls& calls) {
  ITC_NATIVE_RUNNING("sub_821692C8 RaceStates::StandingStartTick");
  const Machine m(base, machine);
  if (RacersNear(base, frame - 208) == 0) {
    Post(base, dispatcher, 19);
    ai_drivers::ClearAll(base, frame - 128);
    return;
  }
  if (m.ticks() >= 900) {
    // The racers put back at the start, step by step.
    const uint32_t step = m.step();
    if (step == 0) {
      mode_tick::ScreenFadesFadeOut(base, G::fade_out_passes(base), 0, 0, 0, 0);
      m.set_step(1);
    }
    if (step <= 1) {
      if (!FadesDone(base)) {
        return;
      }
      Post(base, dispatcher, 31);
      ChangeFlags(base, 0x100, 0);
      ChangeFlags(base, 0x2000, 0);
      ChangeFlags(base, 0x4000, 0);
      race_world::Refresh(base, G::racers_manager(base), frame - 1904, heap, calls.refresh);
      m.set_step(2);
    } else if (step == 2) {
      race_world::Place(base, machine, 1, 0, frame, calls.start);
      race_world::ListRacers(base, G::racers_manager(base), frame - 1872, heap, calls.list);
      m.set_step(3);
    } else if (step == 3) {
      Post(base, dispatcher, 31);
      ChangeFlags(base, 0, 0x100);
      Post(base, dispatcher, 32);
      ChangeFlags(base, 0, 0x2000);
      ChangeFlags(base, 0, 0x4000);
      mode_tick::ScreenFadesFadeIn(base, G::fade_in_passes(base), 0, 0, 0, 0);
      m.set_step(4);
    } else if (step == 4) {
      m.set_ticks(0);
      m.set_step(0);
      m.set_still_ticks(0);
      m.set_word_9632(0);
    }
    return;
  }
  const bool linked = int32_t(Kind(base)) == 6;
  if (!linked) {
    if (RacersClose(base, frame - 208) == 0) {
      m.set_ticks(900);
    } else {
      m.set_ticks(m.ticks() + 1);
    }
  }
  const uint32_t copy = frame + 80;
  if (!va::StagesCopy(base, copy, Stages(base), 0xFFFFFFFF)) {
    calls.start.original(0x820FFD88, frame, copy, Stages(base), 0xFFFFFFFFFFFFFFFFull);
  }
  if (m.settled() == 0) {
    if (RacersStill(base, m.still_ticks_address()) != 1 || !FadesDone(base)) {
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
    m.set_settled(1);
    ChangeFlags(base, 0, 0x1);
    return;
  }
  if (m.settled_ticks() > 900) {
    if (int32_t(G::sign_in_state(base)) == 0) {
      G::set_sign_in_state(base, 1);
      G::set_sign_in_reason(base, 6);
    }
    return;
  }
  // The System Link case (kind 6, the copy's +0 not 1) is the original's
  // (StandingStartTickNative).
  Post(base, dispatcher, 26);
  m.set_still_ticks(0);
  m.set_word_9632(0);
  m.set_ticks(0);
  m.set_settled(0);
  m.set_settled_ticks(0);
  ai_drivers::ClearAll(base, frame - 128);
}

}  // namespace race_states
